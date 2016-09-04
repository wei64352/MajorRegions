#include"GaussianBlur.h"

gaussian::gaussian(int size, int standred, Mat &image){
	cSize = size;
	cStandred = standred;
	cMask.resize(cSize * cSize);
	//imshow("image", image);
	init_mask();
	Gaussian_Blur(image);
	//imshow("new image", image); waitKey(0);
}
void gaussian::init_mask(){
	int index = 0, center = cSize / 2;
	double u, v, standred = pow(cStandred, 2), e;
	for (int i = 0; i < cSize; i++){
		for (int j = 0; j < cSize; j++){
			u = pow(center - i, 2);
			v = pow(center - j, 2);
			e = pow(M_E, -(u + v) / (2 * standred));
			cMask[index] =
				(1 / (2 * M_PI * standred)) * e;
			index++;
		}
	}
}
void gaussian::Gaussian_Blur(Mat &image){
	Mat smoImg = Mat(image.size(), image.type());
	int index;
	int start = cSize / 2;
	Vec3d color;

	for (int i = start; i < image.rows - start; i++){
		for (int j = start; j < image.cols - start; j++){
			index = 0;
			color = { 0, 0, 0 };
			for (int x = i - start; x <= i + start; x++){
				for (int y = j - start; y <= j + start; y++){
					color += image.at<Vec3b>(x, y) * cMask[index];
					index++;
				}
			}
			smoImg.at<Vec3b>(i, j) = color;
		}
	}image = smoImg;
}