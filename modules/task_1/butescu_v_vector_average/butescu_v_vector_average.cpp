  // Copyright 2021 Butescu Vladimir
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include"./butescu_v_vector_average.h"

std::vector<int> getRandomPositiveVector(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    if (size < 0) { size = 0; }
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) { vec[i] = dist(gen); }
    return vec;
}

int getParallelAverage(std::vector<int> parall_vec, int size) {
    int ProcRank, ProcNum, sum_all;
    double Paverage = 0;

    if (size <= 0) {
        return 0;
    }

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int delta = ProcNum - size % ProcNum;
    for (int i = 0; i < delta; i++) {
        parall_vec.push_back(0);
        size++;
    }


    int partSize = size / ProcNum;
    if (ProcRank == 0) {
        for (int i = 1; i < ProcNum; i++) {
            MPI_Send(parall_vec.data() + i * partSize, partSize, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(partSize);
    if (ProcRank == 0) {
        local_vec = std::vector<int>(parall_vec.begin(), parall_vec.begin() + partSize);
    } else {
        MPI_Status status;
        MPI_Recv(local_vec.data(), partSize, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int sum = sum_all = 0;
    for (int i = 0; i < partSize; i++) 
        sum += local_vec[i];

    MPI_Reduce(&sum, &sum_all, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    Paverage = static_cast<double>(sum_all) / size;

    return Paverage;
}

int getSequentialAverage(std::vector<int> sequent_vec) {
    int size = sequent_vec.size(), sum_all = 0;
    double Saverage = 0;

    if (size <= 0) {
        return 0;
    }

    for (int i = 0; i < size; i++) {
        sum_all += sequent_vec[i];
    }

    Saverage = static_cast<double>(sum_all) / size;
    return Saverage;
}

