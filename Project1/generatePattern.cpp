#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cmath>

struct GRAY_STR {
	int values[11];
};

IplImage* generatePatternByDivision(int index, int WIDTH, int HEIGHT) {

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
IplImage* generatePatternGray(int index, int WIDTH, int HEIGHT, GRAY_STR* tab) {
	IplImage* image = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
	cvSet(image, cvScalar(0, 0, 0), 0);
	int i = index;
	int greenLines1 = pow(2, i - 1);
	int greenLines2 = pow(2, i);
	for (int y = 0; y < image->height; y++) {
		uchar* ptr = (uchar*)(image->imageData + y * image->widthStep);
		for (int x = 0; x < image->width; x++) {
			if (tab[x].values[i-1])
			{
				ptr[3 * x] = 255; //Set blue to max (BGR format)
			}
			else
			{
				ptr[3 * x + 2] = 255; //Set red to max (BGR format)
			}
			for (int n = 0; n < 10; n++) //generowanie zielonych pasów
			{
/*				if (ptr[3 * (greenLines1 + n*greenLines2)])
				{
					ptr[3 * (greenLines1 + n*greenLines2)] = 0;
					ptr[3 * (greenLines1 + n*greenLines2) + 1] = 255;
					ptr[3 * (greenLines1 + n*greenLines2) + 2] = 0;
				}*/
			}
			
		}
	}
	return image;
}
GRAY_STR* generateGray(int WIDTH) {
	GRAY_STR* table=new GRAY_STR[WIDTH];
	int temp;
	for (int i = 0; i < WIDTH; i++)
	{
		temp = i;
		for (int j = 0; j < 10; j++)
		{
			table[i].values[10 - j] = temp % 2;
			temp >>= 1;
		}
		table[i].values[0] = temp;
		for (int j = 1; j < 11; j++)
		{
			table[i].values[j] = (table[i].values[j - 1] + table[i].values[j]) % 2;
		}
	}
	return table;
}