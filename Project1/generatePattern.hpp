// generating a pattern for structured lightning
#include <opencv2/highgui/highgui.hpp>

struct GRAY_STR;

IplImage* generatePatternByDivision(int index, int WIDTH, int HEIGHT);
IplImage* generatePatternGray(int index, int WIDTH, int HEIGHT, GRAY_STR* tab);
GRAY_STR* generateGray(int WIDTH);