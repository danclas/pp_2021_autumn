// Copyright 2021 Andrich Maria
#include "../../../modules/task_2/andrich_m_ribbon_vertical_scheme/ribbon_vertical_scheme.h"

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>

int* initEmptyMatrix(int rows) {
  int* matrix = new int[rows];
  for (int i = 0; i < rows; i++) {
    matrix[i] = 0;
  }
  return matrix;
}

int* getRandomMatrix(int rows, int cols) {
  std::random_device dev;
  std::mt19937 gen(dev());
  int* matrix = new int[rows * cols];
  for (int i = 0; i < rows * cols; i++) {
    matrix[i] = gen() % 20;
  }
  return matrix;
}

void SequentialMatrixMultiplication(const int* A, const int ARows,
                                    const int ACols, const int* B,
                                    const int BRows, int* C) {
  for (int i = 0; i < ARows; i++) {
    C[i] = 0;
    int tmp = 0;
    for (int k = 0; k < ACols; k++) {
      tmp += A[i * ACols + k] * B[k];
      C[i] = tmp;
    }
  }
}

void ParallelMatrixMultiplication(const int* A, const int ARows,
                                  const int ACols, const int* B,
                                  const int BRows, int* C) {
  if (ACols != BRows) {
    throw -1;
  }
  int world_size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int ribbon_width = ACols / world_size;
  const int remainder = ACols % world_size;
  int* ribbon = nullptr;
  int* local_vector = nullptr;

  int new_width = ribbon_width + remainder;

  if (rank == 0) {
    ribbon = new int[ARows * new_width];
  } else {
    ribbon = new int[ARows * ribbon_width];
  }
  local_vector = initEmptyMatrix(ARows);
  if (rank == 0) {
    for (int i = 0; i < ARows; i++) {
      MPI_Scatter(A + remainder + i * ACols, ribbon_width, MPI_INT,
                  ribbon + remainder + i * new_width, ribbon_width, MPI_INT, 0,
                  MPI_COMM_WORLD);
      for (int j = 0; j < remainder; j++) {
        ribbon[j + i * new_width] = A[j + i * ACols];
      }
    }
  } else {
    for (int i = 0; i < ARows; i++) {
      MPI_Scatter(A + remainder + i * ACols, ribbon_width, MPI_INT,
                  ribbon + i * ribbon_width, ribbon_width, MPI_INT, 0,
                  MPI_COMM_WORLD);
    }
  }
  if (rank == 0) {
    SequentialMatrixMultiplication(ribbon, ARows, new_width, B, new_width,
                                   local_vector);
  } else {
    int* new_B = new int[ribbon_width];
    std::copy(B + ribbon_width * rank + remainder,
              B + ribbon_width * rank + remainder + ribbon_width, new_B);
    SequentialMatrixMultiplication(ribbon, ARows, ribbon_width, new_B,
                                   ribbon_width, local_vector);
  }
  MPI_Reduce(local_vector, C, ARows, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
}
