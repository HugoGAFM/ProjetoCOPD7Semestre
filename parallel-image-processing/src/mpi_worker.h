#ifndef MPI_WORKER_H
#define MPI_WORKER_H

#include <mpi.h>
#include <vector>
#include <string>

class MPIWorker {
public:
    MPIWorker();
    ~MPIWorker();

    // iniciar MPI
    void initMPI(int &argc, char **argv);

    // manda e recebe imagens como vetores de bytes (por exemplo, dados de imagem serializados).
    void sendImage(const std::vector<unsigned char>& image, int destination);
    void receiveImage(std::vector<unsigned char>& image, int source);

    // Terminar MPI
    void finalizeMPI();

    int getProcessRank() const;
    int getNumProcesses() const;

private:
    int processRank;
    int numProcesses;
};

#endif // MPI_WORKER_H