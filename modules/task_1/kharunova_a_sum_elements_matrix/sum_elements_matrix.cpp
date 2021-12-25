// Copyright 2021 Kharunova Alina

#include <mpi.h>

#include <random>

#include "../../../modules/task_1/kharunova_a_sum_elements_matrix/sum_elements_matrix.h"

int linSum(int* matrix, int len) {
  int sum = 0;
  if (len > 0) {
    for (int i = 0; i < len; ++i) {
      sum += matrix[i];
    }
  } else {
    throw "wrong size";
  }
  return sum;
}

int* createRandomMatrix(int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  int* matrix = new int[size];
  for (int i = 0; i < size; i++) {
    matrix[i] = gen() % 1000;
  }
  return matrix;
}

int paralSum(int* matrix, int row, int column) {
  int number = 0, rank = 0;
  int part, remain;
  int *buffer = nullptr, *sendCounts = nullptr, *disp = nullptr;
  int sum = 0, allSum = 0;

  if (row <= 0 || column <= 0) {
    throw "wrong size";
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &number);

  if (rank == 0) {
    part = row * column / number;
    remain = row * column % number;
    sendCounts = new int[number];
    disp = new int[number];
    sendCounts[0] = part + remain;
    disp[0] = 0;
    for (int i = 1; i < number; i++) {
      sendCounts[i] = part;
      disp[i] = part * i + remain;
    }
    buffer = new int[part + remain];
  }

  MPI_Bcast(&part, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank != 0) {
    buffer = new int[part];
    sendCounts = new int[number];
  }
  MPI_Bcast(sendCounts, number, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Scatterv(matrix, sendCounts, disp, MPI_INT, buffer, sendCounts[rank],
               MPI_INT, 0, MPI_COMM_WORLD);

  sum = linSum(buffer, sendCounts[rank]);

  MPI_Reduce(&sum, &allSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    delete[] sendCounts;
    delete[] disp;
  }
  delete[] buffer;

  return allSum;
}
