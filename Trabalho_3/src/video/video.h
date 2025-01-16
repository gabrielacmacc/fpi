#include <opencv2/opencv.hpp>
#include "../utils/constants.cpp"

using namespace cv;
using namespace std;

#ifndef video_h
#define video_h

Mat getEditedFrame(const Mat& frame, int &applyOperation, bool &shouldShowBar);
int startVideo();

#endif