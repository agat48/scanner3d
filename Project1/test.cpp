#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream> 
//#include "generatePattern.hpp"
#include "handleCapture.hpp"
#include "calculationMath.hpp"


#define WIDTH_CONST 1600
#define HEIGHT_CONST 1200
using namespace cv;
using namespace std;

int WIDTH = WIDTH_CONST;
int HEIGHT = HEIGHT_CONST;

int main(int argc, char** argv)
{
//	capture();
	//	*/
	MatrixObj mat(4, 4);
	mat.writeMatrix();
	RMatrix rmatr;
	rmatr.writeMatrix();
	RMatrix rmatr2(R_ROLL,0.4);
	rmatr.setVal(4, 3, 3);
	rmatr.setVal(2, 0, 0);
	MatrixObj multip = rmatr.multiplMatrix(rmatr2);
	multip.writeMatrix();
	system("pause");

	/*
	Mat sample(255, 255, CV_8UC3);
	Mat sample2(sample);
	for (int i = 0; i < sample.rows; i++) {
		for (int j = 0; j < sample.cols; j++) {
			sample.at<Vec3b>(i, j)[0] = 50;
			sample.at<Vec3b>(i, j)[1] = i;
			sample.at<Vec3b>(i, j)[2] = j;
		}
	}
	namedWindow("Pattern window", WINDOW_AUTOSIZE);
	cvtColor(sample, sample2, CV_Lab2BGR, 0);
	imshow("Display window", sample2);
	waitKey(0); */
//	GRAY_STR** assignedValues = assignToPlane(11, dWidth, dHeight);

	return 0;


}
