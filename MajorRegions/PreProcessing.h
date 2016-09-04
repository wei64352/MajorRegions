#ifndef PREPROCESSING_H_
#define PREPROCESSING_H_

#include<cv.h>
#include<highgui.h>

using namespace cv;

class pre_processing{
private:
	Mat oriImg;
public:
	void smoothing();
};

#endif