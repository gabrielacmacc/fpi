#include <opencv2/opencv.hpp>
#include "../utils/constants.cpp"

using namespace cv;
using namespace std;

#ifndef transform_h
#define transform_h

Mat mirrorVertical(const Mat& img);
Mat mirrorHorizontal(const Mat& img);
Mat zoomOut(const Mat& img, int sx, int sy);
Mat zoomIn(const Mat& img);
Mat rotateImage(const Mat& img, bool clockwise);

#endif