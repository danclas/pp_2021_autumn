// Copyright 2021 Miheev Ivan
#include <mpi.h>
#include <random>
#include "../../../modules/task_1/miheev_i_finding_minimum_values/finding_minimum_values.h"

void getMatrix(std::vector<int>* matrix,
               std::vector<int>::size_type matrixSize) {
  std::random_device dev;
  std::mt19937 gen(dev());

  matrix->resize(matrixSize);
  for (std::vector<int>::size_type i = 0; i < matrixSize; i++) {
    matrix->at(i) = gen() % 1000;
  }
}

// Sequential
std::vector<int> sequentialFindMin(
    const std::vector<int>& matrix, std::vector<int>::size_type matrixRows,
    std::vector<int>::size_type matrixColumns) {
  std::vector<int> vectorMinimum(matrixColumns);

  for (std::vector<int>::size_type i = 0; i < matrixColumns; i++) {
    vectorMinimum[i] = matrix[i];
    for (std::vector<int>::size_type j = 0; j < matrixRows; j++) {
      if (vectorMinimum[i] > matrix[matrixColumns * j + i])
        vectorMinimum[i] = matrix[matrixColumns * j + i];
    }
  }

  return vectorMinimum;
}

// Parallel
std::vector<int> parallelFindMin(
    const std::vector<int>& matrix, std::vector<int>::size_type matrixRows,
    std::vector<int>::size_type matrixColumns) {
  std::vector<int> localVectorMinimum, globalVectorMinimum;
  int dataPerProcess = 0, ExtraData = 0;

  int numberOfProcess, currentProcess;
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

  dataPerProcess = matrixRows / numberOfProcess;
  ExtraData = matrixRows % numberOfProcess;

  if (currentProcess == 0) {
    globalVectorMinimum.resize(matrixColumns);
  }

  localVectorMinimum.resize(dataPerProcess * matrixColumns);

  MPI_Scatter(matrix.data() + ExtraData * matrixColumns,
              dataPerProcess * matrixColumns, MPI_INT,
              localVectorMinimum.data(), dataPerProcess * matrixColumns,
              MPI_INT, 0, MPI_COMM_WORLD);

  localVectorMinimum = sequentialFindMin(localVectorMinimum,
                                               dataPerProcess, matrixColumns);

  MPI_Reduce(localVectorMinimum.data(), globalVectorMinimum.data(),
             matrixColumns, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (currentProcess == 0) {
    if (ExtraData) {
      localVectorMinimum = std::vector<int>(
          matrix.begin(), matrix.begin() + ExtraData * matrixColumns);
      localVectorMinimum =
          sequentialFindMin(localVectorMinimum, ExtraData, matrixColumns);

      for (std::vector<int>::size_type i = 0; i < matrixColumns; ++i) {
        if (globalVectorMinimum[i] > localVectorMinimum[i]) {
          globalVectorMinimum[i] = localVectorMinimum[i];
        }
      }
    }
  }

  return globalVectorMinimum;
}
