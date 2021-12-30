// Copyright 2021 Kirillov Nikita
#include "../../../modules/task_1/kirillov_n_min_value_in_line_matrix/min_value_in_line_matrix.h"

#include <mpi.h>

#include <algorithm>
#include <random>

int** getRandomMatrix(int size_w, int size_h) {
  if (size_h < 0 || size_w < 0) throw -1;
  int** matrix = new int*[size_h];
  for (int i = 0; i < size_h; i++) {
    matrix[i] = new int[size_w];
  }
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < size_h; i++) {
    for (int j = 0; j < size_w; j++) {
      matrix[i][j] = gen() % 100;
    }
  }
  return matrix;
}

int* getSequentialOperations(int** matrix, int width, int height) {
  if (height < 0 || width < 0) throw -1;
  int* res = new int[height];
  for (int i = 0; i < height; i++) {
    int _min = matrix[i][0];
    for (int j = 0; j < width; j++) {
      _min = std::min(_min, matrix[i][j]);
    }
    res[i] = _min;
  }
  return res;
}

int* getParallelOperations(int* global_matrix, int width, int height) {
  if (height < 1 || width < 1) throw -1;

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int local_size = height / size;
  int remains = height % size;

  if (rank == 0) {
    for (int proc = 1; proc < size; ++proc) {
      int ofset = proc * local_size * width + remains * width;
      MPI_Send(global_matrix + ofset, local_size * width, MPI_INT, proc, 1,
               MPI_COMM_WORLD);
    }
  }

  if (rank == 0) local_size += remains;
  int* local_matrix = new int[local_size * width];

  if (rank == 0) {
    for (int i = 0; i < local_size * width; i++) {
      local_matrix[i] = global_matrix[i];
    }
  } else {
    MPI_Status status;
    MPI_Recv(local_matrix, local_size * width, MPI_INT, 0, 1, MPI_COMM_WORLD,
             &status);
  }

  int* local_res = new int[height];
  for (int i = 0; i < height; i++) local_res[i] = 0;
  for (int i = 0; i < local_size; i++) {
    int _min = local_matrix[i * width];
    for (int j = 0; j < width; j++) {
      _min = std::min(_min, local_matrix[i * width + j]);
    }
    int ind;
    if (rank == 0)
      ind = i;
    else
      ind = remains + local_size * rank + i;
    local_res[ind] = _min;
  }

  int* global_res = new int[height];
  MPI_Reduce(local_res, global_res, height, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);
  return global_res;
}
