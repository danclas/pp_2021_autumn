// Copyright 2021 Kostin Maksim
#include "../../../modules/task_2/kostin_m_ribbon_horizontal_scheme/ribbon_horizontal_scheme.h"

#include <mpi.h>

#include <random>
#include <string>
#include <vector>

std::vector<int> getRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = gen() % 100;
  }
  return vec;
}

std::vector<int> getRandomMatrix(int row, int column) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> matrix(row * column);
  for (int i = 0; i < row * column; i++) {
    matrix[i] = gen() % 100;
  }
  return matrix;
}

int* VecToInt(int* arr, std::vector<int> vec, int sz) {
  for (int i = 0; i < sz; i++) {
    arr[i] = vec[i];
  }
  return arr;
}

std::vector<int> IntToVec(std::vector<int> vec, int* arr, int sz) {
  for (int i = 0; i < sz; i++) {
    vec[i] = arr[i];
  }
  return vec;
}

std::vector<int> SequentialMatrixMultiplication(std::vector<int> A, int row_a,
  int col_a, std::vector<int> B, int row_b, int col_b) {
  int tmp;
  if (col_a != row_b) {
    throw -1;
  }
  std::vector<int> C(row_a * col_b);
  for (int i = 0; i < row_a; i++) {
    for (int j = 0; j < col_b; j++) {
      tmp = 0;
      for (int k = 0; k < col_a; k++) {
        tmp += A[i * col_a + k] * B[k * col_b + j];
      }
      C[i * col_b + j] = tmp;
    }
  }
  return C;
}

std::vector<int> ParallelMatrixMultiplication(std::vector<int> A, int row_a,
  int col_a, std::vector<int> B, int row_b, int col_b) {
  if (col_a != row_b) {
    throw -1;
  }

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int rib_width = row_a / size;
  const int rem = row_a % size;
  int* rib = nullptr;
  int* local_sum = nullptr;
  int* B_tmp = new int[col_b * row_b];

  if (rank == 0) {
    for (int k = 0; k < row_b * col_b; k++) {
      int i = k / row_b;
      int j = k % row_b;
      B_tmp[k] = B[col_b * j + i];
    }
  }

  MPI_Bcast(B_tmp, col_b * row_b, MPI_INT, 0, MPI_COMM_WORLD);

  rib = new int[col_a * rib_width];
  local_sum = new int[col_b * rib_width];

  int* A_tmp = new int[row_a * col_a];
  A_tmp = VecToInt(A_tmp, A, row_a * col_a);

  MPI_Scatter(A_tmp, rib_width * col_a, MPI_INT, rib, rib_width * col_a,
              MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < rib_width; i++) {
    for (int j = 0; j < col_b; j++) {
      int tmp = 0;
      for (int k = 0; k < col_a; k++) {
        tmp += rib[i * col_a + k] * B_tmp[j * row_b + k];
      }
      local_sum[i * col_b + j] = tmp;
    }
  }

  int* C_tmp = new int[col_b * row_a];

  MPI_Gather(local_sum, col_b * rib_width, MPI_INT, C_tmp,
             col_b * rib_width, MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<int> C(col_b * row_a);
  C = IntToVec(C, C_tmp, col_b * row_a);
  if (rem) {
    if (rank == 0) {
      for (int i = rib_width * size;
           i < rib_width * size + rem; i++) {
        for (int j = 0; j < col_b; j++) {
          int tmp = 0;
          for (int k = 0; k < col_a; k++) {
            tmp += A[i * col_a + k] * B_tmp[j * row_b + k];
          }
          C[i * col_b + j] = tmp;
        }
      }
    }
  }

  delete[] A_tmp;
  delete[] B_tmp;
  delete[] C_tmp;
  delete[] rib;
  delete[] local_sum;

  return C;
}
