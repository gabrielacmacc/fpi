#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#ifndef filters_h
#define filters_h

cv::Mat convertToGS(const Mat& img);
cv::Mat quantization(const Mat& img, const int& n);
Mat makeHistogramImage(vector<int> histogram);
vector<int> makeHistogram(const Mat& img);
Mat equalize(const Mat& img);
Mat equalizeColor(const Mat &img);
Mat histogramMatching(const Mat &img_src, const Mat &img_target);
Mat convertToNegative(const Mat& img);
Mat adjustLight(const Mat& img, const int& bn);
Mat adjustContrast(const Mat& img, const int& cn);

#endif