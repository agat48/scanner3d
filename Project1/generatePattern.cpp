#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cmath>

IplImage* generatePattern(int index, int WIDTH, int HEIGHT) {

	IplImage* image = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
	cvSet(image, cvScalar(0, 0, 0), 0);
	int i = index;
	int regionWidth = (int)image->width / pow(2, i);
	for (int y = 0; y < image->height; y++) {
		uchar* ptr = (uchar*)(image->imageData + y * image->widthStep);
		for (int k = 0; k < pow(2, (i - 1)); k++) {
			for (int x = 2 * k*regionWidth + 1; x < 2 * k*regionWidth + regionWidth - 1; x++) {
				ptr[3 * x] = 255;
			}
			for (int x = 2 * k*regionWidth + regionWidth + 1; x < 2 * (k + 1)*regionWidth; x++) {
				ptr[3 * x + 2] = 255; //Set red to max (BGR format)
			}
			ptr[3 * 2 * k*regionWidth + 1] = 255;
			ptr[3 * (2 * k*regionWidth + regionWidth) + 1] = 255;
		}
	}
	return image;
}
