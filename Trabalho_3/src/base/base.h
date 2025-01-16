#include <opencv2/opencv.hpp>
#include "../utils/constants.cpp"

using namespace cv;
using namespace std;

#ifndef base_h
#define base_h

void showImage(const Mat& img, const string& title = COPY_IMAGE_TITLE);
void readImage(Mat &img, const string& imagePath, const string& title = ORIGINAL_IMAGE_TITLE);
void copyImage(Mat &original, Mat &copy);
void clearImage(Mat& img);
void saveImage(string filePath, Mat& img);

#endif