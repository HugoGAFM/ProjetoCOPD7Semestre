#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <string>
#include <opencv2/opencv.hpp>

class ImageProcessor {
public:
    ImageProcessor(const std::string& inputDir, const std::string& outputDir);

    // load image from file into cv::Mat
    void loadImage(const std::string& filename, cv::Mat& image);

    // apply named filter ("blur", "sharpen", "edge")
    void applyFilter(const cv::Mat& inputImage, cv::Mat& outputImage, const std::string& filterType);

    // save image (full output path)
    void saveImage(const std::string& filename, const cv::Mat& image);

    // process all images in inputDirectory with filterType, saving to outputDirectory
    void processImages(const std::string& filterType);

private:
    std::string inputDirectory;
    std::string outputDirectory;
};

#endif // IMAGE_PROCESSOR_H