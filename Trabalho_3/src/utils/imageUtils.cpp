#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include "../base/base.h"
#include "constants.cpp"

using namespace cv;
using namespace std;

void checkCopyAndApply(Mat& imgCopy, Mat (*function)(const Mat&)) {
	if (imgCopy.empty()) {
        cout << NO_COPY_ERROR << endl;
        return;
    }
	
	Mat newImage = function(imgCopy);
	copyImage(newImage, imgCopy);
	showImage(imgCopy);
}

void checkCopyAndApply(Mat& imgCopy, int p1, int p2, Mat (*function)(const Mat&, int, int)) {
	if (imgCopy.empty()) {
        cout << NO_COPY_ERROR << endl;
        return;
    }

	Mat newImage = function(imgCopy, p1, p2);
	copyImage(newImage, imgCopy);
	showImage(imgCopy);
}

void checkCopyAndApply(Mat& imgCopy, int p, Mat (*function)(const Mat&, const int&)) {
	if (imgCopy.empty()) {
        cout << NO_COPY_ERROR << endl;
        return;
    }

    Mat newImage = function(imgCopy, p);
    copyImage(newImage, imgCopy);
    showImage(imgCopy);
}

void checkCopyAndApply(Mat& imgCopy, bool param, Mat (*function)(const Mat&, bool)) {
	if (imgCopy.empty()) {
        cout << NO_COPY_ERROR << endl;
        return;
    }

	Mat newImage = function(imgCopy, param);
	copyImage(newImage, imgCopy);
	showImage(imgCopy);
}

void checkCopyAndApply(Mat& imgCopy, float data[], bool param, Mat (*function)(const Mat&, float*, bool)) {
	if (imgCopy.empty()) {
        cout << NO_COPY_ERROR << endl;
        return;
    }

	Mat newImage = function(imgCopy, data, param);
	copyImage(newImage, imgCopy);
	showImage(imgCopy);
}
