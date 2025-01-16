#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

void showImage(const Mat& img, const string& title = "Copied Image") {
    if (!img.empty()) {
        imshow(title, img);
    } else {
        cerr << "Error: Cannot open image" << endl;
    }
}

void readImage(Mat &img, const string& imagePath) {
    img = imread(imagePath);
    showImage(img, "Original Image");
}

void copyImage(Mat &original, Mat &copy) {
	copy = original.clone();
}

void saveImage(string filePath, Mat& img) {
    imwrite(filePath, img);
    cout << "Image saved at: " << filePath << endl;
}

void clearImage(Mat& img) {
    img = Mat::zeros(img.size(), img.type());
}

Mat mirrorVertical(const Mat& img) {
	Mat mirroredImage(img.size(), img.type());

    for (int y = 0; y < img.rows; y++) {
        const uchar* oldRow = img.ptr<uchar>(y);
        uchar* newRow = mirroredImage.ptr<uchar>(img.rows - 1 - y);

        memcpy(newRow, oldRow, img.cols * img.channels());
    }
	
    return mirroredImage;
}

Mat mirrorHorizontal(const Mat& img) {
    Mat mirroredImage(img.size(), img.type());

    for (int y = 0; y < img.rows; y++) {
        const uchar* oldRow = img.ptr<uchar>(y);
        uchar* newRow = mirroredImage.ptr<uchar>(y);

        for (int x = 0; x < img.cols; x++) {
            memcpy(&newRow[(img.cols - 1 - x) * img.channels()], &oldRow[x * img.channels()], img.channels());
        }
    }

    return mirroredImage;
}

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