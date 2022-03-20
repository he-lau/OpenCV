#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;


// fonction pour déplacer une image
Mat translateImg(Mat &img, int offsetx, int offsety){
    Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img,img,trans_mat,img.size());
    return img;
}

int main ( int argc, char **argv )
{

	/*
	 *
	 * 1.1 Extraire la personne de la figure 1 « Mon image »
	 *
	 */

	// On récupère les images
	Mat front = imread("C:\\Users\\helau\\Downloads\\front.png");
	Mat back = imread("C:\\Users\\helau\\Downloads\\back.png");


	/*
	 *	WARNING :
	 * 	front : 684*527 back : 687*526 !
	 *
	 */
	resize(front,front,Size(684,527));
	resize(back,back,Size(684,527));


	// Convertir en niveaux de gris pour faciliter le travail
	cvtColor(front,front,COLOR_BGR2GRAY);
	cvtColor(back,back,COLOR_BGR2GRAY);

	// Soustraction des images pour détecter
	Mat diff;
	// abs() pour garder les écarts négatifs
	diff = abs(front-back);

	// Eliminer le bruit restant avec un filtre
    medianBlur(diff,diff,31);

    // Seuillage pour pouvoir detecter les contours
	Mat binaryImage;

	threshold(diff,binaryImage , 20, 255,  THRESH_BINARY);


    Mat canny_output;

    // init tableaux des contours
    // tableaux 2D contenant tous les points
    vector<vector<Point>> contours;

    // topologie de l'image, informations sur le placement des contours
    // valeurs sous la forme [Next, Previous, First_Child, Parent]
    vector<Vec4i> hierarchy;

    // filtre canny pour garder seulement les contours
    Canny( binaryImage, canny_output,20, 20*2 );

    // on cherche tous les contours et on les stocks dans vector<vector<Point>> contours
    findContours( binaryImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    // la taille du contour le plus long
    double maxArea = 0;
    // indice du contour le plus long
    int maxAreaContourId = -1;

    // on parcours tous les contours
    for (int j = 0; j < int(contours.size()); j++) {

    	// le nombre de pixels non nuls du contour courant
        double newArea = cv::contourArea(contours.at(j));

        // si le contour courant est plus long que le max
        if (newArea > maxArea) {
        	// on actualise la valeur la plus long
            maxArea = newArea;
            // ainsi que son indice
            maxAreaContourId = j;
        }
    }


    // on recupere une nouvelle fois l'image pour avoir les couleurs
    Mat front_bgr;
    front_bgr = imread("C:\\Users\\helau\\Downloads\\front.png");

    // Creation du masque

    // init masque statique avec une matrice initialisée à 0 de la taille de front_bgr
    Mat mask = Mat::zeros(front_bgr.size(), front_bgr.type());

    // on dessine SEULEMENT le contour le plus long
    drawContours(mask, contours,maxAreaContourId,Scalar(255, 255, 255),FILLED);

    // Application du masque
    Mat person;
    // ET logique entre le masque et l'image contenant le personnage
    // on a notre personne extraite !
    bitwise_and(front_bgr,mask,person);

    imshow("person",person);

    /*
     *
     *	1.2 Déplacer la personne à gauche de la scène (ou du décor)
     *	en se servant de la figure 2 « Mon arrière plan »
     *
     */


    // init masque dynamique
    Mat mask_move = Mat::zeros(front_bgr.size(), front_bgr.type());

    // le deplacement souhaité
    int x = -200;
    int y = 0;

    // on parcours tous les points formant les contours
    for (size_t i=0; i<contours.size(); i++)
    {
        for (size_t j=0; j<contours[i].size(); j++)
        {
        	// on applique le deplacement à l'aide des attributs de la classe Point
            contours[i][j].x += x;
            contours[i][j].y += y;
        }
    }

    // on dessine SEULEMENT le contour le plus long
    drawContours(mask_move, contours,maxAreaContourId,Scalar(255, 255, 255),FILLED);


    Mat person_move;
    // appel de la fonction translateImg pour deplacer notre image
    // ici 200 pixels vers la gauche
    translateImg(front_bgr,-200,0);
    // on applique le masque
    bitwise_and(front_bgr,mask_move,person_move);

    //imshow("person move",person_move);

    // on recupere le fond coloré
    Mat back_bgr;
    back_bgr = imread("C:\\Users\\helau\\Downloads\\back.png");
	resize(back_bgr,back_bgr,person.size());

    /*
     *
     * Ajouter la personne sur le fond
     * méthode : simulation de transparence (alpha blending)
     * permet de superposer des images en jouant avec la transparence
     */

    // On procede au blending en suivant la formule :
    // front * mask * back = blending, dans notre : mask_move * back_bgr

    // On convertit les valeurs des matrices en floatant pour multiplier les images par des nombres décimaux sans deborder après une normalisation
    person_move.convertTo(person_move, CV_32FC3);
    back_bgr.convertTo(back_bgr, CV_32FC3);

    // Normalisation du masque
    mask_move.convertTo(mask_move, CV_32FC3, 1.0/255);

    // init mat sortie
    Mat blend = Mat::zeros(person_move.size(), person_move.type());

    // 1 - mask_move pour inverser noir et blanc et ajouter le personnage au fond
    multiply(Scalar::all(1.0)-mask_move, back_bgr, back_bgr);

    add(person_move, back_bgr, blend);

    imshow("blended", blend/255);



	//imshow("front", front);
	//imshow("back_bgr", back_bgr);
	//imshow("diff", diff);
	//imshow("treshold", binaryImage);
	//imshow("contours", canny_output);
	//imshow("mask", mask);
	//imshow("mask_move", mask_move);














	waitKey();
	return 0;
}
