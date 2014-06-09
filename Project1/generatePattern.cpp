#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include <cmath>
#include <Windows.h>

extern int WIDTH;
extern int HEIGHT;


namespace pat
{
	struct GRAY_STR {
		int values[11];
	};
}

using namespace pat;
using namespace std;
using namespace cv;


Mat* binarizeChannels(Mat img)
{
	int aParams[3][2], bParams[3][2];
	aParams[0][0] = 22;
	aParams[0][1] = 127;
	aParams[1][0] = -127;
	aParams[1][1] = 0;
	aParams[2][0] = -127;
	aParams[2][1] = 70;

	bParams[0][0] = 22;
	bParams[0][1] = 127;
	bParams[1][0] = 7;
	bParams[1][1] = 127;
	bParams[2][0] = -127;
	bParams[2][1] = -5;

	Mat* imgBin = new Mat[3];
	inRange(img, Scalar(0 * 255 / 100, aParams[2][0] + 128, bParams[2][0] + 128), Scalar(100 * 255 / 100, aParams[2][1] + 128, bParams[2][1] + 128), imgBin[0]);
	inRange(img, Scalar(0 * 255 / 100, aParams[1][0] + 128, bParams[1][0] + 128), Scalar(100 * 255 / 100, aParams[1][1] + 128, bParams[1][1] + 128), imgBin[1]);
	inRange(img, Scalar(0 * 255 / 100, aParams[0][0] + 128, bParams[0][0] + 128), Scalar(100 * 255 / 100, aParams[0][1] + 128, bParams[0][1] + 128), imgBin[2]);
	return imgBin;
}

IplImage* generatePatternByDivision(int index) {

	IplImage* image = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
	cvSet(image, cvScalar(0, 0, 0), 0);
	int i = index;
	int regionWidth = (int)(image->width / pow(2, i));
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
IplImage* generatePatternGray(int index, GRAY_STR* tab) {
	IplImage* image = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
	cvSet(image, cvScalar(0, 0, 0), 0);
	int i = index;
	int n;
	int wsp;
	int greenLines1 = (int)pow(2, (11-i));
	int greenLines2 = (int)pow(2, (11-i+1));
	for (int y = 0; y < image->height; y++) {
		n = 0;
		uchar* ptr = (uchar*)(image->imageData + y * image->widthStep);
		for (int x = 0; x < image->width; x++) {
			if (tab[x].values[i-1]==1)
			{
				ptr[3 * x] = 255; //Set blue to max (BGR format)
			}
			else
			{
				ptr[3 * x + 2] = 255; //Set red to max (BGR format)
			}
			while (wsp = 3 * (greenLines1 + n*greenLines2) < 3*image->width+2) //generowanie zielonych pasów
			{
				ptr[3 * (greenLines1 + n*greenLines2)] = 0;
				ptr[3 * (greenLines1 + n*greenLines2) + 1] = 255;
				ptr[3 * (greenLines1 + n*greenLines2) + 2] = 0;
				n++;
			}
			
		}
	}
	return image;
}
GRAY_STR* generateGray() {
	GRAY_STR* table=new GRAY_STR[WIDTH];
	int temp;
	for (int i = 0; i < WIDTH; i++)
	{
		temp = i;
		GRAY_STR temptab;
		for (int j = 0; j < 11; j++)
		{
			temptab.values[10 - j] = temp % 2;
			temp >>= 1;
		}
		table[i].values[0] = temptab.values[0];
		for (int j = 1; j < 11; j++)
		{
			table[i].values[j] = (temptab.values[j - 1] + temptab.values[j]) % 2;
		}
	}
	return table;
}

GRAY_STR** assignToPlane(int numberOfPhotos) {
	GRAY_STR** tab;
	tab = new GRAY_STR*[HEIGHT];
	for (int i = 0; i < HEIGHT; i++)
	{
		tab[i] = new GRAY_STR[WIDTH];
	}
	char filename[50];
	Mat img;
	Mat img2;
	Mat* imgBin;
	uchar temp;
	int val;
	for (int k = 0; k < numberOfPhotos; k++)
	{
		sprintf(filename, "./images/captured/cap%d.jpg", k+1);
		img = imread(filename, 1);
		cvtColor(img, img2, CV_BGR2Lab, 0);
		imgBin = binarizeChannels(img2);
		//check if pixel is red/green/blue/other
		for (int i = 0; i < img2.cols; i++) {
			for (int j = 0; j < img2.rows; j++) {
				temp = imgBin[2].at<Vec3b>(j, i)[0];
				if (temp == 255) {
					val = 0;
				}
				else {
					temp = imgBin[0].at<Vec3b>(i, j)[0];
					if (temp == 255) {
						val = 1;
					}
					else {
						temp = imgBin[1].at<Vec3b>(i, j)[0];
						if (temp == 255) {
							val = 2;
						}
						else val = -1;
					}
				}
				tab[i][j].values[k] = val;
			}
		}
	}
	return tab;
}

void checkAndCreateDir(char filename1[], char filename2[])
{
	CreateDirectoryA(filename1, NULL);
	CreateDirectoryA(filename2, NULL);
	cout << GetLastError() << endl;
}

