#include"RegionGrowing.h"

double EuclideanDistance(Vec3d color1, Vec3d color2);

region_growing::region_growing(int threshold, Mat image, Mat &regImg, map<int, colorInfo> &info){
	cThreshold = threshold;
	cImage = image;
	left_top_method(regImg, info);
	show_region_image(regImg, info);
}

void region_growing::left_top_method(Mat &regImg, map<int, colorInfo> &info){
	int row = cImage.rows;
	int col = cImage.cols;
	int newCodeName = 1;
	int code, dis;
	Vec3d color;

	regImg.at<int>(0, 0) = newCodeName;
	info[newCodeName].color = cImage.at<Vec3b>(0, 0);
	info[newCodeName].site.resize(1000);
	info[newCodeName].site[0] = Point(0, 0);
	newCodeName++;

	for (int i = 1; i < row; i++){
		code = regImg.at<int>(i - 1, 0);
		color = info[code].color;
		color /= info[code].count;

		dis = EuclideanDistance(color, cImage.at<Vec3b>(i, 0));
		if (dis <= cThreshold){
			regImg.at<int>(i, 0) = code;
			info[code].color += cImage.at<Vec3b>(i, 0);
			if (info[code].count == info[code].site.size()){
				info[code].site.resize(info[code].site.size() + 1000);
			}
			info[code].site[info[code].count] = Point(i, 0);
			info[code].count++;
		}
		else{
			regImg.at<int>(i, 0) = newCodeName;
			info[newCodeName].color = cImage.at<Vec3b>(i, 0);
			info[newCodeName].site.resize(1000);
			info[newCodeName].site[0] = Point(i, 0);
			newCodeName++;
		}
	}
	for (int j = 1; j < col; j++){
		code = regImg.at<int>(0, j - 1);
		color = info[code].color;
		color /= info[code].count;

		dis = EuclideanDistance(color, cImage.at<Vec3b>(0, j));
		if (dis <= cThreshold){
			regImg.at<int>(0, j) = code;
			info[code].color += cImage.at<Vec3b>(0, j);
			if (info[code].count == info[code].site.size()){
				info[code].site.resize(info[code].site.size() + 1000);
			}
			info[code].site[info[code].count] = Point(0, j);
			info[code].count++;
		}
		else{
			regImg.at<int>(0, j) = newCodeName;
			info[newCodeName].color = cImage.at<Vec3b>(0, j);
			info[newCodeName].site.resize(1000);
			info[newCodeName].site[0] = Point(0, j);
			newCodeName++;
		}
	}
	int codeL, codeU, disL, disU, count, index;
	Vec3d colorL, colorU;
	for (int i = 1; i < row; i++){
		for (int j = 1; j < col; j++){
			//if (i == 3 && j == 16) system("pause");
			//if (newCodeName < 0) system("pause");
			codeL = regImg.at<int>(i, j - 1);
			codeU = regImg.at<int>(i - 1, j);
			colorL = info[codeL].color;
			colorL /= info[codeL].count;
			if (codeL < 0 || codeU < 0) system("pause");

			disL = EuclideanDistance(colorL, cImage.at<Vec3b>(i, j));
			if (codeL == codeU){
				if (disL <= cThreshold){
					regImg.at<int>(i, j) = codeL;
					info[codeL].color += colorL;
					if (info[codeL].count == info[codeL].site.size()){
						info[codeL].site.resize(info[codeL].site.size() + 1000);
					}
					info[codeL].site[info[codeL].count] = Point(i, j);
					info[codeL].count++;
				}
				else{
					regImg.at<int>(i, j) = newCodeName;
					info[newCodeName].color += cImage.at<Vec3b>(i, j);
					info[newCodeName].site.resize(1000);
					info[newCodeName].site[0] = Point(i, j);
					newCodeName++;
				}
			}
			else{
				colorU = info[codeU].color;
				colorU /= info[codeU].count;
				disU = EuclideanDistance(colorU, cImage.at<Vec3b>(i, j));
				if (disL <= cThreshold && disU <= cThreshold){
					if (info[codeL].count >= info[codeU].count){
						regImg.at<int>(i, j) = codeL;
						count = info[codeL].count;
						info[codeL].color += info[codeU].color;
						info[codeL].color += cImage.at<Vec3b>(i, j);
						info[codeL].count += info[codeU].count + 1;
						if (info[codeL].site.size() <= info[codeL].count){
							info[codeL].site.resize(info[codeL].count + 1000);
						}
						info[codeL].site[count++] = Point(i, j);
						index = 0;
						for (vector<Point>::iterator it = info[codeU].site.begin(); index < info[codeU].count; it++, index++){
							regImg.at<int>(it->x, it->y) = codeL;
							info[codeL].site[count] = *it;
							count++;
						}info.erase(codeU);
					}
					else{
						regImg.at<int>(i, j) = codeU;
						count = info[codeU].count;
						info[codeU].color += info[codeL].color;
						info[codeU].color += cImage.at<Vec3b>(i, j);
						info[codeU].count += info[codeL].count + 1;
						if (info[codeU].site.size() <= info[codeU].count){
							info[codeU].site.resize(info[codeU].count + 1000);
						}
						info[codeU].site[count++] = Point(i, j);
						index = 0;
						for (vector<Point>::iterator it = info[codeL].site.begin(); index < info[codeL].count; it++, index++){
							regImg.at<int>(it->x, it->y) = codeU;
							info[codeU].site[count] = *it;
							count++;
						}info.erase(codeL);
					}
				}
				else if (disL > cThreshold && disU <= cThreshold){
					regImg.at<int>(i, j) = codeU;
					info[codeU].color += colorU;
					if (info[codeU].count == info[codeU].site.size()){
						info[codeU].site.resize(info[codeU].site.size() + 1000);
					}
					info[codeU].site[info[codeU].count] = Point(i, j);
					info[codeU].count++;
				}
				else if (disL <= cThreshold && disU > cThreshold){
					regImg.at<int>(i, j) = codeL;
					info[codeL].color += colorL;
					if (info[codeL].count == info[codeL].site.size()){
						info[codeL].site.resize(info[codeL].site.size() + 1000);
					}
					info[codeL].site[info[codeL].count] = Point(i, j);
					info[codeL].count++;
				}
				else{
					regImg.at<int>(i, j) = newCodeName;
					info[newCodeName].color += cImage.at<Vec3b>(i, j);
					info[newCodeName].site.resize(1000);
					info[newCodeName].site[0] = Point(i, j);
					newCodeName++;
				}
			}
		}
	}
	for (map<int, colorInfo>::iterator it = info.begin(); it != info.end(); it++){
		it->second.color /= it->second.count;
	}
}
void region_growing::show_region_image(Mat regImg, map<int, colorInfo> info){
	Mat segImg = Mat(regImg.size(), cImage.type());
	for (map<int, colorInfo>::iterator it = info.begin(); it != info.end(); it++){
		for (vector<Point>::iterator vit = it->second.site.begin(); vit != it->second.site.end(); vit++){
			segImg.at<Vec3b>(vit->x, vit->y) = it->second.color;
		}
	}
	imshow("Segmentation Image", segImg);
	waitKey(0);
}
double EuclideanDistance(Vec3d color1, Vec3d color2){
	double dis = 0;
	for (int i = 0; i < 3; i++){
		dis += pow(color1[i] - color2[i], 2);
	}return sqrt(dis);
}