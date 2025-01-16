#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "../utils/constants.cpp"

using namespace cv;
using namespace std;

void showImage(const Mat& img, const string& title = COPY_IMAGE_TITLE) {
    if (!img.empty()) {
        imshow(title, img);
    } else {
        cerr << CANNOT_OPEN_ERROR << endl;
    }
}

void readImage(Mat &img, const string& imagePath, const string& title = ORIGINAL_IMAGE_TITLE) {
    img = imread(imagePath);
    showImage(img, title);
}

void copyImage(Mat &original, Mat &copy) {
	copy = original.clone();
}

void saveImage(string filePath, Mat& img) {
    imwrite(filePath, img);
    cout << IMAGE_SAVED_LOG << filePath << endl;
}

void clearImage(Mat& img) {
    img = Mat::zeros(img.size(), img.type());
}
