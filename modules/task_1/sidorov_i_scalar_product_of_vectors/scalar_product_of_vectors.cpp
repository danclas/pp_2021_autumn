// Copyright 2021 Sidorov Ilya
#include "../../../modules/task_1/sidorov_i_scalar_product_of_vectors/scalar_product_of_vectors.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

int* getRandomVector(int n, int interval) {
  std::random_device dev;
  std::mt19937 gen(dev());
  int* arr = new int[n];
  for (int i = 0; i < n; i++) arr[i] = gen() % interval;
  return arr;
}

int scalar_product(int* arr1, int* arr2, int n) {
  int product = 0;
  for (int i = 0; i < n; i++) product += arr1[i] * arr2[i];
  return product;
}

int parallCulc(int* arr1, int* arr2, int n) {
  int global_product = 0;
  int local_product = 0;
  int size = 0;
  int delta = 0;
  int* buffer1 = nullptr;
  int* buffer2 = nullptr;
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if (world_rank == 0) {
    size = n / world_size;
    delta = n % world_size;
  }

  MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  buffer1 = new int[size];
  buffer2 = new int[size];

  MPI_Scatter(arr1 + delta, size, MPI_INT, buffer1, size, MPI_INT, 0,
              MPI_COMM_WORLD);
  MPI_Scatter(arr2 + delta, size, MPI_INT, buffer2, size, MPI_INT, 0,
              MPI_COMM_WORLD);
  local_product = scalar_product(buffer1, buffer2, size);

  MPI_Reduce(&local_product, &global_product, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  if (world_rank == 0) {
    for (int i = 0; i < delta; i++) global_product += arr1[i] * arr2[i];
  }
  return global_product;
}
