#ifndef _calculationMath_hpp
#define _calculationMath_hpp

enum R_TYPE {
	R_ROLL = 0,
	R_PITCH = 1,
	R_YAW = 2
};
struct CAMERA_RAY;

class MatrixObj {
private:
	int rows, cols;
	char name[30];
protected:
	double** coord;
public:
	MatrixObj() :MatrixObj(4, 4){};
	MatrixObj(int n, int m);
	MatrixObj(double* params, int n);
	virtual ~MatrixObj() {
//		std::cout << "Delete MatrixObj, name:" << name << std::endl;
		for (int i = 0; i < rows; i++)
		{
			delete[]coord[i];
		}
		delete[]coord;
	}
	int getRows();
	int getCols();
	void setVal(double val, int x, int y);
	double getVal(int x, int y);
	void eye();
	MatrixObj* multiplMatrix(MatrixObj* matr2);
	void writeMatrix();
	void setName(const char n[]);
};
class RMatrix : public MatrixObj {
private:
	R_TYPE type;
public:
	RMatrix():MatrixObj(4, 4) {
		eye();
		type = R_ROLL;
	}
	~RMatrix();
	RMatrix(R_TYPE t, double param);
};
double toRadians(double a);
MatrixObj* calculateQ();

#endif