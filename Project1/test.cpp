
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/*
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/common.h>
#include <pcl/common/transforms.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>
#include <pcl/console/print.h>
#include <pcl/io/pcd_io.h>
#include <iostream>
#include <flann/flann.h>
#include <boost/filesystem.hpp>
*/

#include <iostream> 
#include <fstream>
//#include "generatePattern.hpp"
#include "handleCapture.hpp"
#include "calculationMath.hpp"
#include "structures.hpp"


#define WIDTH_CONST 1600
#define HEIGHT_CONST 1200
#define PI 22/7

using namespace cv;
using namespace std;
//using namespace pcl;

int C_WIDTH = WIDTH_CONST;
int C_HEIGHT = HEIGHT_CONST;

extern int ROI_LEFT;
extern int ROI_TOP;
extern int ROI_RIGHT;
extern int ROI_BOTTOM;

//Logitech diagonal FOV = 75
double alpha = 63.063;
double beta = 49.421;
double gamma = 60;

double sx=-4;
double sy=0;
double sz=0;

double roll=0.4; //przechy³ w lewo
double pitch=-5; // pochylenie do przodu
double yaw=-20; // skrêt w lewo

int main(int argc, char** argv)
{
	capture();
	//
/*	MatrixObj mat(4, 4);
	mat.eye();
	mat.writeMatrix();
	RMatrix rmatr;
	rmatr.writeMatrix();
	RMatrix rmatr2(R_ROLL, roll);
	rmatr2.writeMatrix();
	MatrixObj multip = rmatr.multiplMatrix(rmatr2);
	multip.writeMatrix(); */
	//createROI();
	cout << "generateRayCoords" << endl;
	cameraRay** rays = generateRayCoords();
	cout << "generatePlaneCoords" << endl;
	projectorPlane* planes = generatePlaneCoords();
	cout << "assignToPlane" << endl;
	GRAY_STR** assignedValues = assignToPlane(11);
	double ***coordVals;
	int ROIwidth, ROIheight;
	ROIwidth = ROI_RIGHT - ROI_LEFT+1;
	ROIheight = ROI_BOTTOM - ROI_TOP+1;
	coordVals = new double**[C_HEIGHT];
	for (int i = 0; i < C_HEIGHT; i++) {
		coordVals[i] = new double*[C_WIDTH];
		for (int j = 0; j < C_WIDTH; j++) {
			coordVals[i][j] = new double[3];
		}
	}
	GRAY_STR gray_temp;
	int colNum;
	gray_temp = assignedValues[100][300];
	cout << gray_temp.values[0] << gray_temp.values[1] << gray_temp.values[2] << gray_temp.values[3] << gray_temp.values[4] << gray_temp.values[5] << gray_temp.values[6] << gray_temp.values[7] << gray_temp.values[8] << gray_temp.values[9] << gray_temp.values[10] << endl;
	cout << "calculateCoordinates" << endl;
	MatrixObj* Q = calculateQ();
//	PointCloud<PointXYZ>::Ptr cloud((new PointCloud<PointXYZ>));
	ofstream file;
	file.open("data.pcd");
	file << "WIDTH " << C_WIDTH << " HEIGHT " << C_HEIGHT << endl;
	for (int i = ROI_TOP; i < ROI_BOTTOM; i++) {
		for (int j = ROI_LEFT; j < ROI_RIGHT; j++) {
			gray_temp = assignedValues[i][j];
			colNum = findColumn(gray_temp);
			if (colNum >= 0) {
				coordVals[i][j] = calculateCoordinates(rays[i][j], planes[colNum],Q);
			}
			else {
				coordVals[i][j][0] = j;
				coordVals[i][j][1] = i;
				coordVals[i][j][2] = 0; //?
			}
			file << coordVals[i][j][0] << " " << coordVals[i][j][1] << " " << coordVals[i][j][2] << ";";
		}
		file << endl;
	}
	cout << coordVals[100][300][0] << " " << coordVals[100][300][1] << " " << coordVals[100][300][2] << " " << endl;
	waitKey(0);
	system("pause");

	//free memory
	cout << "free memory" << endl;
	file.close();
	for (int i = 0; i < C_HEIGHT; i++){
		for (int j = 0; j < C_WIDTH; j++) {
			delete[] coordVals[i][j];
		}
		delete[]coordVals[i];
		delete[]assignedValues[i];
		delete[]rays[i];
	}
	delete[]coordVals;
	delete[]assignedValues;
	delete[]rays;
	delete[]planes;
	return 0;


}
