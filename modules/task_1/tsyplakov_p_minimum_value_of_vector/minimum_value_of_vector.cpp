// Copyright 2021 Tsyplakov Pavel
#include <mpi.h>
#include <vector>
#include <random>
#include <iostream>
#include "../../../modules/task_1/tsyplakov_p_minimum_value_of_vector/minimum_value_of_vector.h"


std::vector<int> getRandomVector(std::vector<int>::size_type size) {
    std::mt19937 gen(10);  // pseudo random
    std::vector<int> vec(size);
    for (std::vector<int>::size_type i = 0; i < size; i++) {
        vec[i] = gen() % 119;
    }

    return vec;
}

int getSequentialOperations(const std::vector<int>& vec) {
    int min_elem = vec[0];
    for (std::vector<int>::size_type i = 0; i < vec.size(); ++i) {
        if (vec[i] < min_elem) { min_elem = vec[i]; }
    }
    return min_elem;

    // doesnt work on ubuntu smh
    // return *std::min_element(std::begin(vec), std::end(vec));
}

int getParallelOperations(const std::vector<int>& global_vec,
    const std::vector<int>::size_type count_size_vector) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = count_size_vector / size;
    // global_vec(count_size_vector) - input data
    // local_vec - where min will be saved
    // subset_of_vec(delta) - buffer
    std::vector<int>subset_of_vec(delta);
    int local_min, global_min;

    MPI_Scatter(global_vec.data(), delta, MPI_INT, subset_of_vec.data(),
        delta, MPI_INT, 0, MPI_COMM_WORLD);

    local_min = getSequentialOperations(subset_of_vec);

    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int tail = count_size_vector - (delta * size);
        if (tail) {
            subset_of_vec = std::vector<int>(global_vec.begin() + size * delta, global_vec.end());
            local_min = getSequentialOperations(subset_of_vec);

            if (global_min > local_min) {
                global_min = local_min;
            }
        }
    }

    return global_min;
}
