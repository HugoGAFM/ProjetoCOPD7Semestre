#include "image_processor.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

using namespace cv;
using namespace std;

// Construtor que armazena os diretórios de entrada e saída para processar as imagens.
ImageProcessor::ImageProcessor(const std::string& inputDir, const std::string& outputDir)
    : inputDirectory(inputDir), outputDirectory(outputDir) {}

// Carrega uma imagem colorida do arquivo especificado.
// Lança exceção se o arquivo não existir ou não puder ser lido.
void ImageProcessor::loadImage(const std::string& filename, Mat& image) {
    image = imread(filename, IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Could not load image: " << filename << endl;
        throw runtime_error("Image loading failed");
    }
}

void ImageProcessor::applyFilter(const Mat& inputImage, Mat& outputImage, const std::string& filterType) {
    if (filterType == "blur") {
        // Aplica um desfoque Gaussiano 5x5 na imagem de entrada.
        GaussianBlur(inputImage, outputImage, Size(5, 5), 0);
    } else if (filterType == "sharpen") {
        // Aplica um filtro de nitidez com kernel 3x3.
        Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0,
                                            -1, 5, -1,
                                             0, -1, 0);
        filter2D(inputImage, outputImage, inputImage.depth(), kernel);
    } else if (filterType == "edge") {
        // Converte a imagem para preto e branco.
        cv::Mat gray;
        cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);
        cv::cvtColor(gray, outputImage, cv::COLOR_GRAY2BGR);
    } else if (filterType == "negative") {
        // Inverte as cores da imagem (negativo).
        cv::bitwise_not(inputImage, outputImage);
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

// Processa todos os arquivos encontrados no diretório de entrada usando o filtro selecionado.
// Os resultados são salvos no diretório de saída com o mesmo nome de arquivo.
void ImageProcessor::processImages(const std::string& filterType) {
    for (const auto& entry : filesystem::directory_iterator(inputDirectory)) {
        Mat inputImage, outputImage;
        loadImage(entry.path().string(), inputImage);
        applyFilter(inputImage, outputImage, filterType);
        saveImage(outputDirectory + "/" + entry.path().filename().string(), outputImage);
    }
}