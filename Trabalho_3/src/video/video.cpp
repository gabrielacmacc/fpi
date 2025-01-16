#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include "../base/base.h"
#include "../files/files.h"
#include "../utils/constants.cpp"

using namespace cv;
using namespace std;

int kernelSize = 0;
int brightness = 0;
int contrast = 0;
int lowThresh = 0;
int highThresh = 0;

bool alreadyCreatedKernel = false;
bool alreadyCreatedLight = false;
bool alreadyCreatedContrast = false;
bool alreadyCreatedLowThresh = false;
bool alreadyCreatedHighThresh = false;

bool shouldRecord = false;

int applyOperation = -1;

void onTrackbarKernel(int pos, void* userdata) {
    if (pos % 2 != 0) {
        kernelSize = pos;        
    }
}

void onTrackbarBrightness(int pos, void* userdata) {
    if (pos % 2 != 0) {
        brightness = pos;        
    }
}

void onTrackbarContrast(int pos, void* userdata) {
    if (pos % 2 != 0) {
        contrast = pos;        
    }
}

void onTrackbarLowThresh(int pos, void* userdata) {
    if (pos % 2 != 0) {
        lowThresh = pos;        
    }
}

void onTrackbarHighThresh(int pos, void* userdata) {
    if (pos % 2 != 0) {
        highThresh = pos;        
    }
}

Mat applyGaussianToVideo(const Mat &frame, bool &shouldShowBar) {
    Mat editedFrame;
    if (shouldShowBar && !alreadyCreatedKernel) {
        createTrackbar(KERNEL_TRACKBAR, EDITED_VIDEO_TITLE, NULL, 20, onTrackbarKernel);
        alreadyCreatedKernel = true;
    }
    if (kernelSize != 0) {
        GaussianBlur(frame, editedFrame, Size(kernelSize, kernelSize), 0);
    } else {
        editedFrame = frame.clone();
    }
    shouldShowBar = false;

    return editedFrame;
}

Mat applyCannyToVideo(const Mat &frame, bool &shouldShowBar) {
    Mat editedFrame;

    if (shouldShowBar && !alreadyCreatedLowThresh && !alreadyCreatedHighThresh) {
        createTrackbar(LOW_THRESH_TRACKBAR, EDITED_VIDEO_TITLE, NULL, 200, onTrackbarLowThresh);
        createTrackbar(HIGH_THRESH_TRACKBAR, EDITED_VIDEO_TITLE, NULL, 200, onTrackbarHighThresh);
        alreadyCreatedLowThresh = true;
        alreadyCreatedHighThresh = true;
    }
    Canny(frame, editedFrame, lowThresh, highThresh);

    return editedFrame;
}

Mat applySobelToVideo(const Mat &frame) {
    Mat editedFrame;
    Mat x_src;
    Mat y_src;
    Mat x_dst;
    Mat y_dst;

    Sobel(frame, x_src, 3, 1, 0, 3, 1, 0, 4);
    Sobel(frame, y_src, 3, 0, 1, 3, 1, 0, 4);
    convertScaleAbs(x_src, x_dst);
    convertScaleAbs(y_src, y_dst);
    addWeighted(x_dst, 0.5, y_dst, 0.5, 0, editedFrame);

    return editedFrame;
}

Mat applyBrightnessOrContrastToVideo(const Mat &frame, bool &shouldShowBar) {
    Mat editedFrame;
    if (shouldShowBar && !alreadyCreatedLight && !alreadyCreatedContrast) {
        createTrackbar(BRIGHTNESS_TRACKBAR, EDITED_VIDEO_TITLE, NULL, 255, onTrackbarBrightness);
        setTrackbarMin(BRIGHTNESS_TRACKBAR, EDITED_VIDEO_TITLE, -255);
        createTrackbar(CONTRAST_TRACKBAR, EDITED_VIDEO_TITLE, NULL, 255, onTrackbarContrast);
        setTrackbarMin(CONTRAST_TRACKBAR, EDITED_VIDEO_TITLE, 1);
        alreadyCreatedLight = true;
        alreadyCreatedContrast = true;
    }
    if (brightness <= 255 && brightness >= -255 && contrast <= 255 && contrast > 0) {
        addWeighted(frame, contrast, frame, 0, brightness, editedFrame);
    } else {
        editedFrame = frame.clone();
    }
    shouldShowBar = false;
    
    return editedFrame;
}

Mat getEditedFrame(const Mat &frame, int &applyOperation, bool &shouldShowBar) {
    Mat editedFrame;

    if (applyOperation == 0) {
        editedFrame = applyGaussianToVideo(frame, shouldShowBar);
    } else if (applyOperation == 1) {
        editedFrame = applyCannyToVideo(frame, shouldShowBar);
    } else if (applyOperation == 2) {
        editedFrame = applySobelToVideo(frame);
    } else if (applyOperation == 3) {
        editedFrame = applyBrightnessOrContrastToVideo(frame, shouldShowBar);
    } else if (applyOperation == 4) {
        addWeighted(frame, -1, frame, 0, 255, editedFrame);
    } else if (applyOperation == 5) {
        cvtColor(frame, editedFrame, 6);
    } else if (applyOperation == 6) {
        rotate(frame, editedFrame, ROTATE_90_CLOCKWISE);
    } else if (applyOperation == 7) {
        resize(frame, editedFrame, Size(frame.cols / 2, frame.rows / 2));
    } else if (applyOperation == 8) {
        flip(frame, editedFrame, 1);
    } else if (applyOperation == 9) {
        flip(frame, editedFrame, 0);
    } else {
        editedFrame = frame.clone();
    }

    return editedFrame;
}

int startVideo() {
    int camera = 0; 
    VideoCapture cap; 
    if(!cap.open(camera)) { 
     	return 0;
	}
			
	Mat editedFrame;
	bool shouldShowBar = false;

	int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
   	int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    Size frame_size(frame_width, frame_height);
    int fps = 30;
	VideoWriter video("output.mp4", VideoWriter::fourcc('D', 'I', 'V', 'X'), fps, frame_size);

	for(;;) {  
	    Mat frame;
        cap >> frame;
        if(frame.empty()) {
			break;
		}
			
	    editedFrame = getEditedFrame(frame, applyOperation, shouldShowBar);

	    cv::imshow(ORIGINAL_VIDEO_TITLE, frame);
	    cv::imshow(EDITED_VIDEO_TITLE, editedFrame);

	    if (shouldRecord) {
		    video.write(editedFrame);
	    }

	    char key = waitKey(1);
        if (key == 48) {
    	    applyOperation = 0;
	        shouldShowBar = true;
        } else if (key == 49) {
		    applyOperation = 1;
		    shouldShowBar = true;
	    } else if (key == 50) {
		    applyOperation = 2;
	    } else if (key == 51) {
		    applyOperation = 3;
		    shouldShowBar = true;
	    } else if (key == 52) {
		    applyOperation = 4;
	    } else if (key == 53) {
		    applyOperation = 5;
	    } else if (key == 54) {
		    applyOperation = 6;
	    } else if (key == 55) {
		    applyOperation = 7;
	    } else if (key == 56) {
		    applyOperation = 8;
	    } else if (key == 57) {
		    applyOperation = 9;
	    } else if (key == 114) {
		    cout << VIDEO_RECORDING_LOG << endl;
		    shouldRecord = true;
	    } else if (key == 27) {
    	    break;
        }
    }

    cap.release();
    video.release();
	return 0;
}