#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "../utils/constants.cpp"

using namespace cv;
using namespace std;

Mat convertToGS(const Mat& img) {
	Mat grayImage(img.size(), CV_8UC1);

    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            Vec3b pixel = img.at<Vec3b>(y, x);
            uchar R = pixel[2];
            uchar G = pixel[1];
            uchar B = pixel[0];

            uchar L = 0.299 * R + 0.587 * G + 0.114 * B;

            grayImage.at<uchar>(y, x) = L;
        }
    }

	return grayImage;
}

Mat quantization(const Mat& img, const int& n) {
	if (img.channels() == 3) {
		cout << GRAYSCALE_ERROR << endl;
		return img;
	}

	Mat quantizedImage(img.size(), img.type());
    
    uchar t1 = 255;
    uchar t2 = 0;

    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            uchar pixel = img.at<uchar>(y, x);
            if (pixel < t1) {
				t1 = pixel;
			}
            if (pixel > t2) {
				t2 = pixel;
			}
        }
    }

    int tam_int = t2 - t1 + 1;

    if (n >= tam_int) {
        return img;
    }

    float tb = static_cast<float>(tam_int) / n;

    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            uchar pixel = img.at<uchar>(y, x);

            int binIndex = static_cast<int>((pixel - t1 + 0.5) / tb);

            int quantizedPixel = static_cast<int>(t1 + (binIndex * tb) + (tb / 2));

            quantizedPixel = min(max(quantizedPixel, 0), 255);

            quantizedImage.at<uchar>(y, x) = static_cast<uchar>(quantizedPixel);
        }
    }

    return quantizedImage;
}

int verifyRange(int pixel) {
	return pixel > 255 ? 255 : (pixel < 0 ? 0 : pixel);
}

Mat adjustLight(const Mat& img, const int& bn) {
	if (bn > 255 || bn < -255) {
		cout << OUT_OF_RANGE_ERROR << endl;
		return img;
	}

	Mat ajustedImage(img.size(), img.type());

	if (img.channels() != 3) {
		for (int y = 0; y < img.rows; y++) {
        	for (int x = 0; x < img.cols; x++) {
            	int pixel = img.at<uchar>(y, x) + bn;
				int newValue = verifyRange(pixel);
            	ajustedImage.at<uchar>(y, x) = newValue;
        	}
		}
	} else {
		for (int y = 0; y < img.rows; y++) {
        	for (int x = 0; x < img.cols; x++) {
            	Vec3b pixel = img.at<Vec3b>(y, x);
				for (int c = 0; c < img.channels(); c++) {
            		ajustedImage.at<Vec3b>(y, x)[c] = verifyRange(pixel[c] + bn);
				}
        	}
		}
	}

	return ajustedImage;
}

Mat adjustContrast(const Mat& img, const int& cn) {
	if (cn > 255 || cn <= 0) {
		cout << OUT_OF_RANGE_ERROR << endl;
		return img;
	}

	Mat ajustedImage(img.size(), img.type());

	if (img.channels() != 3) {
		for (int y = 0; y < img.rows; y++) {
        	for (int x = 0; x < img.cols; x++) {
            	int pixel = img.at<uchar>(y, x) * cn;
				int newValue = verifyRange(pixel);
            	ajustedImage.at<uchar>(y, x) = newValue;
        	}
		}
	} else {
		for (int y = 0; y < img.rows; y++) {
        	for (int x = 0; x < img.cols; x++) {
            	Vec3b pixel = img.at<Vec3b>(y, x);
				for (int c = 0; c < img.channels(); c++) {
            		ajustedImage.at<Vec3b>(y, x)[c] = verifyRange(pixel[c] * cn); 
				}
        	}
		}
	}

	return ajustedImage;
}

Mat convertToNegative(const Mat& img) {
	Mat negativeImage(img.size(), img.type());

	if (img.channels() != 3) {
		for (int y = 0; y < img.rows; y++) {
        	for (int x = 0; x < img.cols; x++) {
            	int pixelHue = img.at<uchar>(y, x);
            	negativeImage.at<uchar>(y, x) = 255 - pixelHue;
        	}
		}
	} else {
		for (int y = 0; y < img.rows; y++) {
        	for (int x = 0; x < img.cols; x++) {
            	Vec3b pixel = img.at<Vec3b>(y, x);
				for (int c = 0; c < img.channels(); c++) {
            		negativeImage.at<Vec3b>(y, x)[c] = 255 - pixel[c];
				}
        	}
		}
	}

	return negativeImage;
}
