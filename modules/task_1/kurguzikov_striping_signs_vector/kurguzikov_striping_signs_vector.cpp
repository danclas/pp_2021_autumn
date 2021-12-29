// Copyright 2021 Kurguzikov Kirill

#include "../../../modules/task_1/kurguzikov_striping_signs_vector/kurguzikov_striping_signs_vector.h"
#include <mpi.h>
#include <random>

int sequentialCount(int* vect, int size) {
  int count = 0;

  if (size > 0) {
    for (int i = 1; i < size; i++)
      if ((vect[i - 1] > 0 && vect[i] < 0) ||
        (vect[i - 1] < 0 && vect[i] > 0))
        count++;
  }
  return count;
}

int* getRandomValue(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-100, 100);

    if (size < 0) { size = 0; }
    int* vect = new int[size];
    for (int i = 0; i < size; i++) { vect[i] = dist(gen); }

    return vect;
}

int parallelCount(int* vect, int size) {
  int procCount = 0, procRank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &procCount);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  int global_res = 0;
  int local_res = 0;
  int size_vect = size;
  MPI_Bcast(&size_vect, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int* sendcounts = new int[procCount];
  int* displs = new int[procCount];

  int len = size_vect / procCount;
  int rem = size_vect % procCount;

  sendcounts[0] = len + rem;
  displs[0] = 0;

  for (int i = 1; i < procCount; i++) {
    sendcounts[i] = len + 1;
    displs[i] = displs[i - 1] + sendcounts[i - 1] - 1;
  }

  int* local_vect = new int[sendcounts[procRank]];
  MPI_Scatterv(&vect[0], sendcounts, displs, MPI_INT, &local_vect[0],
    sendcounts[procRank], MPI_INT, 0, MPI_COMM_WORLD);
  local_res = sequentialCount(local_vect, sendcounts[procRank]);
  MPI_Reduce(&local_res, &global_res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  delete[] sendcounts;
  delete[] displs;
  delete[] local_vect;

  return global_res;
}
