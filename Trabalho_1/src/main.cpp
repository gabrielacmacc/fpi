#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include "imageProcessing/imageProcessing.h"
#include "filesOperations/filesOperations.h"

#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define WINDOW_NAME "INF01046 - Gabriela Copetti Maccagnan - 288598"
#define WINDOW_WIDTH 280
#define WINDOW_HEIGHT 720

#define GRAMADO "Gramado"
#define EXT_GRAMADO "_72k.jpg"

#define SPACE "Space"
#define EXT_SPACE "_187k.jpg"

#define UNDERWATER "Underwater"
#define EXT_UNDERWATER "_53k.jpg"

#define PATH_ASSETS "\\assets\\"
#define DEFAULT_NAME "AlteredImage.jpeg"
#define STATE_DEFAULT "default"
#define STATE_GS "grayscale"

using namespace cv;
using namespace std;

int main(int argc, const char *argv[])
{
	Mat frame = Mat(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);

	cvui::init(WINDOW_NAME);

	string rootDir = getRootPath() + PATH_ASSETS;

	Mat img;
	Mat imgCopy;

	int n = 0;

	string img_state = STATE_DEFAULT;	

	while (true) {
		frame = cv::Scalar(49, 52, 49);

		// Render UI components to the frame
		cvui::text(frame, 20, 20, "Actions Menu");

		if (cvui::button(frame, 20, 50, 100, 30, "Open Image")) {
			string filePath = openFileDialog();
			readImage(img, filePath);
			clearImage(imgCopy);
	    }

		cvui::text(frame, 20, 100, "Use Default Images");

		if (cvui::button(frame, 40, 130, 100, 30, GRAMADO)) {
			readImage(img, rootDir + GRAMADO + EXT_GRAMADO);
			clearImage(imgCopy);
		}

		if (cvui::button(frame, 40, 170, 100, 30, SPACE)) {
			readImage(img, rootDir + SPACE + EXT_SPACE);
			clearImage(imgCopy);
		}

		if (cvui::button(frame, 40, 210, 100, 30, UNDERWATER)) {
			readImage(img, rootDir + UNDERWATER + EXT_UNDERWATER);
			clearImage(imgCopy);
		}

		cvui::text(frame, 20, 270, "Mirror");

		if (cvui::button(frame, 40, 300, 100, 30, "Vertical")) {
			if (!imgCopy.empty()) {
				Mat mirrored = mirrorVertical(imgCopy);
				copyImage(mirrored, imgCopy);
				showImage(imgCopy);
			}
		}

		if (cvui::button(frame, 40, 340, 100, 30, "Horizontal")) {
			if (!imgCopy.empty()) {
				Mat mirrored = mirrorHorizontal(imgCopy);
				copyImage(mirrored, imgCopy);
				showImage(imgCopy);
			}
		}

		cvui::text(frame, 20, 400, "Filters");

		if (cvui::button(frame, 40, 430, 100, 30, "Grayscale")) {
			if (!imgCopy.empty()) {
				Mat filtered = convertToGS(imgCopy);
				copyImage(filtered, imgCopy);
				showImage(imgCopy);
				img_state = STATE_GS;
			}
		}

		cvui::counter(frame, 40, 473, &n);

		if (cvui::button(frame, 150, 470, 100, 30, "Quantization")) {
			if (!imgCopy.empty() && n > 0 && img_state == STATE_GS) {
				Mat filtered = quantization(imgCopy, n);
				copyImage(filtered, imgCopy);
				showImage(imgCopy);
				n = 0;
			}
		}		

		if (cvui::button(frame, 20, 520, 100, 30, "Copy Image")) {
			copyImage(img, imgCopy);
            showImage(imgCopy);
			img_state = STATE_DEFAULT;
		}

		if (cvui::button(frame, 20, 560, 100, 30, "Save Image")) {
			std::string savePath = saveFileDialog(DEFAULT_NAME);
            if (!savePath.empty() && !imgCopy.empty()) {
                saveImage(savePath, imgCopy);
            }
		}

		// Update cvui stuff and show everything on the screen
		cvui::imshow(WINDOW_NAME, frame);

		if (cv::waitKey(20) == 27) {
			break;
		}
	}

	return 0;
}