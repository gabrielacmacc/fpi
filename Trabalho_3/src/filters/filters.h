#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#ifndef filters_h
#define filters_h

Mat convertToGS(const Mat& img);
Mat quantization(const Mat& img, const int& n);
Mat convertToNegative(const Mat& img);
Mat adjustLight(const Mat& img, const int& bn);
Mat adjustContrast(const Mat& img, const int& cn);

#endif