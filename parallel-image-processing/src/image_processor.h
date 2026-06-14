#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <string>
#include <opencv2/opencv.hpp>

class ImageProcessor {
public:
    // Construtor que define o diretório de entrada e o diretório de saída.
    ImageProcessor(const std::string& inputDir, const std::string& outputDir);

    // Carrega uma imagem colorida do disco para um cv::Mat.
    void loadImage(const std::string& filename, cv::Mat& image);

    // Aplica um filtro nomeado à imagem de entrada e grava o resultado em outputImage.
    // Filtros suportados: "blur", "sharpen", "edge" (preto e branco).
    void applyFilter(const cv::Mat& inputImage, cv::Mat& outputImage, const std::string& filterType);

    // Salva a imagem processada em arquivo.
    void saveImage(const std::string& filename, const cv::Mat& image);

    // Processa todas as imagens do diretório de entrada e grava no diretório de saída.
    void processImages(const std::string& filterType);

private:
    std::string inputDirectory;
    std::string outputDirectory;
};

#endif // IMAGE_PROCESSOR_H