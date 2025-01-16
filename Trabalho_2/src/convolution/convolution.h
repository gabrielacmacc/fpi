#include <stdio.h>
#include <iostream>

#ifndef convolution_h
#define convolution_h

Mat applyConvolution(const Mat& img, float data[], bool shouldMove);
Mat applyGaussian(const Mat& img);
Mat applyLaplacian(const Mat& img);
Mat applyHighPass(const Mat& img);
Mat applyPrewittHx(const Mat& img);
Mat applyPrewittHy(const Mat& img);
Mat applySobelHx(const Mat& img);
Mat applySobelHy(const Mat& img);

#endif