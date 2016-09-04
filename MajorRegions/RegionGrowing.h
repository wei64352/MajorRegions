#ifndef REGIONGROWING_H_
#define REGIONGROWING_H_

#include<map>
#include<cv.h>
#include<highgui.h>

using namespace cv;
using namespace std;

struct colorInfo{
	Vec3d color;
	int count = 1;
	vector<Point> site;
};

class region_growing{
private:
	int cThreshold;
	Mat cImage;
public:
	region_growing(int threshold, Mat image, Mat &regImg, map<int, colorInfo> &info);
	void left_top_method(Mat &regImg, map<int, colorInfo> &info);
	void show_region_image(Mat regImg, map<int, colorInfo> info);
};

#endif