#ifndef _structures_hpp
#define _structures_hpp

#include "calculationMath.hpp"

class cameraRay {
private:
	double dx, dy;
	int x, y;
public:
	cameraRay() :dx(0), dy(0), x(0), y(0){}
	~cameraRay();
	void setParams(double a, double b);
	double* getParams();
	void setCoords(int x, int y);
	int* getCoords();

};

class projectorPlane {
private:
	double A, B, C, D;
	int index;
public:
	projectorPlane():A(0),B(0),C(0),D(0) {}
	~projectorPlane();
	void setParams(double a, double b, double c, double d);
	double* getParams();
	void setIndex(int i);
	int getIndex();
};

double* calculateCoordinates(cameraRay camR, projectorPlane projPl, MatrixObj* Q);

cameraRay** generateRayCoords();
projectorPlane* generatePlaneCoords();
double* switchCoordsToLocal(projectorPlane planeLocal, MatrixObj* Q);
MatrixObj* calculateQ();

#endif