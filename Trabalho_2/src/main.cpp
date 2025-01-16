#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include "basic/basic.h"
#include "convolution/convolution.h"
#include "files/files.h"
#include "filters/filters.h"
#include "utils/constants.cpp"
#include "utils/imageUtils.cpp"

#define CVUI_IMPLEMENTATION
#include "cvui.h"

using namespace cv;
using namespace std;
using namespace cvui;

int main(int argc, const char *argv[])
{
	Mat frame = Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);

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

	while (true) {
		frame = Scalar(49, 52, 49);
		
		text(frame, 20, 20, MAIN_MENU_TITLE);

		if (button(frame, 20, 50, BUTTON_WIDTH, BUTTON_HEIGHT, OPEN_BUTTON)) {
			string filePath = openFileDialog();
			readImage(img, filePath);
			clearImage(imgCopy);
	    }

		text(frame, 20, 100, USE_DEFAULT_IMAGES_TITLE);

		if (button(frame, 40, BUTTON_WIDTH, BUTTON_WIDTH, BUTTON_HEIGHT, GRAMADO)) {
			readImage(img, rootDir + GRAMADO_PATH);
			clearImage(imgCopy);
		}

		if (button(frame, 40, 170, BUTTON_WIDTH, BUTTON_HEIGHT, SPACE)) {
			readImage(img, rootDir + SPACE_PATH);
			clearImage(imgCopy);
		}

		if (button(frame, 40, 210, BUTTON_WIDTH, BUTTON_HEIGHT, UNDERWATER)) {
			readImage(img, rootDir + UNDERWATER_PATH);
			clearImage(imgCopy);
		}

		if (button(frame, 20, 260, BUTTON_WIDTH, BUTTON_HEIGHT, COPY_BUTTON)) {
			copyImage(img, imgCopy);
            showImage(imgCopy);
		}

		if (button(frame, 20, 300, BUTTON_WIDTH, BUTTON_HEIGHT, SAVE_BUTTON)) {
			string savePath = saveFileDialog(DEFAULT_NAME);
            if (!savePath.empty() && !imgCopy.empty()) {
                saveImage(savePath, imgCopy);
            } else {
				cout << NO_COPY_ERROR << endl;
			}
		}

		text(frame, 20, 350, OPERATIONS_MENU_TITLE);

		text(frame, 40, 380, MIRROR_MENU_TITLE);

		if (button(frame, 60, 400, BUTTON_WIDTH, BUTTON_HEIGHT, VERTICAL_BUTTON)) {
			checkCopyAndApply(imgCopy, &mirrorVertical);
		}

		if (button(frame, 60, 440, BUTTON_WIDTH, BUTTON_HEIGHT, HORIZONTAL_BUTTON)) {
			checkCopyAndApply(imgCopy, &mirrorHorizontal);
		}

		text(frame, 40, 480, ZOOM_MENU_TITLE);

		if (button(frame, 60, 500, BUTTON_WIDTH, BUTTON_HEIGHT, ZOOM_IN_BUTTON)) {
			checkCopyAndApply(imgCopy, &zoomIn);
		}

		counter(frame, 210, 540, &sx);
		counter(frame, 210, 570, &sy);

		if (button(frame, 60, 540, BUTTON_WIDTH, BUTTON_HEIGHT, ZOOM_OUT_BUTTON)) {
			if (!imgCopy.empty()) {
				if (sx >= 1 && sy >= 1) {
					Mat zoomedImage = zoomOut(imgCopy, sx, sy);
					copyImage(zoomedImage, imgCopy);
					showImage(imgCopy);
				} else {
					cout << OUT_OF_RANGE_ERROR << endl;
				}
			} else {
        		cout << NO_COPY_ERROR << endl;
			}
		}

		text(frame, 40, 620, ADJUSTMENTS_MENU_TITLE);

		counter(frame, 210, 640, &bn);

		if (button(frame, 60, 640, BUTTON_WIDTH, BUTTON_HEIGHT, BRIGHTNESS_BUTTON)) {
			if (!imgCopy.empty()) {
				if (bn <= 255 && bn >= -255) {
					Mat ajustedImg = adjustLight(imgCopy, bn);
					copyImage(ajustedImg, imgCopy);
					showImage(imgCopy);
				} else {
        			cout << OUT_OF_RANGE_ERROR << endl;
				}
			} else {
        		cout << NO_COPY_ERROR << endl;
			}
		}

		counter(frame, 210, 680, &cn);

		if (button(frame, 60, 680, BUTTON_WIDTH, BUTTON_HEIGHT, CONTRAST_BUTTON)) {
			if (!imgCopy.empty()) {
				if (cn <= 255 && cn > 0) {
					Mat ajustedImg = adjustContrast(imgCopy, cn);
					copyImage(ajustedImg, imgCopy);
					showImage(imgCopy);
				} else {
        			cout << OUT_OF_RANGE_ERROR << endl;
				}
			} else {
        		cout << NO_COPY_ERROR << endl;
			}
		}

		text(frame, 320, 380, ROTATIONS_MENU_TITLE);

		if (button(frame, 340, 400, BUTTON_WIDTH, BUTTON_HEIGHT, CLOCKWISE_BUTTON)) {
			if (!imgCopy.empty()) {
				Mat rotatedImage = rotateImage(imgCopy, true);
				copyImage(rotatedImage, imgCopy);
				showImage(imgCopy);
			} else {
        		cout << NO_COPY_ERROR << endl;
			}
		}

		if (button(frame, 340, 440, BUTTON_WIDTH, BUTTON_HEIGHT, COUNTERCLOCKWISE_BUTTON)) {
			if (!imgCopy.empty()) {
				Mat rotatedImage = rotateImage(imgCopy, false);
				copyImage(rotatedImage, imgCopy);
				showImage(imgCopy);
			} else {
        		cout << NO_COPY_ERROR << endl;
			}
		}

		text(frame, 320, 480, FILTERS_MENU_TITLE);

		if (button(frame, 340, 500, BUTTON_WIDTH, BUTTON_HEIGHT, GRAYSCALE_BUTTON)) {
			checkCopyAndApply(imgCopy, &convertToGS);
		}
		
		if (button(frame, 340, 540, BUTTON_WIDTH, BUTTON_HEIGHT, NEGATIVE_BUTTON)) {
			checkCopyAndApply(imgCopy, &convertToNegative);
		}

		counter(frame, 490, 580, &n);

		if (button(frame, 340, 580, BUTTON_WIDTH, BUTTON_HEIGHT, QUANTIZATION_BUTTON)) {
			if (!imgCopy.empty() && n > 0) {
				if (imgCopy.channels() != 3) {
					Mat filtered = quantization(imgCopy, n);
					copyImage(filtered, imgCopy);
					showImage(imgCopy);
				} else {
					cout << GRAYSCALE_ERROR << endl;
				}
			} else {
				cout << NO_COPY_ERROR << endl;
			}
		}

		text(frame, 600, 380, CONVOLUTIONS_MENU_TITLE);
		
		counter(frame, 620, 400, &a, CONVOLUTIONS_STEP);

		counter(frame, 620, 430, &d, CONVOLUTIONS_STEP);

		counter(frame, 620, 460, &g, CONVOLUTIONS_STEP);

		counter(frame, 730, 400, &b, CONVOLUTIONS_STEP);

		counter(frame, 730, 430, &e, CONVOLUTIONS_STEP);

		counter(frame, 730, 460, &h, CONVOLUTIONS_STEP);

		counter(frame, 840, 400, &c, CONVOLUTIONS_STEP);

		counter(frame, 840, 430, &f, CONVOLUTIONS_STEP);

		counter(frame, 840, 460, &i, CONVOLUTIONS_STEP);

		if (button(frame, 620, 500, 90, BUTTON_HEIGHT, APPLY_BUTTON)) {
			float data[] = {a, b, c, d, e, f, g, h, i};
			if (!imgCopy.empty()) {
				Mat convImage = applyConvolution(imgCopy, data, true);
				copyImage(convImage, imgCopy);
				showImage(imgCopy);
			} else {
				cout << NO_COPY_ERROR << endl;
			}
		}

		text(frame, 600, 580, USE_DEFAULT_KERNELS_TITLE);

		if (button(frame, 620, 600, BUTTON_WIDTH, BUTTON_HEIGHT, PREWITTX_BUTTON)) {
			checkCopyAndApply(imgCopy, &applyPrewittHx);
		}

		if (button(frame, 620, 640, BUTTON_WIDTH, BUTTON_HEIGHT, SOBELX_BUTTON)) {
			checkCopyAndApply(imgCopy, &applySobelHx);
		}

		if (button(frame, 620, 680, BUTTON_WIDTH, BUTTON_HEIGHT, GAUSSIAN_BUTTON)) {
			checkCopyAndApply(imgCopy, &applyGaussian);
		}

		if (button(frame, 770, 600, BUTTON_WIDTH, BUTTON_HEIGHT, PREWITTY_BUTTON)) {
			checkCopyAndApply(imgCopy, &applyPrewittHy);
		}

		if (button(frame, 770, 640, BUTTON_WIDTH, BUTTON_HEIGHT, SOBELY_BUTTON)) {
			checkCopyAndApply(imgCopy, &applySobelHy);
		}

		if (button(frame, 770, 680, BUTTON_WIDTH, BUTTON_HEIGHT, LAPLACIAN_BUTTON)) {
			checkCopyAndApply(imgCopy, &applyLaplacian);
		}

		if (button(frame, 620, 720, BUTTON_WIDTH, BUTTON_HEIGHT, HP_BUTTON)) {
			checkCopyAndApply(imgCopy, &applyHighPass);
		}

		text(frame, 320, 620, HISTOGRAMS_MENU_TITLE);

		if (button(frame, 340, 640, BUTTON_WIDTH, BUTTON_HEIGHT, HISTOGRAM_BUTTON)) {
			if (!imgCopy.empty()) {
				vector<int> histogram = makeHistogram(imgCopy);
				Mat histogramImage = makeHistogramImage(histogram); 
				copyImage(histogramImage, graph);
				showImage(graph, HISTOGRAM_BUTTON);
			} else {
				cout << NO_COPY_ERROR << endl;
			}
		}

		if (button(frame, 340, 680, BUTTON_WIDTH, BUTTON_HEIGHT, EQUALIZATION_BUTTON)) {
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

		if (button(frame, 340, 720, BUTTON_WIDTH, BUTTON_HEIGHT, MATCHING_BUTTON)) {
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

		cvui::imshow(WINDOW_NAME, frame);

		if (waitKey(20) == 27) {
			break;
		}
	}

	return 0;
}