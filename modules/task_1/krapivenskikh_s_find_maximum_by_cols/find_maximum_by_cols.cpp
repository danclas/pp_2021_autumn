// Copyright 2021 Krapivenskikh Stepan
#include <mpi.h>
#include <vector>
#include <random>
#include "../../../modules/task_1/krapivenskikh_s_find_maximum_by_cols/find_maximum_by_cols.h"

std::vector<int> getRandomMatrix(size_t matrixSize) {
    std::random_device dev;
    std::mt19937 gen(dev());

    std::vector<int> vectorOfValue(matrixSize);
    for (size_t  i = 0; i < matrixSize; i++) {
        vectorOfValue[i] = gen() % 1000;
    }
    return vectorOfValue;
}

std::vector<int> getSequentialFunction(std::vector<int> matrix, size_t matrixRows, size_t matrixCols) {
    std::vector<int> vectorOfMaximum(matrixCols);

    for (size_t i = 0; i < matrixCols; i++) {
        vectorOfMaximum[i] = 0;
        for (size_t j = 0; j < matrixRows; j++) {
            if (vectorOfMaximum[i] < matrix[j * matrixCols + i]) {
                vectorOfMaximum[i] = matrix[j * matrixCols + i];
            }
        }
    }
    return vectorOfMaximum;
}
std::vector<int> getParallelFunction(std::vector<int> global_matrix, size_t matrixRows, size_t matrixCols) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = matrixRows / size;
    std::vector<int> localMatrix(delta * matrixCols), vectorOfMaximum;

    if (rank == 0) {
        vectorOfMaximum.resize(matrixCols);
    }

    MPI_Scatter(global_matrix.data(), delta * matrixCols, MPI_INT, localMatrix.data(), delta * matrixCols,
        MPI_INT, 0, MPI_COMM_WORLD);

    localMatrix = getSequentialFunction(localMatrix, delta, matrixCols);
    MPI_Reduce(localMatrix.data(), vectorOfMaximum.data(), matrixCols, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int loss = matrixRows - delta * size;
        if (loss) {
            localMatrix = std::vector<int>(global_matrix.begin() + ((matrixRows - loss) * matrixCols),
                global_matrix.end());
            localMatrix = getSequentialFunction(localMatrix, loss, matrixCols);

            for (size_t i = 0; i < matrixCols; i++) {
                if (localMatrix[i] > vectorOfMaximum[i]) {
                    vectorOfMaximum[i] = localMatrix[i];
                }
            }
        }
    }

    return vectorOfMaximum;
}
