#include"Feature.h"

void quick_sort(vector<vecColorInfo> &vec, int left, int right);

feature::feature(int regThres, int cutSize){
	cRegThres = regThres;
	cCutSize = cutSize;
}
feature::~feature(){
	cnvMat.~vector();
}
bool feature::color_name_vector_load(int colorNum, int colorSize, string fileName){
	cQuaSize = pow(16777216 / colorNum, 1 / 3);
	cColorRange = 256 / cQuaSize;
	ifstream fin(fileName);
	if (fin){
		istream_iterator<float> fit(fin);
		istream_iterator<float> end;
		cnvMat.resize(colorNum, vector<float>(colorSize));
		int i = 0;
		while (fit != end){
			for (int j = 0; j < colorSize; j++){
				cnvMat[i][j] = *fit++;
			}i++;
		}fin.close;
		printf("Loading color name file successful...");
		return true;
	}
	else{
		printf("Error: can't find the color name vector file, name \"%s \"", fileName);
		return false;
	}
}
void feature::localized_feature(map<int, colorInfo> &info, Mat &regImg, vector<float> &feature){
	map<int, feaRegion> feaReg;
	int code;
	for (map<int, colorInfo>::iterator it = info.begin(); it != info.end(); it++){
		if (it->second.count >= cRegThres){
			feaReg[it->first].site = it->second.site;
			feaReg[it->first].cnv.resize(cnvMat[0].size());
			code =
				it->second.color[2] / cQuaSize * pow(cColorRange, 2) +
				it->second.color[1] / cQuaSize * cColorRange +
				it->second.color[0] / cQuaSize;
			feaReg[it->first].cnv = cnvMat[code];
		}
	}
	produce(regImg, feaReg, feature);
}
void feature::top_cluster_feature(map<int, colorInfo> &info, Mat &regImg, vector<float> &feature){
	vector<vecColorInfo> cBuf(info.size());
	int index = 0;
	for (map<int, colorInfo>::iterator it = info.begin(); it != info.end(); it++){
		cBuf[index].name = it->first;
		cBuf[index].count = it->second.count;
		index++;
	}
	quick_sort(cBuf, 0, index - 1);
	int maxPixel = (regImg.rows * regImg.cols) * cRegThres;
	int pxlCount = 0, code;
	map<int, feaRegion> feaReg;
	for (vector<vecColorInfo>::iterator it = cBuf.begin(); it != cBuf.end(); it++){
		if (pxlCount < maxPixel){
			feaReg[it->name].site = info[it->name].site;
			feaReg[it->name].cnv.resize(cnvMat[0].size());
			code =
				info[it->name].color[2] / cQuaSize * pow(cColorRange, 2) +
				info[it->name].color[1] / cQuaSize * cColorRange +
				info[it->name].color[0] / cQuaSize;
			feaReg[it->name].cnv = cnvMat[code];
			pxlCount += it->count;
		}
	}
	produce(regImg, feaReg, feature);
}
void feature::merge_feature(map<int, colorInfo> &info, Mat &regImg, vector<float> &feature, int topThres, int regNum){
	int count = 0;
	for (map<int, colorInfo>::iterator it = info.begin(); it != info.end(); it++){
		if (it->second.count >= cRegThres){
			count++;
		}
	}
	if (count > regNum){
		localized_feature(info, regImg, feature);
	}else{
		cRegThres = topThres;
		top_cluster_feature(info, regImg, feature);
	}
}
void feature::produce(Mat &regImg, map<int, feaRegion> &feaReg, vector<float> &feature){
	Mat feaImg = Mat(regImg.size(), regImg.type());
	for (map<int, feaRegion>::iterator it = feaReg.begin(); it != feaReg.end(); it++){
		for (vector<Point>::iterator vit = it->second.site.begin();
			vit != it->second.site.end(); vit++){
			feaImg.at<int>(vit->x, vit->y) = it->first;
		}
	}
	int maxCount, maxCode;
	map<int, int> buf;
	vector<float>::iterator fit = feature.begin();
	for (int i = 0; i < feaImg.rows; i += cCutSize){
		for (int j = 0; j < feaImg.cols; j += cCutSize){
			maxCount = -1;
			for (int x = i; x < i + cCutSize; x++){
				for (int y = j; y < j + cCutSize; y++){
					if (feaImg.at<int>(x, y) > 0){
						buf[feaImg.at<int>(x, y)]++;
					}
				}
			}
			for (map<int, int>::iterator it = buf.begin(); it != buf.end(); it++){
				if (maxCount < it->second){
					maxCount = it->second;
					maxCode = it->first;
				}
			}
			for (vector<float>::iterator it = feaReg[maxCode].cnv.begin();
				it != feaReg[maxCode].cnv.end(); it++, *fit++){
				*fit = *it * maxCount;
			}buf.clear();
		}
	}
}
void quick_sort(vector<vecColorInfo> &vec, int left, int right){
	if (left < right){
		int s = vec[(left + right) / 2].count;
		int i = left - 1;
		int j = right + 1;
		while (true){
			while (vec[++i].count < s);
			while (vec[--j].count > s);
			if (i >= j){
				break;
			}
			std::swap(vec[i], vec[j]);
		}
		quick_sort(vec, left, i - 1);
		quick_sort(vec, j + 1, right);
	}
}