#include <opencv2/opencv.hpp>
#include "../utils/constants.cpp"

using namespace cv;
using namespace std;

#ifndef basic_h
#define basic_h

void showImage(const Mat& img, const string& title = COPY_IMAGE_TITLE);
void readImage(Mat &img, const string& imagePath, const string& title = ORIGINAL_IMAGE_TITLE);
void copyImage(Mat &original, Mat &copy);
void clearImage(Mat& img);
void saveImage(string filePath, Mat& img);
Mat mirrorVertical(const Mat& img);
Mat mirrorHorizontal(const Mat& img);
Mat zoomOut(const Mat& img, int sx, int sy);
Mat zoomIn(const Mat& img);
Mat rotateImage(const Mat& img, boolean clockwise);

#endif