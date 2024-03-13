#ifndef __LEVEL2__
#define __LEVEL2__
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Point FindPoints(Mat img);
Mat ImageProcessFun(Mat img_rgb);
Point level2(Mat img);

#endif