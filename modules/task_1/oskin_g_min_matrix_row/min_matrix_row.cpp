// Copyright 2021 Oskin Georgii
#include "../../../modules/task_1/oskin_g_min_matrix_row/min_matrix_row.h"

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<std::vector<int>> getRandMatrix(int m, int n) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<std::vector<int>> matrix(m, std::vector<int>(n));
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i][j] = gen() % 100;
    }
  }
  return matrix;
}

int getLocalMinimum(std::vector<int> vec, int n) {
  int min;
  min = vec[0];
  if (vec.size() > 1) {
    for (int i = 0; i < n; i++) {
      if (vec[i] < min) min = vec[i];
    }
  }

  return min;
}

void findMinNonParallel(std::vector<std::vector<int>> mat, int m, int n,
                        std::vector<int> result) {
  for (int i = 0; i < m; i++) {
    result[i] = getLocalMinimum(mat[i], n);
  }
}

void getParallelOperations(std::vector<std::vector<int>> g_matrix, int m, int n,
                           std::vector<int> res) {
  int size, rank;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int vec_size = floor(m / size - 1);
  int temp_rows = 0;
  int result = 0;
  MPI_Comm newworld;
  MPI_Comm_dup(MPI_COMM_WORLD, &newworld);
  if (size == 1) {
    findMinNonParallel(g_matrix, m, n, res);
  }
  if (size >= m) {
    int size_n;
    if (rank == 0) {
      int rank, size;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      MPI_Comm_size(MPI_COMM_WORLD, &size);
      MPI_Group world_group;
      MPI_Comm_group(MPI_COMM_WORLD, &world_group);
      MPI_Group new_group;
      int starter = m;
      int ranges[3] = {starter, size - 1, 1};
      MPI_Group_range_excl(world_group, 1, &ranges, &new_group);
      MPI_Comm_create(MPI_COMM_WORLD, new_group, &newworld);
      MPI_Comm_size(newworld, &size_n);
      if (newworld == MPI_COMM_NULL) {
        MPI_Finalize();
        exit(0);
      }
    }
    if (rank <= m - 1) {
      res[rank] = getLocalMinimum(g_matrix[rank], n);
    }
  }
  if (size < m) {
    if (size != 1) {
      if (rank != 0) {
        if (rank != size - 1) {
          for (int i = (rank - 1) * vec_size; i < vec_size * rank; i++) {
            temp_rows = getLocalMinimum(g_matrix[i], n);
            MPI_Send(&temp_rows, 1, MPI_INT, 0, 0, newworld);
          }
        }
      }
      if (rank == size - 1) {
        for (int i = (rank - 1) * vec_size; i < m; i++) {
          temp_rows = getLocalMinimum(g_matrix[i], n);
          MPI_Send(&temp_rows, 1, MPI_INT, 0, 0, newworld);
        }
      }

      if (rank == 0) {
        for (int i = 1; i < size; i++) {
          if (i != size - 1) {
            for (int j = (i - 1) * vec_size; j < i * vec_size; j++) {
              MPI_Recv(&result, 1, MPI_INT, i, 0, newworld, &status);
              res[j] = result;
            }
          } else {
            for (int j = (i - 1) * vec_size; j < m; j++) {
              MPI_Recv(&result, 1, MPI_INT, i, 0, newworld, &status);
              res[j] = result;
            }
          }
        }
      }
    }
  }
}
