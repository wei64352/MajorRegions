#ifndef FEATURE_H_
#define FEATURE_H_

#include<fstream>
#include<cv.h>
#include<highgui.h>
#include<map>

#include"RegionGrowing.h"

using namespace cv;
using namespace std;

struct feaRegion{
	vector<Point> site;
	vector<float> cnv;
};
struct vecColorInfo{
	int name;
	int count;
};

class feature{
private:
	vector<vector<float>> cnvMat;
	int cRegThres;
	int cCutSize;
	int cQuaSize;
	int cColorRange;
public:
	feature(int regThres, int cutSize);
	~feature();
	bool color_name_vector_load(int colorNum, int colorSize, string fileName);
	void localized_feature(map<int, colorInfo> &info, Mat &regImg, vector<float> &feature);
	void top_cluster_feature(map<int, colorInfo> &info, Mat &regImg, vector<float> &feature);
	void merge_feature(map<int, colorInfo> &info, Mat &regImg, vector<float> &feature, int topThres, int regNum);
	void produce(Mat &regImg, map<int, feaRegion> &feaReg, vector<float> &feature);
};

#endif