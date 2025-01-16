#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include "../basic/basic.h"
#include "../utils/constants.cpp"

using namespace cv;
using namespace std;

void rotateKernel(cv::Mat& kernel) {
    swap(kernel.at<float>(0, 0), kernel.at<float>(2, 2));
    swap(kernel.at<float>(0, 1), kernel.at<float>(2, 1));
    swap(kernel.at<float>(0, 2), kernel.at<float>(2, 0));
    swap(kernel.at<float>(1, 0), kernel.at<float>(1, 2));
}

Mat applyConvolution(const Mat& img, float data[], bool shouldMove) {
    Mat convImage(img.size(), img.type());
    Mat kernel = Mat(3, 3, CV_32F, data).clone();
    rotateKernel(kernel);

    if (img.channels() != 3) {
        for (int y = 1; y < img.rows - 1; y++) {
            for (int x = 1; x < img.cols - 1; x++) {
                int iA = img.at<uchar>(y - 1, x - 1) * kernel.at<float>(0, 0);
                int hB = img.at<uchar>(y - 1, x) * kernel.at<float>(0, 1);
                int gC = img.at<uchar>(y - 1, x + 1) * kernel.at<float>(0, 2);
                int fD = img.at<uchar>(y, x - 1) * kernel.at<float>(1, 0);
                int eE = img.at<uchar>(y, x) * kernel.at<float>(1, 1);
                int dF = img.at<uchar>(y, x + 1) * kernel.at<float>(1, 2);
                int cG = img.at<uchar>(y + 1, x - 1) * kernel.at<float>(2, 0);
                int bH = img.at<uchar>(y + 1, x) * kernel.at<float>(2, 1);
                int aI = img.at<uchar>(y + 1, x + 1) * kernel.at<float>(2, 1);
                int pixel = iA + hB + gC + fD + eE + dF + cG + bH + aI;

                if (shouldMove) {
                    pixel += 127;
                }
                pixel = max(0, min(255, pixel));

                convImage.at<uchar>(y, x) = pixel;
            }
        }
    } else {
        cout << GRAYSCALE_ERROR << endl;
        convImage = img.clone();
    }

    return convImage;
}

Mat applyGaussian(const Mat& img) {
    float data[] = { 
        0.0625, 0.125, 0.0625, 
        0.125,  0.25,  0.125, 
        0.0625, 0.125, 0.0625
    };

    Mat convImage = applyConvolution(img, data, false);

    return convImage;
}

Mat applyLaplacian(const Mat& img) {
    float data[] = {
         0, -1,  0, 
        -1,  4, -1, 
         0, -1,  0
    };
    
    Mat convImage = applyConvolution(img, data, true);

    return convImage;
}

Mat applyHighPass(const Mat& img) {
    float data[] = {
        -1, -1, -1, 
        -1,  8, -1, 
        -1, -1, -1
    };

    Mat convImage = applyConvolution(img, data, false);

    return convImage;
}

Mat applyPrewittHx(const Mat& img) {
    float data[] = {
        -1, 0, 1, 
        -1, 0, 1, 
        -1, 0, 1
    };
    
    Mat convImage = applyConvolution(img, data, true);

    return convImage;
}

Mat applyPrewittHy(const Mat& img) {
    float data[] = {
        -1, -1, -1, 
         0,  0,  0, 
         1,  1,  1
    };
    
    Mat convImage = applyConvolution(img, data, true);
    return convImage;
}

Mat applySobelHx(const Mat& img) {
    float data[] = {
        -1, 0, 1, 
        -2, 0, 2, 
        -1, 0, 1
    };
    
    Mat convImage = applyConvolution(img, data, true);
    return convImage;
}

Mat applySobelHy(const Mat& img) {
    float data[] = {
        -1, -2, -1, 
         0,  0,  0, 
         1,  2,  1
    };
    
    Mat convImage = applyConvolution(img, data, true);
    return convImage;
}