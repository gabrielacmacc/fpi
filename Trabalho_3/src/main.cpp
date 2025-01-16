#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include "base/base.h"
#include "convolution/convolution.h"
#include "files/files.h"
#include "filters/filters.h"
#include "histogram/histogram.h"
#include "transform/transform.h"
#include "utils/constants.cpp"
#include "utils/imageUtils.cpp"
#include "video/video.h"

#define CVUI_IMPLEMENTATION
#include "cvui.h"

using namespace cv;
using namespace std;
using namespace cvui;

int main(int argc, const char *argv[])
{	
	Mat window = Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);

	cvui::init(WINDOW_NAME);

	string rootDir = getRootPath() + PATH_ASSETS;

	Mat img;
	Mat imgCopy;
	Mat graph;
	Mat graph2;
	Mat matchingImg;

	int n = 0, bn = 0, cn = 0;

	int sx = 0, sy = 0;

	double a = 0.0, b = 0.0, c = 0.0, d = 0.0, e = 0.0, f = 0.0, g = 0.0, h = 0.0, i = 0.0;

	bool shouldStartVideo = false;

	while (true) {
		window = Scalar(49, 52, 49);

		text(window, 20, 20, VIDEO_MENU_TITLE);

		text(window, 20, 50, MAIN_MENU_TITLE);

		if (button(window, 20, 70, BUTTON_WIDTH, BUTTON_HEIGHT, VIDEO_START)) {
			shouldStartVideo = true;
		}

		text(window, 20, 110, VIDEO_MENU_GAUSSIAN);
		text(window, 20, 130, VIDEO_MENU_CANNY);
		text(window, 20, 150, VIDEO_MENU_SOBEL);
		text(window, 20, 170, VIDEO_MENU_LIGHT_CONTRAST);
		text(window, 20, 190, VIDEO_MENU_NEGATIVE);
		text(window, 20, 210, VIDEO_MENU_GRAYSCALE);
		text(window, 20, 230, VIDEO_MENU_ROTATE);
		text(window, 20, 250, VIDEO_MENU_RESIZE);
		text(window, 20, 270, VIDEO_MENU_MIRROR_H);
		text(window, 20, 290, VIDEO_MENU_MIRROR_V);
		text(window, 20, 310, VIDEO_MENU_RECORD);

		if (shouldStartVideo) {
			return startVideo();
		}
		
		text(window, 20, 380, MAIN_MENU_TITLE);

		if (button(window, 20, 400, BUTTON_WIDTH, BUTTON_HEIGHT, OPEN_BUTTON)) {
			string filePath = openFileDialog();
			readImage(img, filePath);
			clearImage(imgCopy);
	    }

		text(window, 20, 480, USE_DEFAULT_IMAGES_TITLE);

		if (button(window, 40, 500, BUTTON_WIDTH, BUTTON_HEIGHT, GRAMADO)) {
			readImage(img, rootDir + GRAMADO_PATH);
			clearImage(imgCopy);
		}

		if (button(window, 40, 540, BUTTON_WIDTH, BUTTON_HEIGHT, SPACE)) {
			readImage(img, rootDir + SPACE_PATH);
			clearImage(imgCopy);
		}

		if (button(window, 40, 580, BUTTON_WIDTH, BUTTON_HEIGHT, UNDERWATER)) {
			readImage(img, rootDir + UNDERWATER_PATH);
			clearImage(imgCopy);
		}

		if (button(window, 20, 640, BUTTON_WIDTH, BUTTON_HEIGHT, COPY_BUTTON)) {
			copyImage(img, imgCopy);
            showImage(imgCopy);
		}

		if (button(window, 20, 680, BUTTON_WIDTH, BUTTON_HEIGHT, SAVE_BUTTON)) {
			string savePath = saveFileDialog(DEFAULT_NAME);
            if (!savePath.empty() && !imgCopy.empty()) {
                saveImage(savePath, imgCopy);
            } else {
				cout << NO_COPY_ERROR << endl;
			}
		}

		text(window, 20, 350, OPERATIONS_MENU_TITLE);

		text(window, 220, 380, MIRROR_MENU_TITLE);

		if (button(window, 240, 400, BUTTON_WIDTH, BUTTON_HEIGHT, VERTICAL_BUTTON)) {
			checkCopyAndApply(imgCopy, &mirrorVertical);
		}

		if (button(window, 240, 440, BUTTON_WIDTH, BUTTON_HEIGHT, HORIZONTAL_BUTTON)) {
			checkCopyAndApply(imgCopy, &mirrorHorizontal);
		}

		text(window, 220, 480, ZOOM_MENU_TITLE);

		if (button(window, 240, 500, BUTTON_WIDTH, BUTTON_HEIGHT, ZOOM_IN_BUTTON)) {
			checkCopyAndApply(imgCopy, &zoomIn);
		}

		counter(window, 380, 540, &sx);
		counter(window, 380, 570, &sy);

		if (button(window, 240, 540, BUTTON_WIDTH, BUTTON_HEIGHT, ZOOM_OUT_BUTTON)) {
			checkCopyAndApply(imgCopy, sx, sy, &zoomOut);
		}

		text(window, 220, 620, ADJUSTMENTS_MENU_TITLE);

		counter(window, 380, 640, &bn);

		if (button(window, 240, 640, BUTTON_WIDTH, BUTTON_HEIGHT, BRIGHTNESS_BUTTON)) {
			checkCopyAndApply(imgCopy, bn, &adjustLight);
		}

		counter(window, 380, 680, &cn);

		if (button(window, 240, 680, BUTTON_WIDTH, BUTTON_HEIGHT, CONTRAST_BUTTON)) {
			checkCopyAndApply(imgCopy, cn, &adjustContrast);
		}

		text(window, 500, 380, ROTATIONS_MENU_TITLE);

		if (button(window, 520, 400, BUTTON_WIDTH, BUTTON_HEIGHT, CLOCKWISE_BUTTON)) {
			checkCopyAndApply(imgCopy, true, &rotateImage);
		}

		if (button(window, 520, 440, BUTTON_WIDTH, BUTTON_HEIGHT, COUNTERCLOCKWISE_BUTTON)) {
			checkCopyAndApply(imgCopy, false, &rotateImage);
		}

		text(window, 500, 480, FILTERS_MENU_TITLE);

		if (button(window, 520, 500, BUTTON_WIDTH, BUTTON_HEIGHT, GRAYSCALE_BUTTON)) {
			checkCopyAndApply(imgCopy, &convertToGS);
		}
		
		if (button(window, 520, 540, BUTTON_WIDTH, BUTTON_HEIGHT, NEGATIVE_BUTTON)) {
			checkCopyAndApply(imgCopy, &convertToNegative);
		}

		counter(window, 660, 580, &n);

		if (button(window, 520, 580, BUTTON_WIDTH, BUTTON_HEIGHT, QUANTIZATION_BUTTON)) {
			if (n > 0) {
				checkCopyAndApply(imgCopy, n, &quantization);
			}
		}

		text(window, 780, 380, CONVOLUTIONS_MENU_TITLE);
		
		counter(window, 800, 400, &a, CONVOLUTIONS_STEP);

		counter(window, 800, 430, &d, CONVOLUTIONS_STEP);

		counter(window, 800, 460, &g, CONVOLUTIONS_STEP);

		counter(window, 910, 400, &b, CONVOLUTIONS_STEP);

		counter(window, 910, 430, &e, CONVOLUTIONS_STEP);

		counter(window, 910, 460, &h, CONVOLUTIONS_STEP);

		counter(window, 1020, 400, &c, CONVOLUTIONS_STEP);

		counter(window, 1020, 430, &f, CONVOLUTIONS_STEP);

		counter(window, 1020, 460, &i, CONVOLUTIONS_STEP);

		if (button(window, 800, 500, 90, BUTTON_HEIGHT, APPLY_BUTTON)) {
			float data[] = {a, b, c, d, e, f, g, h, i};
			checkCopyAndApply(imgCopy, data, true, applyConvolution);
		}

		text(window, 780, 580, USE_DEFAULT_KERNELS_TITLE);

		if (button(window, 800, 600, BUTTON_WIDTH, BUTTON_HEIGHT, PREWITTX_BUTTON)) {
			checkCopyAndApply(imgCopy, &applyPrewittHx);
		}

		if (button(window, 800, 640, BUTTON_WIDTH, BUTTON_HEIGHT, SOBELX_BUTTON)) {
			checkCopyAndApply(imgCopy, &applySobelHx);
		}

		if (button(window, 800, 680, BUTTON_WIDTH, BUTTON_HEIGHT, GAUSSIAN_BUTTON)) {
			checkCopyAndApply(imgCopy, &applyGaussian);
		}

		if (button(window, 950, 600, BUTTON_WIDTH, BUTTON_HEIGHT, PREWITTY_BUTTON)) {
			checkCopyAndApply(imgCopy, &applyPrewittHy);
		}

		if (button(window, 950, 640, BUTTON_WIDTH, BUTTON_HEIGHT, SOBELY_BUTTON)) {
			checkCopyAndApply(imgCopy, &applySobelHy);
		}

		if (button(window, 950, 680, BUTTON_WIDTH, BUTTON_HEIGHT, LAPLACIAN_BUTTON)) {
			checkCopyAndApply(imgCopy, &applyLaplacian);
		}

		if (button(window, 800, 720, BUTTON_WIDTH, BUTTON_HEIGHT, HP_BUTTON)) {
			checkCopyAndApply(imgCopy, &applyHighPass);
		}

		text(window, 500, 620, HISTOGRAMS_MENU_TITLE);

		if (button(window, 520, 640, BUTTON_WIDTH, BUTTON_HEIGHT, HISTOGRAM_BUTTON)) {
			if (!imgCopy.empty()) {
				vector<int> histogram = makeHistogram(imgCopy);
				Mat histogramImage = makeHistogramImage(histogram); 
				copyImage(histogramImage, graph);
				showImage(graph, HISTOGRAM_BUTTON);
			} else {
				cout << NO_COPY_ERROR << endl;
			}
		}

		if (button(window, 520, 680, BUTTON_WIDTH, BUTTON_HEIGHT, EQUALIZATION_BUTTON)) {
			if (!imgCopy.empty()) {
				vector<int> histogram = makeHistogram(imgCopy);
				Mat histogramImage = makeHistogramImage(histogram); 
				copyImage(histogramImage, graph);
				showImage(graph, HISTOGRAM_IMAGE_TITLE);
				Mat equalizedImage = equalize(imgCopy);
				copyImage(equalizedImage, imgCopy);
				showImage(imgCopy);
				vector<int> equalizedHistogram = makeHistogram(equalizedImage);
				Mat equalizedHistogramImage = makeHistogramImage(equalizedHistogram);
				copyImage(equalizedHistogramImage, graph2);
				showImage(graph2, EQ_HISTOGRAM_IMAGE_TITLE);
			} else {
				cout << NO_COPY_ERROR << endl;
			}
		}

		if (button(window, 520, 720, BUTTON_WIDTH, BUTTON_HEIGHT, MATCHING_BUTTON)) {
			if (!imgCopy.empty()) {
				string filePath = openFileDialog();
				readImage(matchingImg, filePath, MATCHING_TARGET_IMAGE_TITLE);
				Mat matchedImage = histogramMatching(imgCopy, matchingImg);
				copyImage(matchedImage, imgCopy);
				showImage(imgCopy, MATCHING_IMAGE_TITLE);
			} else {
				cout << NO_COPY_ERROR << endl;
			}
		}

		cvui::imshow(WINDOW_NAME, window);

		if (waitKey(20) == 27) {
			break;
		}
	}

	return 0;
}
