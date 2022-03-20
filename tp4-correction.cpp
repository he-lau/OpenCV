#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat differenceNDG(Mat img1, Mat img2){
	Mat result;
	result.create(img1.size(),CV_8U);
	for(int x=0;x<img1.cols;x++){
		for(int y=0;y<img1.rows;y++){
			uchar val1 = img1.at<uchar>(x, y);
			uchar val2 = img2.at<uchar>(x, y);
			uchar val3;

			if(val2>val1)
				val3 = val2-val1;
			else
				val3= val1-val2;

			result.at<uchar>(x,y)=val3;
		}
	}
	return result;
}

int main(int argc, char** argv)
{
	// Ouverture des images couleurs
	Mat frontImg, backImg;
	frontImg = imread( "/home/<chemin_vers_votre_image>/front.png", IMREAD_COLOR );
	backImg = imread( "/home/<chemin_vers_votre_image>/back.png", IMREAD_COLOR );
	imshow("Front Image", frontImg ); imshow("Back Image", backImg );

	// Étape1: Conversion Image couleur => Image gris
	Mat frontGrayImg, backGrayImg;
	cvtColor(frontImg, frontGrayImg, COLOR_BGR2GRAY);
	cvtColor(backImg, backGrayImg, COLOR_BGR2GRAY);
	//imshow("Front Gray Image", frontGrayImg ); imshow("Back Gray Image", backGrayImg );

	// Étape2: Soustraction des deux images
	Mat resulSoustraction = differenceNDG(frontGrayImg, backGrayImg);
	imshow("Soustraction Image", resulSoustraction );

	// Étape3: Élimination de bruit
	Mat medianResult;
	medianBlur(resulSoustraction, medianResult, 5);
	//imshow("Median Image", medianResult);

	// Étape4: Binarisation de l'image
	Mat binResult;
	threshold(medianResult, binResult, 50, 255, THRESH_OTSU + THRESH_BINARY);
	//imshow("Binary Result Soustraction Image", binResult );

	// Étape 5: Dilatation
	dilate(binResult, binResult, Mat(), Point(-1,-1),13);
	//imshow("Binary Dilate Image", binResult );

	// Étape 6: Érosion
	erode(binResult, binResult, Mat(), Point(-1,-1), 11);
	//imshow("Binary Erode Image", binResult );

	// Étape 7 (étape finale)
	Mat finalBack = backImg.clone();
	//imshow("finalBack copie image back",finalBack);
	Mat finalFront = frontImg.clone();
	//imshow("finalFront copie image front",finalFront);
	Mat mask = binResult.clone();
	//imshow("mask Image", mask);

	for (int j=0; j<mask.cols; j++){
		for (int i=0; i<mask.rows; i++){
			if (mask.at<uchar>(i, j)!=0){
				finalBack.at<cv::Vec3b>(i,j-150)[0] = finalFront.at<cv::Vec3b>(i,j)[0];
				finalBack.at<cv::Vec3b>(i,j-150)[1] = finalFront.at<cv::Vec3b>(i,j)[1];
				finalBack.at<cv::Vec3b>(i,j-150)[2] = finalFront.at<cv::Vec3b>(i,j)[2];
			}

		}
	}

	imshow("Image Resultat Final", finalBack);

    waitKey();
    return EXIT_SUCCESS;

}
