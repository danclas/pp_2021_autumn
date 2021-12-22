  // Copyright 2021 Grishchenko Andrei
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "./matrix_max_in_rows.h"

std::vector<int> randomMatrix(int cols, int rows) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> rmat(cols * rows);
    for (int i = 0; i < cols * rows; i++) {
        rmat[i] = gen() % 100;
    }
    return rmat;
}

std::vector<int> getSequentialOperations(std::vector<int> mat, int cols, int rows) {
    std::vector<int> max_elems(rows);
    for (int i = 0; i < rows; i++) {
        int max_elem = mat[i * cols];
        for (int j = 1; j < cols; j++)
            max_elem = std::max(max_elem, mat[i * cols + j]);
        max_elems[i] = max_elem;
    }
    return max_elems;
}

std::vector<int> getParallelOperations(std::vector<int> global_mat, int cols, int rows) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int count = rows / size;
    const int epsilon = rows % size;
    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(global_mat.data() + proc * count * cols + epsilon * cols,
                count * cols, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }
    std::vector<int> local_mat(rank == 0 ? (count + epsilon) * cols : count * cols);
    if (rank == 0) {
        local_mat = { global_mat.begin(), global_mat.begin() + (count + epsilon) * cols };
    } else {
        MPI_Status status;
        MPI_Recv(local_mat.data(), count * cols, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    std::vector<int> result(rows);
    std::vector<int> local_result = getSequentialOperations(local_mat, cols, rank == 0 ? count + epsilon : count);
    if (rank == 0) {
        for (int i = 0; i < count + epsilon; i++)
            result[i] = local_result[i];
        for (int proc = 1; proc < size; proc++) {
            MPI_Status status;
            MPI_Recv(result.data() + epsilon + count * proc, count, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
        }
    } else {
        MPI_Send(local_result.data(), count, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    return result;
}
