// Copyright 2021 Pyatckin Nickolay
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/pyatckin_n_avg_of_vector/avg_of_vector.h"


std::vector<int> getRandomVector(int sz) {
    std::mt19937 gen(121);
    std::vector<int> vec(sz);
    for (int i = 0; i < sz; i++) { vec[i] = gen() % 1000 / 100.0; }
    return vec;
}

int getSequentialOperations(std::vector<int>& vec, int size_vec) {
    int local_average = 0;
    for (int i = 0; i < size_vec; i++) {
        local_average += vec[i];
    }
    return  static_cast<int>(local_average / size_vec);
}

int getParallelOperations(std::vector<int>& vec,
    int size_vec) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = size_vec / size;
    int global_average = 0;
    int tail_average = 0;
    std::vector<int> subset_of_vec(delta);

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(vec.data() + proc * delta, delta,
                MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(delta);
    int local_sum = 0;
    if (rank == 0) {
        local_vec = std::vector<int>(vec.begin(),
            vec.begin() + delta);
    } else {
        MPI_Recv(local_vec.data(), delta, MPI_INT,
            0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    }
    for (std::vector<int>::size_type i = 0; i < delta; i++) {
        local_sum += local_vec[i];
    }

    MPI_Reduce(&local_sum, &global_average, 1,
        MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int tail = size_vec - (delta * size);
        if (tail) {
            subset_of_vec = std::vector<int>(vec.end() - tail, vec.end());

            for (std::vector<int>::size_type i = 0; i < tail; i++) {
                tail_average += subset_of_vec[i];
            }

            return  static_cast<int>
                ((tail_average + global_average) / size_vec);
        }
    }
    return  static_cast<int>(global_average / size_vec);
}
