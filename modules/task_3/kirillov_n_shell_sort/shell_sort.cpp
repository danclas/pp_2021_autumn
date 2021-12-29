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
  for (int step = size / 2; step > 0; step /= 2)
    for (int i = step; i < size; i++) {
      int tmp = vec[i];
      int j;
      for (j = i; j >= step; j -= step) {
        if (tmp < vec[j - step])
          vec[j] = vec[j - step];
        else
          break;
      }
      vec[j] = tmp;
    }
}

void getParallelOperations(int* vec, int size_vec) {
  if (size_vec < 1) throw -1;
  int rank, size;
  int* res;
  int* _vec;
  int step = 1;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int local_size = size_vec / size;
  int remains = size_vec % size;

  if (rank == 0) {
    if (remains != 0) {
      vec = new int[size_vec + size - remains];
      for (int i = size_vec; i < size_vec + size - remains; i++) vec[i] = 0;
      local_size = local_size + 1;
    }

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

  while (step < size) {
    if (rank % (2 * step) == 0) {
      if (rank + step < size) {
        MPI_Recv(&size_vec, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD,
                 &status);
        _vec = new int[size_vec];
        MPI_Recv(_vec, size_vec, MPI_INT, rank + step, 0, MPI_COMM_WORLD,
                 &status);
        int* _res = res;
        res = new int[local_size + size_vec];
        int i = 0, j = 0, ij = 0;
        while (i < local_size && j < size_vec)
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
          while (j < size_vec) {
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
        local_size = local_size + size_vec;
      }
    } else {
      MPI_Send(&local_size, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
      MPI_Send(res, local_size, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
      break;
    }
    step = step * 2;
  }
  if (rank == 0) {
    vec = res;
    if (size_vec % size) {
      delete[] vec;
      vec = new int[size_vec];
      vec = res;
    }
  }
}
