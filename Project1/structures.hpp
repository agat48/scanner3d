

class cameraRay {
private:
	double x, y;
	cameraRay() {
		x = 0;
		y = 0;
	}
public:
	void setParams(double a, double b);
	double* getParams();

};

class projectorPlane {
private:
	double A, B, C, D;
	projectorPlane() {
		A = B = C = D = 0;
	}
public:
	void setParams(double a, double b, double c, double d);
	double* getParams();
};