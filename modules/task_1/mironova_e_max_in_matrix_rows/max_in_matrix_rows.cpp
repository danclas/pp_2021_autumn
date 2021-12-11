// Copyright 2021 Mironova Ekaterina
#include "../../../modules/task_1/mironova_e_max_in_matrix_rows/max_in_matrix_rows.h"
#include <string>
#include <random>
#include <algorithm>

std::vector<int> fillRandomMatrix(const size_t rows, const size_t columns) {
    if (rows == 0 ||columns == 0) throw "Matrix size is uncorrect";
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(rows * columns);
    for (std::vector<int>::size_type i = 0; i < rows * columns; i++) {
        matrix[i] = gen() % 100;
    }
    return matrix;
}

std::vector<int> findMaxesInMatrixRows(std::vector<int> matrix, const size_t columns) {
    if (matrix.size() == 0 || columns == 0) throw "Matrix is empty or number of columns is uncorrect";
    std::vector<int> maxesInRows;
    int max;
    for (std::vector<int>::size_type i = 0; i < matrix.size(); i += columns) {
        max = matrix[i];
        for (std::vector<int>::size_type j = 1; j < columns; j++) {
            if (matrix[i + j] > max) {
                max = matrix[i + j];
            }
        }
        maxesInRows.push_back(max);
    }
    return maxesInRows;
}

std::vector<int> parallelFindMaxesInMatrixRows(std::vector<int> matrix, const size_t rows, const size_t columns) {
    std::vector<int> partOfMatrix, maxesInRows, resultMaxesInRows;
    size_t dataPiece = 0;
    int remainingData = 0;
    int max = 0;
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    dataPiece = rows / procNum;
    remainingData = rows % procNum;

    partOfMatrix.resize(dataPiece * columns);
    if (procRank == 0) {
        resultMaxesInRows.resize(rows - remainingData);
    }

    MPI_Scatter(matrix.data(), dataPiece * columns, MPI_INT,
        partOfMatrix.data(), dataPiece * columns, MPI_INT, 0, MPI_COMM_WORLD);

    for (std::vector<int>::size_type i = 0; i < partOfMatrix.size(); i += columns) {
        max = partOfMatrix[i];
        for (std::vector<int>::size_type j = 1; j < columns; j++) {
            if (partOfMatrix[i + j] > max) {
                max = partOfMatrix[i + j];
            }
        }
        maxesInRows.push_back(max);
    }

    MPI_Gather(maxesInRows.data(), dataPiece, MPI_INT, resultMaxesInRows.data(), dataPiece, MPI_INT, 0, MPI_COMM_WORLD);

    if ((procRank == 0) && (remainingData != 0)) {
        for (std::vector<int>::size_type i = (rows - remainingData) * columns; i < matrix.size(); i += columns) {
            max = matrix[i];
            for (std::vector<int>::size_type j = 1; j < columns; j++) {
                if (matrix[i + j] > max) {
                    max = matrix[i + j];
                }
            }
            resultMaxesInRows.push_back(max);
        }
    }

    return resultMaxesInRows;
}
