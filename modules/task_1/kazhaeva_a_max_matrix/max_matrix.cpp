// Copyright 2021 Kazhaeva Anastasia
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/kazhaeva_a_max_matrix/max_matrix.h"


void Rand(int* matrix, int rows, int columns) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      std::mt19937 gen(time(0));
      matrix[i * columns + j] = gen() % 100;
    }
  }
}

int* CalcMaxNumber(int* matrix, int row, int col) {
  int* max = new int[col];
  for (int i = 0; i < col; i++) max[i] = 0;
  for (int i = 0; i < col; i++)
    for (int j = 0; j < row; j++)
      if (matrix[j * col + i] > max[i]) max[i] = matrix[j * col + i];
  return max;
}

int* Max_Matrix(int* a, const int A_row, const int A_col) {
  int* sendcounts = nullptr;
  int* displs = nullptr;
  int ProcNum, ProcRank, cim = 0;
  int* max, * max_m;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  max_m = new int[A_col];
  if (ProcRank == 0) {
    sendcounts = new int[ProcNum];
    displs = new int[ProcNum];
    int ost = A_row % ProcNum;
    int sum = 0;
    for (int i = 0; i < ProcNum; i++) {
      sendcounts[i] = (A_row / ProcNum) * A_col;
      if (ost > 0) {
        sendcounts[i] += A_col;
        ost--;
      }
      displs[i] = sum;
      sum += sendcounts[i];
    }
  }
  MPI_Scatter(sendcounts, 1, MPI_INT,
    &cim, 1, MPI_INT, 0, MPI_COMM_WORLD);
  int* b = new int[cim];
  MPI_Scatterv(a, sendcounts, displs, MPI_INT,
    b, cim, MPI_INT, 0, MPI_COMM_WORLD);
  max = CalcMaxNumber(b, cim / A_col, A_col);
  MPI_Reduce(max, max_m, A_col, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  if (ProcRank == 0) {
    delete[] displs;
    delete[] sendcounts;
  }
  return max_m;
}
