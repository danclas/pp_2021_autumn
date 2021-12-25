// Copyright 2021 Chornyi Yurii
#include <mpi.h>
#include <random>
#include <vector>

#include "../../../modules/task_1/chornyi_y_find_matrix_min/find_matrix_min.h"

std::vector<int> fillMatrixWithRandom(const size_t numberOfRows, const size_t numberOfColumns) {
    std::random_device dev;
    std::mt19937 gen(dev());

    std::vector<int> matrix(numberOfRows * numberOfColumns);
    for (auto it = matrix.begin(); it != matrix.end(); ++it) {  // Filling the matrix with random values
        *it = gen() % 10000;
    }

    return matrix;
}
int singleFindMinimum(std::vector<int> matrix) {
    int minimum = matrix[0];  // First assigment minimum

    for (auto it = matrix.begin() + 1; it != matrix.end(); ++it) {  // Find minimum in the whole matrix
        if (minimum > *it) {
            minimum = *it;
        }
    }

    return minimum;
}

int parallelFindMinimum(std::vector<int> matrix, const size_t numberOfRows, const size_t numberOfColumns) {
    int excessData = 0, dataPackage = 0;  // Necessary variables to send data between processes
    int globalMinimum = 0, localMinimum = 0;  // Interprocess minimum and process minimum

    int numberOfProcess, currentRank;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);

    // Calculating the amount of data that the root process will leave
    excessData = numberOfRows % numberOfProcess;
    // Calculating the amount of data that is distributed between the process
    dataPackage = numberOfRows / numberOfProcess;

    // Allocating memory for the local matrix at each process
    std::vector<int> localMatrix(dataPackage * numberOfColumns);

    MPI_Scatter(matrix.data() + excessData * numberOfColumns, dataPackage * numberOfColumns, MPI_INT,
        localMatrix.data(), dataPackage * numberOfColumns, MPI_INT, 0, MPI_COMM_WORLD);

    localMinimum = singleFindMinimum(localMatrix);  // Calculating minimum at each process

    // Collect the result for each process in the root process
    MPI_Reduce(&localMinimum, &globalMinimum, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (currentRank == 0) {
        if (excessData) {
            localMatrix = std::vector<int>(matrix.begin(), matrix.begin() + excessData * numberOfColumns);
            localMinimum = singleFindMinimum(localMatrix);
            if (globalMinimum > localMinimum) {
                globalMinimum = localMinimum;
            }
        }
    }

    return globalMinimum;
}
