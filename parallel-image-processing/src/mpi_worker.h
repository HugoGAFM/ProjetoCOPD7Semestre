#ifndef MPI_WORKER_H
#define MPI_WORKER_H

#include <mpi.h>
#include <vector>
#include <string>

class MPIWorker {
public:
    MPIWorker();
    ~MPIWorker();

    // initialize MPI (pass argc/argv from main)
    void initMPI(int &argc, char **argv);

    // send/receive raw image buffer
    void sendImage(const std::vector<unsigned char>& image, int destination);
    void receiveImage(std::vector<unsigned char>& image, int source);

    // finalize MPI
    void finalizeMPI();

    int getProcessRank() const;
    int getNumProcesses() const;

private:
    int processRank;
    int numProcesses;
};

#endif // MPI_WORKER_H