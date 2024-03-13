#ifndef __LEVEL1__
#define __LEVEL1__
#include <string>
#include <opencv2/opencv.hpp>
using namespace cv;

void level1(Mat srcImg);
Mat addGuassNoise(Mat srcImg);
Mat meanFilter(Mat srcImg);
Mat perspectiveTransform1(Mat srcImg);
Mat perspectiveTransform2(Mat srcImg);
Mat colorToGray(Mat srcImg);
Mat cannyEdgeDetect(Mat srcImg);

#endif