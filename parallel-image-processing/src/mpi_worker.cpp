#include "mpi_worker.h"
#include <mpi.h>
#include <iostream>

MPIWorker::MPIWorker()
    : processRank(0), numProcesses(1) {}

MPIWorker::~MPIWorker() {
    // nada a fazer aqui, apenas para satisfazer o link
}

void MPIWorker::initMPI(int &argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
}

void MPIWorker::sendImage(const std::vector<unsigned char>& image, int destination) {
    int imageSize = static_cast<int>(image.size());
    MPI_Send(&imageSize, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);
    if (imageSize > 0) {
        MPI_Send(image.data(), imageSize, MPI_UNSIGNED_CHAR, destination, 1, MPI_COMM_WORLD);
    }
}

void MPIWorker::receiveImage(std::vector<unsigned char>& image, int source) {
    int imageSize = 0;
    MPI_Recv(&imageSize, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    image.resize(imageSize);
    if (imageSize > 0) {
        MPI_Recv(image.data(), imageSize, MPI_UNSIGNED_CHAR, source, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void MPIWorker::finalizeMPI() {
    MPI_Finalize();
}

int MPIWorker::getProcessRank() const {
    return processRank;
}

int MPIWorker::getNumProcesses() const {
    return numProcesses;
}