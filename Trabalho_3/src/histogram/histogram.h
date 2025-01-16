#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#ifndef histogram_h
#define histogram_h

Mat makeHistogramImage(vector<int> histogram);
vector<int> makeHistogram(const Mat& img);
Mat equalize(const Mat& img);
Mat equalizeColor(const Mat &img);
Mat histogramMatching(const Mat &img_src, const Mat &img_target);

#endif