// generating a pattern for structured lightning
#ifndef _generatePattern_hpp
#define _generatePattern_hpp

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


namespace pat
{
	struct GRAY_STR {
		int values[11];
	};
}

using namespace pat;
using namespace cv;

Mat* binarizeChannels(Mat img, Mat roi);
IplImage* generatePatternByDivision(int index);
IplImage* generatePatternGray(int index, GRAY_STR* tab);
GRAY_STR* generateGray();
GRAY_STR** assignToPlane(int numberOfPhotos);
void checkAndCreateDir(char filename1[], char filename2[]);
Mat createROI();
int findColumn(GRAY_STR gray);
int grayToInt(GRAY_STR gray, int len);

#endif