#ifndef GAUSSINABLUR_H_
#define GAUSSINABLUR_H_

#define _USE_MATH_DEFINES

#include<cv.h>
#include<highgui.h>
#include<math.h>

using namespace cv;
using namespace std;

class gaussian{
private:
	vector<double> cMask;
	int cStart;
	int cSize;
	double cStandred;
public:
	gaussian(int size, int standred, Mat &image);
	void init_mask();
	void Gaussian_Blur(Mat &image);
};

#endif