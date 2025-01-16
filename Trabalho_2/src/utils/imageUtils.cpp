#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include "../basic/basic.h"
#include "constants.cpp"

using namespace cv;
using namespace std;

void checkCopyAndApply(Mat& imgCopy, Mat (*function)(const Mat&)) {
	if (!imgCopy.empty()) {
		Mat newImage = function(imgCopy);
		copyImage(newImage, imgCopy);
		showImage(imgCopy);
	} else {
		cout << NO_COPY_ERROR << endl;
	}
}