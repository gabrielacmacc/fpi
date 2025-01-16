#include <opencv2/opencv.hpp>
#include <windows.h>
#include <stdio.h>
#include <iostream>

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

Mat makeHistogramImage(vector<int> histogram) {
	int hue = 256;
	int height = 400;
	int width = 400;

	Mat histImage(height, width, CV_8UC1, Scalar(255));

	int binWidth = cvRound((double)width / hue);
	int max = *max_element(histogram.begin(), histogram.end());

	for (int i = 0; i < hue; i++) {
    	histogram[i] = ((double)histogram[i] / max) * height;
        line(
			histImage,
            Point(binWidth * i, height),
            Point(binWidth * i, height - histogram[i]),
            Scalar(0), 
			1, 8, 0
		);
    }

	int padding = 10;
    Mat paddedHistImage(height + 2 * padding, width + 2 * padding, CV_8UC1, Scalar(255, 255, 255));
    histImage.copyTo(paddedHistImage(Rect(padding, padding, width, height)));

	return paddedHistImage; 
}

vector<int> makeHistogram(const Mat& img) { 
	int hue = 256;

	vector<int> histogram(hue, 0);

	Mat grayImage(img.size(), img.type());

	if (img.channels() != 3) {
		grayImage = img.clone();
	} else {
		grayImage = convertToGS(img);
	}

    for (int y = 0; y < grayImage.rows; y++) {
        for (int x = 0; x < grayImage.cols; x++) {
            int pixel = grayImage.at<uchar>(y, x);
            histogram[pixel]++;
    	}
	}

	return histogram; 
}

Mat equalizeColor(const Mat &img) {
	Mat equalizedImage(img.size(), img.type());

	int hue = 256;
    vector<int> histogram_cum(hue, 0);
	
	int num_pixels = img.total();
	float scaling_factor = 255.0 / num_pixels;

	vector<int> histogram = makeHistogram(img);
    histogram_cum[0] = static_cast<int>(scaling_factor * histogram[0]);
    for (int i = 1; i < hue; i++) {
        histogram_cum[i] = histogram_cum[i - 1] + static_cast<int>(scaling_factor * histogram[i]);
    }

    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
			for (int c = 0; c < img.channels(); c++) {
            	equalizedImage.at<Vec3b>(y, x)[c] = histogram_cum[img.at<Vec3b>(y, x)[c]];
			}
        }
    }

    return equalizedImage;
}

Mat equalize(const Mat &img) {
	if (img.channels() == 3) {
		return equalizeColor(img);
	}

	Mat equalizedImage(img.size(), img.type());

	vector<int> histogram = makeHistogram(img);

	int hue = 256;
    vector<int> histogram_cum(hue, 0);

	int num_pixels = img.total();
	float scaling_factor = 255.0 / num_pixels;

    histogram_cum[0] = static_cast<int>(scaling_factor * histogram[0]);
    for (int i = 1; i < hue; i++) {
        histogram_cum[i] = histogram_cum[i - 1] + static_cast<int>(scaling_factor * histogram[i]);
    }

    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
			equalizedImage.at<uchar>(y, x) = histogram_cum[img.at<uchar>(y,x)];
        }
    }

    return equalizedImage;
}

Mat histogramMatching(const Mat &img_src, const Mat &img_target) {
	Mat matchedImage(img_src.size(), CV_8UC1);

	Mat grayImage_src(img_src.size(), CV_8UC1);
	Mat grayImage_target(img_target.size(), CV_8UC1);

	if (img_src.channels() != 3) {
		grayImage_src = img_src.clone();
	} else {
		grayImage_src = convertToGS(img_src);
	}

	if (img_target.channels() != 3) {
		grayImage_target = img_target.clone();
	} else {
		grayImage_target = convertToGS(img_target);
	}

	int hue = 256;
    vector<int> histogram_src_cum(hue, 0);
    vector<int> histogram_target_cum(hue, 0);
    vector<int> histogram_matched(hue, 0);

	vector<int> histogram_src = makeHistogram(grayImage_src);
	vector<int> histogram_target = makeHistogram(grayImage_target);

	int num_pixels_src = grayImage_src.total();
	float scaling_factor_src = 255.0 / num_pixels_src;

	int num_pixels_target = grayImage_target.total();
	float scaling_factor_target = 255.0 / num_pixels_target;


    histogram_src_cum[0] = static_cast<int>(scaling_factor_src * histogram_src[0]);
	histogram_target_cum[0] = static_cast<int>(scaling_factor_target * histogram_target[0]);
    for (int i = 1; i < hue; i++) {
        histogram_src_cum[i] = histogram_src_cum[i - 1] + static_cast<int>(scaling_factor_src * histogram_src[i]);
		histogram_target_cum[i] = histogram_target_cum[i - 1] + static_cast<int>(scaling_factor_target * histogram_target[i]);
    }

	for (int i = 0; i < hue; i++) {
        int min_diff = abs(histogram_src_cum[i] - histogram_target_cum[0]);
        int best_match = 0;

        for (int j = 1; j < hue; j++) {
            int diff = abs(histogram_src_cum[i] - histogram_target_cum[j]);
            if (diff < min_diff) {
                min_diff = diff;
                best_match = j;
            }
        }
        histogram_matched[i] = best_match;
    }

    for (int y = 0; y < grayImage_src.rows; y++) {
        for (int x = 0; x < grayImage_src.cols; x++) {
            matchedImage.at<uchar>(y, x) = histogram_matched[grayImage_src.at<uchar>(y, x)];
        }
    }

	return matchedImage;
}

int verifyRange(int pixel) {
	return pixel > 255 ? 255 : (pixel < 0 ? 0 : pixel);
}

Mat adjustLight(const Mat& img, const int& bn) {
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
