// Copyright 2021 Alimov Mikhail
#include "../../../modules/task_1/alimov_m_ne_poradok_vector/ne_poradok_vector.h"
#include <mpi.h>
#include <time.h>
#include <algorithm>
#include <random>
#include <vector>

void Rand(int* vec, int n) {
  for (int i = 0; i < n; i++) {
    std::mt19937 gen(time(0));
    vec[i] = gen() % 100;
  }
}

int getNumNonMonotonyElems(int* input_vec, int size) {
  int count = 0;
  for (int i = 1; i < size; i++)
    if ((input_vec[i] - input_vec[i - 1]) < 0) count++;
  return count;
}

int fun(int* a, const int n) {
  int* sendcounts = nullptr;
  int* displs = nullptr;
  int ProcNum, ProcRank, n1 = 0, sum = 0;
  int tmp = 0, count = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcRank == 0) {
    sendcounts = new int[ProcNum];
    displs = new int[ProcNum];
    int ost = n % ProcNum;
    for (int i = 0; i < ProcNum; i++) {
      sendcounts[i] = (n / ProcNum) + 1;
      if (ost > 0) {
        sendcounts[i] += 1;
        ost--;
      }
      displs[i] = sum - i;
      sum += sendcounts[i];
      if (sum - i > n) sendcounts[i]--;
    }
  }

  MPI_Scatter(sendcounts, 1, MPI_INT, &n1, 1, MPI_INT, 0, MPI_COMM_WORLD);
  int* vec2 = new int[n1];

  MPI_Scatterv(a, sendcounts, displs, MPI_INT, vec2, n1, MPI_INT, 0,
               MPI_COMM_WORLD);

  tmp = getNumNonMonotonyElems(vec2, n1);

  MPI_Reduce(&tmp, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (ProcRank == 0) {
    delete[] displs;
    delete[] sendcounts;
  }
  return count;
}
