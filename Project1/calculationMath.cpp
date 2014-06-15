#include <cmath>
#include <iostream>

#include "calculationMath.hpp"

#define PI 22/7

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


struct CAMERA_RAY {
	double A;
	double B;
	double C;
};

using namespace std;

MatrixObj::MatrixObj(int n, int m) {
	coord = new double*[n];
	for (int i = 0; i < n; i++) {
		coord[i] = new double[m];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			coord[i][j] = 0;
		}
	}
	rows = n;
	cols = m;
	sprintf_s(name, "empty");
}
MatrixObj::MatrixObj(double* params, int n):MatrixObj(n, 1) {
	for (int i = 0; i < n; i++) {
		coord[i][0] = params[i];
	}
}
int MatrixObj::getRows() {
	return rows;
}
int MatrixObj::getCols() {
	return cols;
}
void MatrixObj::setVal(double val, int x, int y) {
	coord[x][y] = val;
}
double MatrixObj::getVal(int x, int y) {
	return coord[x][y];
}
void MatrixObj::eye() {
	if (rows != cols)
		cout << "Matrix not square!" << endl;
	else {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (i == j)
					setVal(1, i, j);
				else
					setVal(0, i, j);
			}
		}
	}
}
MatrixObj* MatrixObj::multiplMatrix(MatrixObj* matr2) {
	int row2 = matr2->getRows();
	if (row2 != cols)
	{
		cout << "Incorrect matrix dimensions, cannot multiply!" << endl;
		return matr2;
	}
	else {
		double tempVal;
		int col2 = matr2->getCols();
		MatrixObj* newMatr;
		newMatr=new MatrixObj(rows, col2);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < col2; j++) {
				tempVal = 0;
				for (int k = 0; k < rows; k++)
				{
					tempVal = tempVal + getVal(i, k)*matr2->getVal(k, j);
				}
				newMatr->setVal(tempVal, i, j);
			}
		}
		return newMatr;
	}
}
void MatrixObj::writeMatrix() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << coord[i][j] << " ";
		}
		cout << endl;
	}
}
void MatrixObj::setName(const char n[]){
	sprintf_s(name, n);
}

RMatrix::~RMatrix() {
//	cout << "RMatrix delete, type:" << type << endl;
}

RMatrix::RMatrix(R_TYPE t, double param):MatrixObj(4,4) {
	type = t;
	eye();
	double sinval = sin(toRadians(param)), cosval = cos(toRadians(param));
	switch (t)
	{
	case R_ROLL:
		setVal(cosval, 0, 0);
		setVal(sinval, 1, 0);
		setVal(-sinval, 0, 1);
		setVal(cosval, 1, 1);
		break;
	case R_PITCH:
		setVal(cosval, 1, 1);
		setVal(sinval, 2, 1);
		setVal(-sinval, 1, 2);
		setVal(cosval, 2, 2);
		break;
	case R_YAW:
		setVal(cosval, 0, 0);
		setVal(-sinval, 2, 0);
		setVal(sinval, 0, 2);
		setVal(cosval, 2, 2);
		break;
	}
}
double toRadians(double a) {
	return a*PI / 180;
}
MatrixObj* calculateQ() {
	MatrixObj* Q;
	RMatrix rollM(R_ROLL, roll), pitchM(R_PITCH, pitch), yawM(R_YAW, yaw);
	MatrixObj S(4, 4);
	rollM.setName("rollM");
	pitchM.setName("pitchM");
	yawM.setName("yawM");
	S.setName("S");
	S.eye();
	S.setVal(sx, 3, 0);
	S.setVal(sy, 3, 1);
	S.setVal(sz, 3, 2);
	Q = S.multiplMatrix(&rollM);
	Q = Q->multiplMatrix(&pitchM);
	Q = Q->multiplMatrix(&yawM);
	Q->setName("Q");
	return Q;
}