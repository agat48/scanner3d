#include "generatePattern.hpp"
#include <iostream>

extern int C_WIDTH;
extern int C_HEIGHT;

using namespace std;
using namespace pat;

void showPattern(int index, GRAY_STR* gr, IplImage* pattern);
int capturePattern(int index, VideoCapture cap);

int capture() {
	IplImage* pattern;
	namedWindow("Pattern window", WINDOW_AUTOSIZE);
	GRAY_STR* gr = generateGray();
	VideoCapture cap(0); // open the video camera no. 0
	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, C_WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, C_HEIGHT);
	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo
	checkAndCreateDir("./images", "./images/captured");

	pattern = cvCreateImage(cvSize(C_WIDTH, C_HEIGHT), IPL_DEPTH_8U, 3); //generatePatternGray(i, C_WIDTH, HEIGHT, gr);
	cvSet(pattern, cvScalar(255, 255, 255), 0);
	cvShowImage("Pattern window", pattern); // Show our image inside it.
	waitKey(0);
	capturePattern(0, cap);
	waitKey(2000);
	for (int i = 1; i < 12; i++)
	{
		showPattern(i, gr, pattern);
		waitKey(2000);
		if(capturePattern(i, cap)<0)
			break;
	}
	destroyWindow("Pattern window");
	destroyWindow("MyVideo");
	return 0;
}

void showPattern(int index, GRAY_STR* gr, IplImage* pattern) {
	pattern = generatePatternGray(index, gr);
	cvShowImage("Pattern window", pattern); // Show our image inside it.
}

int capturePattern(int index, VideoCapture cap) {
	Mat frame;
	char filename[50];

	bool bSuccess = cap.read(frame); // read a new frame from video
	cap.read(frame);

	if (!bSuccess) //if not success, break loop
	{
		cout << "Cannot read a frame from video stream" << endl;
		return -1;
	}

	imshow("MyVideo", frame); //show the frame in "MyVideo" window
	sprintf(filename, "./images/captured/cap%d.jpg", index);


	imwrite(filename, frame);
	return 0;
}