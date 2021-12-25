// Copyright 2021 Abdullin Konstantin
#include <mpi.h>
#include <random>
#include "../../../modules/task_1/abdullin_k_min_col_matrix/min_columns_matrix.h"

int* random_matrix(int n, int m) {
  if (n <= 0 || m <= 0) throw - 1;
  int* array;
  array = new int[n*m];

  std::random_device r;
  std::mt19937 gen(r());

  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      array[i * m + j] = gen() % 100;

  return array;
}

int array_min(int* array, int n) {
  if (n <= 0) throw - 1;
  int min = array[0];

  for (int i = 1; i < n; i++)
  if (array[i] < min)
    min = array[i];

  return min;
}

int* sequential_min(int* matrix, int n, int m) {
  if (n <= 0 || m <= 0) throw - 1;
  int* result;
  result = new int[m];
  for (int i = 0; i < m; i++)
    result[i] = matrix[i];

  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      if (result[i] > matrix[j * m + i])
        result[i] = matrix[j * m + i];

  return result;
}

const int Tag = 0;
const int root = 0;

int* parallel_min(int* matrix, int n, int m) {
  if (n <= 0 || m <= 0) throw - 1;
  int rank, commSize;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);

  MPI_Status status;

  int* other_matrix, * result, * tmp, * buffer, * bufSize;
  bufSize = new int;
  result = new int[m];

  other_matrix = new int[n*m];

  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      other_matrix[j * n + i] = matrix[i * m + j];

  int partSize = m / commSize;
  int shift = m % commSize;
  buffer = new int[partSize];

  if (root == rank) {
    for (int i = root + 1; i < commSize; i++)
      MPI_Send(other_matrix + (shift + i * partSize) * n, n * partSize,
        MPI_INT, i, Tag, MPI_COMM_WORLD);

    for (int i = 0; i < shift + partSize; i++)
      result[i] = array_min(other_matrix + i * n, n);

    for (int i = root + 1; i < commSize; i++) {
      MPI_Recv(buffer, partSize, MPI_INT, i, Tag, MPI_COMM_WORLD, &status);
      for (int j = 0; j < partSize; j++)
        result[shift + i * partSize + j] = buffer[j];
    }
  }

  if (rank != root) {
    MPI_Probe(root, Tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, bufSize);

    tmp = new int[*bufSize];
    MPI_Recv(tmp, *bufSize, MPI_INT, root, Tag, MPI_COMM_WORLD, &status);

    for (int i = 0; i < partSize; i++)
      buffer[i] = array_min(tmp + i * n, n);

    MPI_Send(buffer, partSize, MPI_INT, root, Tag, MPI_COMM_WORLD);
  }

  return result;
}
