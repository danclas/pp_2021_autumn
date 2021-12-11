// Copyright 2021 Feoktistov Andrey
#include "../feoktistov_a_vector_avg/vector_avg.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

std::vector<int> generateRandomPositiveVector(int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(30, 70);

  if (size < 0) {
    size = 0;
  }
  std::vector<int> vec(size);
  for (int i = 0; i < size; i++) {
    vec[i] = dist(gen);
  }
  return vec;
}

int calcSequentialAverage(std::vector<int> nonparalllel_vec) {
  int size = nonparalllel_vec.size(), sum_all = 0;
  float averageVal = 0;

  if (size <= 0) {
    return 0;
  }

  for (int i = 0; i < size; i++) {
    sum_all += nonparalllel_vec[i];
  }

  averageVal = static_cast<float>(sum_all) / size;
  return averageVal;
}

int calcParallelAverage(std::vector<int> parallel_vec, int size) {
  int ProcessRank, ProcessNum, sum_all;
  float averageVal;

  if (size <= 0) {
    return 0;
  }

  MPI_Comm_size(MPI_COMM_WORLD, &ProcessNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcessRank);

  int partSize = size / ProcessNum;
  int offset = size % ProcessNum;
  if (ProcessRank == 0) {
    for (int i = 1; i < ProcessNum; i++) {
      MPI_Send(parallel_vec.data() + offset + (i * partSize), partSize, MPI_INT,
               i, 0, MPI_COMM_WORLD);
    }
  }

  std::vector<int> local_vec(partSize);
  if (ProcessRank == 0) {
    local_vec = std::vector<int>(parallel_vec.begin(),
                                 parallel_vec.begin() + partSize + offset);
  } else {
    MPI_Status status;
    MPI_Recv(local_vec.data(), partSize, MPI_INT, 0, 0, MPI_COMM_WORLD,
             &status);
  }

  int sum = sum_all = 0;
  if (ProcessRank != 0) {
    offset = 0;
  }
  for (int i = 0; i < (partSize + offset); i++) sum += local_vec[i];

  MPI_Reduce(&sum, &sum_all, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  averageVal = static_cast<float>(sum_all) / size;
  return averageVal;
}
