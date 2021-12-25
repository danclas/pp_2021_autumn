// Copyright 2021 Podovinnikov Artyom
#include "../../../modules/task_2/podovinnikov_a_broadcast/broadcast.h"

#include <mpi.h>

#include <iostream>
#include <random>

int Broadcast(

    void* sendbuf, int count, MPI_Datatype type, int root, MPI_Comm comm

) {
  int commSize, rank;
  MPI_Comm_size(comm, &commSize);
  MPI_Comm_rank(comm, &rank);

  if (rank == root) {
    for (int i = 0; i < commSize; i++) {
      if (i != root) {
        MPI_Send(sendbuf, count, type, i, root, comm);
      }
    }
  }
  if (type == MPI_DOUBLE) {
    if (rank != root) {
      MPI_Status status{};
      MPI_Recv(sendbuf, count, type, root, root, comm, &status);
    }

  } else if (type == MPI_FLOAT) {
    if (rank != root) {
      MPI_Status status{};
      MPI_Recv(sendbuf, count, type, root, root, comm, &status);
    }

  } else if (type == MPI_INT) {
    if (rank != root) {
      MPI_Status status{};

      MPI_Recv(sendbuf, count, type, root, root, comm, &status);
    }
    return 0;
  }
  return 0;
}

int randInt(int min, int max) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<unsigned> d(min, max);
  return d(rng);
}
