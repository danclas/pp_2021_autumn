// Copyright 2021 Ivina Anastasiya
#include <mpi.h>
#include <vector>
#include <random>
#include "../../../modules/task_1/ivina_a_vector_out_of_order/vector_out_of_order.h"


std::vector<int> randomValues(int len) {
  std::vector<int> vec;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 1000);
  if (len > 0) {
    for (int i = 0; i < len; i++) {
      vec.push_back(dis(rd));
    }
  }
  return vec;
}

int countViolation(const std::vector<int> &vec) {
  int count = 0;
  if (!vec.empty()) {
    for (int i = 0; i < static_cast<int>(vec.size()) - 1; i++) {
      if (vec[i] > vec[i + 1])
        count++;
    }
    return count;
  }
  return 0;
}

int parallelCountViolation(const std::vector<int> &vec) {
  int Size = 0, Rank = 0;
  int count = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &Size);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  int len = vec.size();
  if (len < 2) {
    return 0;
  }

  int block = len / Size;
  int sum = 0;
  std::vector<int> scounts;
  std::vector<int> displs;
  std::vector<int> dataReceived;

  for (int i = 0; i < Size; i++) {
    scounts.push_back(len / Size + 1);
    displs.push_back(sum);
    sum += len / Size;
    if (i == (Size - 1)) {
      scounts[i] = (len / Size + len % Size);
    }
  }
  if (Rank == (Size - 1)) {
    block = len / Size + len % Size;
    dataReceived.resize(block);
  } else {
    block = len / Size + 1;
  }
  dataReceived.resize(block);

  MPI_Scatterv(vec.data(), scounts.data(), displs.data(), MPI_INT,
               dataReceived.data(), block, MPI_INT, 0, MPI_COMM_WORLD);

  count = countViolation(dataReceived);
  int glcount = 0;
  MPI_Reduce(&count, &glcount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return glcount;
}
