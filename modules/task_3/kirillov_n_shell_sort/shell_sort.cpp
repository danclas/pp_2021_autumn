// Copyright 2021 Kirillov Nikita
#include "../../../modules/task_3/kirillov_n_shell_sort/shell_sort.h"

#include <mpi.h>

#include <algorithm>
#include <random>

void getRandomVector(int* vec, int size) {
  if (size < 1) throw -1;
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < size; i++) vec[i] = gen() % 100;
}

void getSequentialOperations(int* vec, int size) {
  if (size < 1) throw -1;
  for (int offset = size / 2; offset > 0; offset /= 2) {
    for (int i = offset; i < size; i++) {
      int tmp = vec[i];
      int j;
      for (j = i; j >= offset; j -= offset) {
        if (tmp < vec[j - offset])
          vec[j] = vec[j - offset];
        else
          break;
      }
      vec[j] = tmp;
    }
  }
}

void getParallelOperations(int* vec, int size_vec) {
  if (size_vec < 1) throw -1;
  int rank, size;
  int* res = 0;
  int* _vec = 0;
  int offset = 1;
  int size_n = size_vec, size_m = size_vec;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int local_size = size_n / size;

  if (rank == 0) {
    MPI_Bcast(&local_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    res = new int[local_size];
    MPI_Scatter(vec, local_size, MPI_INT, res, local_size, MPI_INT, 0,
                MPI_COMM_WORLD);
    getSequentialOperations(res, local_size);
  } else {
    MPI_Bcast(&local_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    res = new int[local_size];
    MPI_Scatter(vec, local_size, MPI_INT, res, local_size, MPI_INT, 0,
                MPI_COMM_WORLD);
    getSequentialOperations(res, local_size);
  }
  while (offset < size) {
    if (rank % (2 * offset) == 0) {
      if (rank + offset < size) {
        MPI_Recv(&size_m, 1, MPI_INT, rank + offset, 0, MPI_COMM_WORLD,
                 &status);
        _vec = new int[size_m];
        MPI_Recv(_vec, size_m, MPI_INT, rank + offset, 0, MPI_COMM_WORLD,
                 &status);
        int* _res = new int[local_size];
        for (int i = 0; i < local_size; i++) {
          _res[i] = res[i];
        }
        res = new int[local_size + size_m];
        int i = 0, j = 0, ij = 0;
        while (i < local_size && j < size_m)
          if (_res[i] < _vec[j]) {
            res[ij] = _res[i];
            i++;
            ij++;
          } else {
            res[ij] = _vec[j];
            j++;
            ij++;
          }
        if (i == local_size) {
          while (j < size_m) {
            res[ij] = _vec[j];
            j++;
            ij++;
          }
        } else {
          while (i < local_size) {
            res[ij] = _res[i];
            i++;
            ij++;
          }
        }
        delete[] _res;
        local_size = local_size + size_m;
      }
    } else {
      MPI_Send(&local_size, 1, MPI_INT, rank - offset, 0, MPI_COMM_WORLD);
      MPI_Send(res, local_size, MPI_INT, rank - offset, 0, MPI_COMM_WORLD);
      break;
    }
    offset = offset * 2;
  }
  if (rank == 0) {
    for (int i = 0; i < size_vec; i++) {
      vec[i] = res[i];
    }
  }
}
