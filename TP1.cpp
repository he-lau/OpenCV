/*
	TP1 traitement d'images
*/

/*
	QUESTIONS :
    	1 - Convertir une image RGB en Image de niveaux de gris (Gray Image)
    	2 - tracer l'histogramme d'une image couleur puis l'histogramme d'une image de niveaux de gris
    	3 - Img binaire avec seuillage
    	4 -
*/


#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

// Driver code
int main(int argc, char** argv)
{

	Mat src = imread("/home/lhe/Images/Histogram_Calculation_Original_Image.jpg");
	Mat grayImage;

	cvtColor(src,grayImage,COLOR_BGR2GRAY);

	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	vector<Mat> gray_planes;
	split(grayImage, gray_planes);

	int histSize = 256;

	float range[] = { 0, 256 };
	const float* histRange[] = { range };

	bool uniform = true, accumulate = false;

	Mat b_hist, g_hist, r_hist, gray_hist;

	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
	calcHist( &gray_planes[0], 1, 0, Mat(), gray_hist, 1, &histSize, histRange, uniform, accumulate );

	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(gray_hist, gray_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	for( int i = 1; i < histSize; i++ )
	{
    	line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
          	Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
          	Scalar( 255, 0, 0), 2, 8, 0  );
    	line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
          	Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
          	Scalar( 0, 255, 0), 2, 8, 0  );
    	line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
          	Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
          	Scalar( 0, 0, 255), 2, 8, 0  );
    	line( histImage, Point( bin_w*(i-1), hist_h - cvRound(gray_hist.at<float>(i-1)) ),
          	Point( bin_w*(i), hist_h - cvRound(gray_hist.at<float>(i)) ),
          	Scalar( 230, 230, 230), 2, 8, 0  );
	}

	Mat binaryImage;
	threshold(src,binaryImage , 200, 255, cv::THRESH_BINARY);


	// Affichage
	imshow("Color image", src);
	imshow("Gray Image", grayImage);
	imshow("calcHist Demo", histImage);
	imshow("calcHist Demo", binaryImage);

	// Wait for any keystroke
	waitKey();
	return 0;
}
