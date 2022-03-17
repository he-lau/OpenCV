#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{



	int x = 500;
	int y = 500;


	Mat img1(x, y, CV_8UC3, Scalar(0,0,0));

	// BGR

	int i = 50;

	rectangle(img1, Point(i, i), Point(x-i, y-i),Scalar(255, 255, 255),2);
	rectangle(img1, Point(i*2, i*2), Point(x-i*2, x-i*2),Scalar(255, 0, 0),2);
	rectangle(img1, Point(i*3, i*3), Point(x-i*3, y-i*3),Scalar(0, 255, 0),2);
	rectangle(img1, Point(i*4, i*4), Point(x-i*4, y-i*4),Scalar(0, 0, 255),2);

	Point p1 = Point(0,0);
	Point p2 = Point(250,0);
	//Point p3 = Point(500,0);

	Point p4 = Point(0,250);
	Point p5 = Point(250,250);
	Point p6 = Point(500,250);

	//Point p7 = Point(0,500);
	Point p8 = Point(250,500);
	Point p9 = Point(500,500);

	Mat img2(x, y, CV_8UC3, Scalar(0,0,0));

	//imshow("Output", img1);

	rectangle(img2, p1+Point(25,25), p5-Point(25,25),Scalar(255, 255, 255),2);
	rectangle(img2, p2+Point(25,25), p6-Point(25,25),Scalar(255, 255, 255),2);
	rectangle(img2, p4+Point(25,25), p8-Point(25,25),Scalar(255, 255, 255),2);
	rectangle(img2, p5+Point(25,25), p9-Point(25,25),Scalar(255, 255, 255),2);

	imshow("img2", img2);

	Mat img3(x, y, CV_8UC3, Scalar(0,0,0));






	 waitKey();
	return EXIT_SUCCESS;

}
