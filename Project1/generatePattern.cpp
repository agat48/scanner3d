#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include <cmath>
#include <Windows.h>

#include "generatePattern.hpp"

extern int WIDTH;
extern int HEIGHT;

int ROI_LEFT=0;
int ROI_TOP=0;
int ROI_RIGHT=0;
int ROI_BOTTOM=0;

using namespace pat;
using namespace std;
using namespace cv;

Mat createROI();
int grayToInt(GRAY_STR gray, int len);

Mat* binarizeChannels(Mat img, Mat roi)
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
	Mat img2;
	Mat imgGreen;
	Mat imgRef;
	cvtColor(img, img2, CV_BGR2Lab, 0);
	img2.copyTo(imgRef, roi);
	img.copyTo(imgGreen, roi);
	inRange(imgRef, Scalar(0 * 255 / 100, aParams[2][0] + 128, bParams[2][0] + 128), Scalar(100 * 255 / 100, aParams[2][1] + 128, bParams[2][1] + 128), imgBin[0]);
//	inRange(imgGreen, Scalar(180,180,180), Scalar(255,255,255), imgBin[1]);
//	inRange(imgRef, Scalar(0 * 255 / 100, aParams[1][0] + 128, bParams[1][0] + 128), Scalar(100 * 255 / 100, aParams[1][1] + 128, bParams[1][1] + 128), imgBin[1]);
	inRange(imgRef, Scalar(0 * 255 / 100, aParams[0][0] + 128, bParams[0][0] + 128), Scalar(100 * 255 / 100, aParams[0][1] + 128, bParams[0][1] + 128), imgBin[2]);
	//addWeighted(imgBin[0],1, imgBin[2],1,0,imgGreen);
	imgBin[1] = roi-(imgBin[0] + imgBin[2]);
//	imgGreen.copyTo(imgBin[1], roi);
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
	Mat roi = createROI();
	Mat z;
	uchar temp;
	int val;
	namedWindow("disp", WINDOW_AUTOSIZE);
	for (int k = 0; k < numberOfPhotos; k++)
	{
		sprintf(filename, "./images/captured/cap%d.jpg", k+1);
		img = imread(filename, CV_LOAD_IMAGE_COLOR);
		imgBin = binarizeChannels(img,roi);
		imshow("disp", imgBin[0]);
		waitKey(0);
		imshow("disp", imgBin[1]);
		waitKey(0);
		imshow("disp", imgBin[2]);
		waitKey(0);
		imshow("disp", img);
		waitKey(0);
		//check if pixel is red/green/blue/other
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				temp = imgBin[2].at<uchar>(i,j);
				if (temp == 255) {
					val = 0;
				}
				else {
					temp = imgBin[0].at<uchar>(i,j);
					if (temp == 255) {
						val = 1;
					}
					else {
						temp = imgBin[1].at<uchar>(i,j);
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
	destroyWindow("disp");
	delete[]imgBin;
	return tab;
}

void checkAndCreateDir(char filename1[], char filename2[])
{
	CreateDirectoryA(filename1, NULL);
	CreateDirectoryA(filename2, NULL);
	cout << GetLastError() << endl;
}
Mat createROI() {
	char filename[50];
	Mat img;
	Mat img2;
	Mat imgBin;
	Mat imgTemp;
/*	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy; */
	sprintf(filename, "./images/captured/cap%d.jpg", 0);
	img = imread(filename, 1);
	cvtColor(img, img2, CV_BGR2Lab, 0); 

	inRange(img2, Scalar(50 * 255 / 100, -127 + 128, -127 + 128), Scalar(100 * 255 / 100, 127 + 128, 127 + 128), imgTemp);
	morphologyEx(imgTemp, imgBin, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(5, 5)));
	//morphologyEx(imgTemp, imgBin, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(5, 5)));
	int ROIleft, ROItop, ROIright, ROIbottom;
	ROIleft = imgBin.cols;
	ROItop = imgBin.rows;
	ROIright = 0;
	ROIbottom = 0;
	uchar temp; 
	for (int i = 0; i < imgBin.rows; i++) {
		for (int j = 0; j < imgBin.cols; j++) {
			temp = imgBin.at<uchar>(i, j);
			if (temp==255) {
				if (i < ROItop) {
					ROItop = i;
				}
				if (j < ROIleft) {
					ROIleft = j;
				}if (imgBin.rows - i > ROIbottom) {
					ROIbottom = imgBin.rows - i;
				}
				if (imgBin.cols - j > ROIright) {
					ROIright = imgBin.cols - j;
				}

			}
		}
	}
	ROI_LEFT = ROIleft;
	ROI_TOP = ROItop;
	ROI_RIGHT = ROIright;
	ROI_BOTTOM = ROIbottom;
	cout << ROIleft << " " << ROItop << " " << ROIright << " " << ROIbottom << endl;
/*	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	rectangle(imgBin, Point(ROIleft, ROItop), Point(ROIbottom,ROIright), color, 2, 8, 0);
	Mat imgFinal;
	imgBin(Rect(ROIleft, ROItop, ROIright - ROIleft, ROIbottom - ROItop)).copyTo(imgFinal);
	cout << imgFinal.cols << endl;
/*	findContours(imgBin, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());
	cout << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}
	Mat drawing = Mat::zeros(imgBin.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
	}

	/// Show in a window */
	Mat drawing = Mat::zeros(imgBin.size(), CV_8UC3);
	namedWindow("ROI", CV_WINDOW_AUTOSIZE);
	imshow("ROI", imgTemp);
	waitKey(0);
	imshow("ROI", imgBin);
	waitKey(0);
	destroyWindow("ROI");
	return imgBin;
}

int findColumn(GRAY_STR gray) {
	int res=0;
	int* vals = gray.values;
	int i = 0;
	while (vals[i] >= 0 && vals[i] < 2 && i < 11) {
		i++;
	}
	if (i == 0 && vals[i] != 2){
		return -1;
	}
	res = grayToInt(gray, i);
	if (res < 0) {
		res = 0;
	}
	i = 11 - i;
	res = (int)(pow(2, i - 1) + pow(2, i)*res);
	return res;
}

int grayToInt(GRAY_STR gray, int len) {
	if (len == 0) {
		return -1;
	}
	else {
		int* vals = gray.values;
		int* bin = new int[len];
		bin[0] = vals[0];
		for (int i = 1; i < len; i++) {
			bin[i] = (bin[i - 1] + vals[i]) % 2;
		}
		int res = 0;
		for (int i = 0; i < len; i++) {
			res = res + bin[i] * (int)pow(2, len - i - 1);
		}
		delete[]bin;
		return res;
	}
	
}