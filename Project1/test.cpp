#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream> 

#include "generatePattern.hpp"


#define WIDTH 1600
#define HEIGHT 1200
using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	IplImage* image;
	namedWindow("Display window", WINDOW_AUTOSIZE);
	for (int i = 1; i < 10; i++)
	{
		image = generatePattern(i, WIDTH, HEIGHT);
		cvShowImage("Display window", image); // Show our image inside it.
		fflush(stdin);
		waitKey(0);
	}

	/*
	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	if (capture == NULL){
		printf("ERROR: No camera was detected by OpenCV!\n");
		printf("Press any key to exit.\n");
		waitKey(0);
		return -1;
	}
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, WIDTH);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);

	IplImage* cam_frame = cvQueryFrame2(capture, &sl_params);
	if (cam_frame == NULL){
		printf("ERROR: No frame was available!\n");
		printf("Press any key to exit.\n");
		waitkey(0);
		return -1;
	}
	*/
	/*
	VideoCapture cap(0); // open the video camera no. 0

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;
	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	
//	while (1)
	{
		Mat frame;
		fflush(stdin);
		waitKey(0);

		bool bSuccess = cap.read(frame); // read a new frame from video
		bSuccess = cap.read(frame);

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
	//		break;
		}

		imshow("MyVideo", frame); //show the frame in "MyVideo" window
		imwrite("./cap2.jpg", frame);
		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
	//		break;
		}
	}
	*/
	destroyWindow("Display window");
	waitKey(0); // Wait for a keystroke in the window
	Mat img=imread("./cap1.jpg", 1);
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", img);
	return 0;
}
