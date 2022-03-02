#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

/*
	1.Filtrage linéaire (filtre moyenneur, filtre gaussien)
	2.Filtrage non linéaire (filtre médian, filtres morphologiques)
*/

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat src = imread("/home/lhe/Images/Histogram_Calculation_Original_Image.jpg");
    imshow("Source image", src );
    
	/*
		filtre moyenneur
    	blur(src, dst,Size ksize, point, borderType = BORDER_DEFAULT)    
    */ 
    Mat blur;
    
    cv::blur(src,blur,Size(151,151));
    imshow("Blur Image", blur );
    
    /*
    	filtre gaussien
    	GaussianBlur(src,dst,Size ksize,sigmaX,sigmaY=0,borderType)
    */
    Mat gaussian;
	
	GaussianBlur(src, gaussian, Size(45, 45), 0);
    imshow("Gaussian Image", gaussian );
    
    /*
    	filtre médian
    	medianBlur(src,dst,int ksize)
    */
    Mat median;
    
    medianBlur(src,median,45);
    imshow("Median Image", median);
    
    /*
    	filtre bilateral
    	bilateralFilter(src,dst,int d,double sigmaColor, double sigmaSpace, int borderType )
    */
    Mat bilateral;
    
    bilateralFilter(src,bilateral,100,20.0,20.0);       
    imshow("Bilateral Image", bilateral);
    
    /*
    	filtre box
    	boxFilter(src,dst,int ddepth,Size kSize, Point anchor, bool normalize=true, int bordertype)
    */
    Mat box;
    
    boxFilter(src,box,-1,Size(45,45));   
    imshow("Box Image", box);    
    
    /*
		Filtres dérivateurs
    */
    
    // filtre sobel
    
    Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    
    Mat sobel; 
    Sobel(src_gray, sobel, -1,1,1);
   
    
    imshow("Sobel Image", sobel);        
    
    
    
    
    
	
	
    waitKey();
    return EXIT_SUCCESS;
}