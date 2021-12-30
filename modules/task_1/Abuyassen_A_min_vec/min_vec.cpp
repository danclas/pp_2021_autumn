// Copyright 2021 Abuyassen Albara

#include "../../../modules/task_1/Abuyassen_A_min_vec/min_vec.h"

#include <mpi.h>

#include <vector>

#include <random>

#include <algorithm>

#include <iostream>

std::vector < int > getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector < int > vec(sz);
    for (int i = 0; i < sz; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}

int getSequentialOperations(std::vector < int > vec) {
    return *std::min_element(vec.begin(), vec.end());
}

int getParallelOperations(std::vector < int > global_vec,
    int count_size_vector) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int delta = count_size_vector / ProcNum;
    int temp = count_size_vector - delta * ProcNum;
    if (ProcRank == 0) {
        for (int proc = 1; proc < ProcNum; proc++) {
            MPI_Send(&global_vec[0] + temp + proc * delta, delta,
                MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }
    int size;
    if (ProcRank == 0) size = temp + delta;
    else
        size = delta;
    std::vector < int > loc_vec(size);
    if (ProcRank == 0) {
        loc_vec = std::vector < int >(global_vec.begin(),
            global_vec.begin() + delta + temp);
    } else {
        MPI_Status status;
        MPI_Recv(&loc_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    int loc_min = getSequentialOperations(loc_vec);
    int glob_min;
    MPI_Reduce(&loc_min, &glob_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    return glob_min;
}
