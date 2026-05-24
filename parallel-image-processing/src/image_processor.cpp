#include "image_processor.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

using namespace cv;
using namespace std;

ImageProcessor::ImageProcessor(const std::string& inputDir, const std::string& outputDir)
    : inputDirectory(inputDir), outputDirectory(outputDir) {}

void ImageProcessor::loadImage(const std::string& filename, Mat& image) {
    image = imread(filename, IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Could not load image: " << filename << endl;
        throw runtime_error("Image loading failed");
    }
}

void ImageProcessor::applyFilter(const Mat& inputImage, Mat& outputImage, const std::string& filterType) {
    if (filterType == "blur") {
        GaussianBlur(inputImage, outputImage, Size(5, 5), 0);
    } else if (filterType == "sharpen") {
        Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
        filter2D(inputImage, outputImage, inputImage.depth(), kernel);
    } else if (filterType == "edge") {
        Canny(inputImage, outputImage, 100, 200);
    } else {
        cerr << "Unknown filter type: " << filterType << endl;
        throw invalid_argument("Invalid filter type");
    }
}

void ImageProcessor::saveImage(const std::string& filename, const Mat& image) {
    if (!imwrite(filename, image)) {
        cerr << "Could not save image: " << filename << endl;
        throw runtime_error("Image saving failed");
    }
}

void ImageProcessor::processImages(const std::string& filterType) {
    for (const auto& entry : filesystem::directory_iterator(inputDirectory)) {
        Mat inputImage, outputImage;
        loadImage(entry.path().string(), inputImage);
        applyFilter(inputImage, outputImage, filterType);
        saveImage(outputDirectory + "/" + entry.path().filename().string(), outputImage);
    }
}