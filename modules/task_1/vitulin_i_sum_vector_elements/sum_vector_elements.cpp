// Copyright 2021 Vitulin Ivan 381908-1
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/vitulin_i_sum_vector_elements/sum_vector_elements.h"

std::vector<int> getRandomVectorElements(int vector_size, int k) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vect(vector_size);
    for (int i = 0; i < vector_size; i++) { vect[i] = gen() % k;  }
    return vect;
}

int getSumOfVectorElementsSequentially(std::vector<int> vect) {
    const int vector_size = vect.size();
    int sum_vect_elements = 0;
    for (int i = 0; i < vector_size; i++) {
        sum_vect_elements += vect[i];
    }
    return sum_vect_elements;
}

int getSumOfVectorElementsParallelly(std::vector<int> global_vector, int count_size_vec) {
    int ProcessNum, ProcessRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcessNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);

    int addition = ProcessNum - count_size_vec % ProcessNum;
    for (int i = 0; i < addition; i++) {
        global_vector.push_back(0);
        count_size_vec++;
    }

    int local_part = count_size_vec / ProcessNum;
    std::vector<int> local_vector(local_part);
    if (ProcessRank == 0) {
        for (int i = 1; i < ProcessNum; i++) {
            MPI_Send(&global_vector[0] + i * local_part, local_part, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        local_vector = std::vector<int>(global_vector.begin(), global_vector.begin() + local_part);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vector[0], local_part, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }

    int global_sum_elements = 0;
    int local_sum_elements = getSumOfVectorElementsSequentially(local_vector);
    MPI_Reduce(&local_sum_elements, &global_sum_elements, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum_elements;
}
