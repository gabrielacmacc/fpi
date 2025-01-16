#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "../utils/constants.cpp"

using namespace cv;
using namespace std;

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

Mat zoomOut(const Mat& img, int sx, int sy) {
    if (sx < 1 || sy < 1) {
        cout << OUT_OF_RANGE_ERROR << endl;
        return img;
    }

    Mat zoomedImage(img.rows / sy, img.cols / sx, img.type());
    int channels = img.channels();

    if (channels == 3) {
        for (int y = 0; y < zoomedImage.rows; y++) {
            for (int x = 0; x < zoomedImage.cols; x++) {
                int sumB = 0, sumG = 0, sumR = 0;
                int count = 0;

                for (int j = 0; j < sy; j++) {
                    for (int i = 0; i < sx; i++) {
                        int oldY = y * sy + j;
                        int oldX = x * sx + i;

                        if (oldY < img.rows && oldX < img.cols) {
                            Vec3b pixel = img.at<Vec3b>(oldY, oldX);
                            sumB += pixel[0];
                            sumG += pixel[1];
                            sumR += pixel[2];
                            count++;
                        }
                    }
                }

                zoomedImage.at<cv::Vec3b>(y, x) = cv::Vec3b(
                    static_cast<uchar>(sumB / count),
                    static_cast<uchar>(sumG / count),
                    static_cast<uchar>(sumR / count)
                );
            }
        }
    } else {
        for (int y = 0; y < zoomedImage.rows; y++) {
            for (int x = 0; x < zoomedImage.cols; x++) {
                int sum = 0;
                int count = 0;

                for (int j = 0; j < sy; j++) {
                    for (int i = 0; i < sx; i++) {
                        int oldY = y * sy + j;
                        int oldX = x * sx + i;

                        if (oldY < img.rows && oldX < img.cols) {
                            uchar pixel = img.at<uchar>(oldY, oldX);
                            sum += pixel;
                            count++;
                        }
                    }
                }

                zoomedImage.at<uchar>(y, x) = uchar(static_cast<uchar>(sum / count));
            }
        }
    }

    return zoomedImage;
}

Mat zoomIn(const Mat& img) {
    Mat zoomedImage(2 * img.rows, 2 * img.cols, img.type());
    int channels = img.channels();

    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            const uchar* oldRow = img.ptr<uchar>(y, x);
            uchar* newRow = zoomedImage.ptr<uchar>(2 * y, 2 * x);
            memcpy(newRow, oldRow, channels);
        }
    }

    if (channels == 3) {
        for (int y = 0; y < zoomedImage.rows; y += 2) {
            for (int x = 1; x < zoomedImage.cols - 1; x += 2) {
                Vec3b previousPixel = zoomedImage.at<Vec3b>(y, x - 1);
                Vec3b nextPixel = zoomedImage.at<Vec3b>(y, x - 1);
                for (int c = 0; c < channels; c ++) {
                    zoomedImage.at<Vec3b>(y, x)[c] = (previousPixel[c] + nextPixel[c]) / 2;
                }
            }
        }

        for (int x = 0; x < zoomedImage.cols; x ++) {
            for (int y = 1; y < zoomedImage.rows - 1; y += 2) {
                Vec3b previousPixel = zoomedImage.at<Vec3b>(y - 1, x);
                Vec3b nextPixel = zoomedImage.at<Vec3b>(y + 1, x);
                for (int c = 0; c < channels; c ++) {
                    zoomedImage.at<Vec3b>(y, x)[c] = (previousPixel[c] + nextPixel[c]) / 2;
                }
            }
        }
    } else {
        for (int y = 0; y < zoomedImage.rows; y += 2) {
            for (int x = 1; x < zoomedImage.cols - 1; x += 2) {
                int previousPixel = zoomedImage.at<uchar>(y, x - 1);
                int nextPixel = zoomedImage.at<uchar>(y, x + 1);
                int currentPixel = (previousPixel + nextPixel) / 2;
                zoomedImage.at<uchar>(y, x) = currentPixel;
            }
        }

        for (int x = 0; x < zoomedImage.cols; x ++) {
            for (int y = 1; y < zoomedImage.rows - 1; y += 2) {
                int previousPixel = zoomedImage.at<uchar>(y - 1, x);
                int nextPixel = zoomedImage.at<uchar>(y + 1, x);
                int currentPixel = (previousPixel + nextPixel) / 2;
                zoomedImage.at<uchar>(y, x) = currentPixel;
            }
        }
    }

    return zoomedImage;
}

Mat rotateImage(const Mat& img, bool clockwise) {
    Mat rotatedImage(img.cols, img.rows, img.type());
    int channels = img.channels();
    
    if (clockwise) {
        for (int y = 0; y < img.rows; y++) {
            for (int x = 0; x < img.cols; x++) {
                const uchar* oldRow = img.ptr<uchar>(y) + x * channels;
                uchar* newColumn = rotatedImage.ptr<uchar>(x) + (img.rows - y - 1) * channels;
                memcpy(newColumn, oldRow, channels);
            }
        }
    } else {
        for (int y = 0; y < img.rows; y++) {
            for (int x = 0; x < img.cols; x++) {
                const uchar* oldRow = img.ptr<uchar>(y) + x * channels;
                uchar* newColumn = rotatedImage.ptr<uchar>(img.cols - x - 1) + y * channels;
                memcpy(newColumn, oldRow, channels);
            }
        }
    }

    return rotatedImage;
}
