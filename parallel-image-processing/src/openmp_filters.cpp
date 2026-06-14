#include "openmp_filters.h"
#include <opencv2/opencv.hpp>
#include <omp.h>

// Aplica desfoque Gaussiano na imagem.
void applyBlur(cv::Mat& image, int kernelSize) {
    if (kernelSize < 1) kernelSize = 1;
    if (kernelSize % 2 == 0) kernelSize += 1;
    cv::GaussianBlur(image, image, cv::Size(kernelSize, kernelSize), 0);
}

// Aplica nitidez à imagem usando convolução com kernel 3x3.
void applySharpen(cv::Mat& image) {
    cv::Mat kernel = (cv::Mat_<float>(3, 3) << 0, -1, 0,
                                               -1, 5, -1,
                                                0, -1, 0);
    cv::Mat result;
    cv::filter2D(image, result, image.depth(), kernel);
    image = result;
}

// Converte a imagem para preto e branco usando limiarização.
void applyEdgeDetection(cv::Mat& image) {
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(gray, image, cv::COLOR_GRAY2BGR);
}

// Inverte as cores da imagem (negativo).
void applyNegative(cv::Mat& image) {
    cv::bitwise_not(image, image);
}

// Processa um vetor de imagens em paralelo usando OpenMP.
// Cada iteração aplica o filtro selecionado à imagem correspondente.
void processImagesWithOpenMP(std::vector<cv::Mat>& images, const std::string& filterType) {
    int count = static_cast<int>(images.size());

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < count; ++i) {
        if (filterType == "blur") {
            applyBlur(images[i], 5);
        } else if (filterType == "sharpen") {
            applySharpen(images[i]);
        } else if (filterType == "edge") {
            applyEdgeDetection(images[i]);
        } else if (filterType == "negative") {
            applyNegative(images[i]);
        } else {
            applyBlur(images[i], 3);
        }
    }
}