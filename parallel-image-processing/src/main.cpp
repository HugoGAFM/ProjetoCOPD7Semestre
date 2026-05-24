#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <opencv2/opencv.hpp>
#include "image_processor.h"
#include "mpi_worker.h"
#include "openmp_filters.h"

namespace fs = std::filesystem;

void processImagesWithMPI(int argc, char** argv, const std::string& inputDir,
                          const std::string& outputDir, const std::string& filterType) {
    MPIWorker mpiWorker;
    mpiWorker.initMPI(argc, argv);

    int rank = mpiWorker.getProcessRank();
    int size = mpiWorker.getNumProcesses();

    std::vector<std::string> images;
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        if (ext == ".jpg" || ext == ".png" || ext == ".jpeg" || ext == ".bmp") {
            images.push_back(entry.path().string());
        }
    }

    if (rank == 0) {
        std::cout << "MPI: processos = " << size
                  << ", imagens totais = " << images.size() << std::endl;
    }

    // mapping for "mixed" mode: each rank gets a fixed filter
    const std::vector<std::string> filters = { "blur", "sharpen", "edge" };
    std::string localFilter;
    if (filterType == "mixed") {
        localFilter = filters[rank % static_cast<int>(filters.size())];
        std::cout << "[rank " << rank << "] usando filtro (mixed) -> " << localFilter << std::endl;
    } else {
        // validate requested filter, fallback to blur if unknown
        if (filterType == "blur" || filterType == "sharpen" || filterType == "edge") {
            localFilter = filterType;
        } else {
            localFilter = "blur";
            std::cout << "[rank " << rank << "] filtro desconhecido '" << filterType << "', usando 'blur' por padrao\n";
        }
    }

    auto startTime = std::chrono::steady_clock::now();
    int processed = 0;

    ImageProcessor processor(inputDir, outputDir);
    for (size_t i = 0; i < images.size(); ++i) {
        if ((int)(i % size) != rank) continue;

        cv::Mat in, out;
        processor.loadImage(images[i], in);
        processor.applyFilter(in, out, localFilter);

        // add rank suffix to avoid name clash in mixed mode
        std::string outName = fs::path(images[i]).filename().string();
        if (filterType == "mixed") {
            outName = fs::path(images[i]).stem().string() + "_r" + std::to_string(rank) + "_" + localFilter + fs::path(images[i]).extension().string();
        }
        std::string outPath = outputDir + "/" + outName;
        processor.saveImage(outPath, out);

        ++processed;
        if (processed % 10 == 0) {
            auto elapsed = std::chrono::duration<double>(
                std::chrono::steady_clock::now() - startTime).count();
            double rate = elapsed > 0.0 ? processed / elapsed : 0.0;
            std::cout << "[rank " << rank << "] processadas " << processed
                      << " imagens (" << rate << " img/s)" << std::endl;
        }
    }

    auto totalTime = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - startTime).count();

    // sincroniza todos os ranks antes do resumo final
    MPI_Barrier(MPI_COMM_WORLD);

    if (size > 1) {
        if (rank == 0) {
            std::vector<int> counts(size, 0);
            std::vector<double> times(size, 0.0);
            counts[0] = processed;
            times[0] = totalTime;
            for (int r = 1; r < size; ++r) {
                MPI_Recv(&counts[r], 1, MPI_INT, r, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&times[r], 1, MPI_DOUBLE, r, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            for (int r = 0; r < size; ++r) {
                double rate = times[r] > 0.0 ? counts[r] / times[r] : 0.0;
                std::cout << "[rank " << r << "] finalizado: " << counts[r]
                          << " imagens em " << times[r] << " s -> "
                          << rate << " img/s" << std::endl;
            }
        } else {
            MPI_Send(&processed, 1, MPI_INT, 0, 100, MPI_COMM_WORLD);
            MPI_Send(&totalTime, 1, MPI_DOUBLE, 0, 101, MPI_COMM_WORLD);
        }
    } else {
        double finalRate = totalTime > 0.0 ? processed / totalTime : 0.0;
        std::cout << "[rank 0] finalizado: " << processed
                  << " imagens em " << totalTime << " s -> "
                  << finalRate << " img/s" << std::endl;
    }

    mpiWorker.finalizeMPI();
}

void processImagesOpenMP(const std::string& inputDir,
                         const std::string& outputDir,
                         const std::string& filterType) {
    std::vector<std::string> paths;
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        if (ext == ".jpg" || ext == ".png" || ext == ".jpeg" || ext == ".bmp") {
            paths.push_back(entry.path().string());
        }
    }

    int totalImages = static_cast<int>(paths.size());
    int numThreads = 1;
#ifdef _OPENMP
    numThreads = omp_get_max_threads();
#endif

    std::cout << "OpenMP: threads = " << numThreads
              << ", imagens totais = " << totalImages << std::endl;

    std::vector<int> threadCounts(numThreads, 0);
    auto startTime = std::chrono::steady_clock::now();

    const std::vector<std::string> filters = { "blur", "sharpen", "edge" };
    bool mixedMode = (filterType == "mixed");

    if (mixedMode) {
        std::cout << "Mixed mode: thread-to-filter mapping:\n";
        for (int t = 0; t < numThreads; ++t) {
            std::cout << "  thread " << t << " -> " << filters[t % (int)filters.size()] << "\n";
        }
    } else {
        std::cout << "Uniform mode: all threads use filter '" << filterType << "'\n";
    }

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < totalImages; ++i) {
        int tid = 0;
#ifdef _OPENMP
        tid = omp_get_thread_num();
#endif
        std::string useFilter = mixedMode ? filters[tid % (int)filters.size()] : filterType;

        cv::Mat image = cv::imread(paths[i], cv::IMREAD_COLOR);
        if (image.empty()) continue;

        if (useFilter == "blur") {
            applyBlur(image, 5);
        } else if (useFilter == "sharpen") {
            applySharpen(image);
        } else if (useFilter == "edge") {
            applyEdgeDetection(image);
        } else {
            applyBlur(image, 3);
        }

        auto p = fs::path(paths[i]);
        std::string outName = p.stem().string() + "_t" + std::to_string(tid) + "_" + useFilter + p.extension().string();
        std::string outPath = outputDir + "/" + outName;
        cv::imwrite(outPath, image);
        #pragma omp atomic
        threadCounts[tid]++;
    }

    auto totalTime = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - startTime).count();
    double rate = totalTime > 0.0 ? totalImages / totalTime : 0.0;

    std::cout << "OpenMP finalizado: " << totalImages
              << " imagens em " << totalTime << " s -> " << rate << " img/s" << std::endl;

    for (int t = 0; t < numThreads; ++t) {
        if (threadCounts[t] > 0) {
            std::cout << "  thread " << t << " processou "
                      << threadCounts[t] << " imagens" << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <mode> <input_dir> <output_dir> [filter]\n";
        std::cerr << "Modes: mpi, openmp\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputDir = argv[2];
    std::string outputDir = argv[3];
    std::string filter = (argc >= 5) ? argv[4] : "blur";

    if (mode == "mpi") {
        processImagesWithMPI(argc, argv, inputDir, outputDir, filter);
    } else if (mode == "openmp") {
        processImagesOpenMP(inputDir, outputDir, filter);
    } else {
        std::cerr << "Invalid mode. Use 'mpi' or 'openmp'.\n";
        return 1;
    }

    return 0;
}