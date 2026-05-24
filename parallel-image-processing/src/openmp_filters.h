#ifndef OPENMP_FILTERS_H
#define OPENMP_FILTERS_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

void applyBlur(cv::Mat& image, int kernelSize);
void applySharpen(cv::Mat& image);
void applyEdgeDetection(cv::Mat& image);

// process images vector in parallel applying filterType
void processImagesWithOpenMP(std::vector<cv::Mat>& images, const std::string& filterType);

#endif // OPENMP_FILTERS_H