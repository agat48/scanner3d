// generating a pattern for structured lightning
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


namespace pat
{
	struct GRAY_STR;
}

using namespace pat;
using namespace cv;

Mat* binarizeChannels(Mat img);
IplImage* generatePatternByDivision(int index);
IplImage* generatePatternGray(int index, GRAY_STR* tab);
GRAY_STR* generateGray();
GRAY_STR** assignToPlane(int numberOfPhotos);
void checkAndCreateDir(char filename1[], char filename2[]);