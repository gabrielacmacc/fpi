#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#ifndef imageProceesing_h
#define imageProceesing_h

void showImage(const Mat& img, const string& title = "Copied Image");
void readImage(Mat &img, const string& imagePath);
void copyImage(Mat &original, Mat &copy);
void clearImage(Mat& img);
void saveImage(string filePath, Mat& img);
cv::Mat mirrorVertical(const Mat& img);
cv::Mat mirrorHorizontal(const Mat& img);
cv::Mat convertToGS(const Mat& img);
cv::Mat quantization(const Mat& img, const int& n);

#endif