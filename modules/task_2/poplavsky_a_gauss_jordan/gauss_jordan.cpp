// Copyright 2021 Poplavsky Anton
#include "../../../modules/task_2/poplavsky_a_gauss_jordan/gauss_jordan.h"

double* GaussJordan(int count_rows, int* rows, int size, double* A,
                    double* right_col, double* current) {
  int ProcRank, ProcNum;
  int iterator = 0;
  double div;
  for (int i = 0; i < size; i++) {
    right_col[i] = 0;
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  for (int i = 0; i < size - 1; i++) {
    if (rows[iterator] == i) {
      for (int j = 0; j < size + 1; j++) {
        current[j] = A[iterator * (size + 1) + j];
      }
      MPI_Bcast(&A[iterator * (size + 1)], size + 1, MPI_DOUBLE, i % ProcNum,
                MPI_COMM_WORLD);
      iterator++;
    } else {
      MPI_Bcast(current, size + 1, MPI_DOUBLE, i % ProcNum, MPI_COMM_WORLD);
    }

    for (int j = iterator; j < count_rows; j++) {
      div = A[j * (size + 1) + i] / current[i];
      for (int k = i; k < size + 1; k++) {
        A[j * (size + 1) + k] = A[j * (size + 1) + k] - div * current[k];
      }
    }
  }

  iterator = 0;
  for (int i = 0; i < size; i++) {
    if (rows[iterator] == i) {
      right_col[i] = A[iterator * (size + 1) + size];
      iterator++;
    }
  }

  iterator = count_rows - 1;
  for (int i = size - 1; i > 0; i--) {
    if (iterator >= 0) {
      if (rows[iterator] == i) {
        right_col[i] = right_col[i] / A[iterator * (size + 1) + i];
        MPI_Bcast(&right_col[i], 1, MPI_DOUBLE, i % ProcNum, MPI_COMM_WORLD);
        iterator--;
      } else {
        MPI_Bcast(&right_col[i], 1, MPI_DOUBLE, i % ProcNum, MPI_COMM_WORLD);
      }
    } else {
      MPI_Bcast(&right_col[i], 1, MPI_DOUBLE, i % ProcNum, MPI_COMM_WORLD);
    }

    for (int j = 0; j < iterator + 1; j++) {
      right_col[rows[j]] =
          right_col[rows[j]] - A[j * (size + 1) + i] * right_col[i];
    }
  }

  if (ProcRank == 0) {
    right_col[0] = right_col[0] / A[iterator * (size + 1)];
  }

  MPI_Bcast(right_col, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return right_col;
}
