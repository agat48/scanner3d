#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

IplImage* createPatternByIndex(int index, int width, int height) {
	IplImage* img = cvCreateImage(cvSize(width, height), 8, 3);
	cvSet(img, cvScalar(1, 0, 0));
	return img;
}
