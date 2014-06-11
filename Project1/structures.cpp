#include <cmath>
#include "structures.hpp"
#include "calculationMath.hpp"
#include <iostream>

extern int WIDTH;
extern int HEIGHT;

extern double alpha;
extern double beta;
extern double gamma;

extern double sx;
extern double sy;
extern double sz;

extern double roll;
extern double pitch;
extern double yaw;

using namespace std;

cameraRay::~cameraRay(){
//	cout << "camera ray destructor" << endl;
}
void cameraRay::setParams(double a, double b) {
	dx = a;
	dy = b;
}
double* cameraRay::getParams() {
	double *vals = new double[2];
	vals[0] = dx;
	vals[1] = dy;
	return vals;
}
void cameraRay::setCoords(int xn, int yn) {
	x = xn;
	y = yn;
}
int* cameraRay::getCoords() {
	int* coords = new int[2];
	coords[0] = x;
	coords[1] = y;
	return coords;
}
projectorPlane::~projectorPlane() {
//	cout << "projector plane destructor" << endl;
}
void projectorPlane::setParams(double a, double b, double c, double d) {
	A = a;
	B = b;
	C = c;
	D = d;
}
double* projectorPlane::getParams() {
	double* vals = new double[4];
	vals[0] = A;
	vals[1] = B;
	vals[2] = C;
	vals[3] = D;
	return vals;
}
void projectorPlane::setIndex(int i) {
	index = i;
}
int projectorPlane::getIndex() {
	return index;
}
double* calculateCoordinates(cameraRay camR, projectorPlane projPl, MatrixObj* Q) {

	double* coords=new double[3];
	double* planeCoords = switchCoordsToLocal(projPl, Q);
	coords[0] = camR.getCoords()[0];
	coords[1] = camR.getCoords()[1];
	coords[2] = -planeCoords[3] / (planeCoords[0] * camR.getParams()[0] + planeCoords[1] * camR.getParams()[1] + planeCoords[2]);
	return coords;
}

//create matrix with rays - each for camera column and row
cameraRay** generateRayCoords() {
	cameraRay** rays;
	rays = new cameraRay*[HEIGHT];
	double dx = tan(toRadians(alpha / 2));
	double dy = tan(toRadians(beta / 2));

	for (int i = 0; i < HEIGHT; i++) {
		rays[i] = new cameraRay[WIDTH];
	}
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			rays[i][j].setParams(dx*((2 * j) / (WIDTH - 1) - 1), dy*((2 * i) / (HEIGHT - 1) - 1));
			rays[i][j].setCoords(i, j);
		}
	}
	return rays;
}
//create table with planes - each for projector column
projectorPlane* generatePlaneCoords() {
	projectorPlane* planes;
	double c = tan(toRadians(gamma / 2));
	planes = new projectorPlane[WIDTH];
	for (int i = 0; i < WIDTH; i++) {
		planes[i].setParams(i / (WIDTH - 1) - 1, 0, c, 0);
	}
	return planes;
}

double* switchCoordsToLocal(projectorPlane planeLocal, MatrixObj* Q) {
	double* vals=new double[4];
	MatrixObj *planeCoords, planeLocalCoords(planeLocal.getParams(), 4);
	planeLocalCoords.setName("planeLocalCoords");
	planeCoords = Q->multiplMatrix(&planeLocalCoords);
	planeCoords->setName("planeCoords");
	for (int i = 0; i < 4; i++) {
		vals[i] = planeCoords->getVal(i, 0);
	}
	return vals;

}