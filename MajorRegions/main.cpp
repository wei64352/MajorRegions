#include"GaussianBlur.h"
#include"RegionGrowing.h"

int main(int argc, char* argv[]){
	Mat image = imread("1.jpg", -1);
	if (image.empty()){
		return 0;
	}
	Mat resImg;
	resize(image, resImg, Size(256, 256), 0, 0, CV_INTER_LINEAR);
	gaussian gau(5, 1, resImg);
	map<int, colorInfo> info;
	Mat code = Mat(resImg.size(), CV_32SC1);
	region_growing rg(35, resImg, code, info);
}