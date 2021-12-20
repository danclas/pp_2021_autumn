// Copyright 2021 Kharunova Alina
#include <mpi.h>

#include <iostream>
#include <ctime>
#include <stack>
#include <random>

#include "../../../modules/task_2/kharunova_a_producer_consumer/producer_consumer.h"

int process(int numberOfEnterings) {
  int element, rank, emptyConsumer = -1;
  MPI_Status proc;
  MPI_Request request;

  std::stack<int> buffer;
  while (numberOfEnterings != 0) {
    if (emptyConsumer == -1) {
      MPI_Recv(&rank, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
               &proc);
    } else {
      MPI_Recv(&rank, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &proc);
    }
    if (rank % 2 == 1) {
      buffer.push(rank);
      if (emptyConsumer != -1) {
        element = buffer.top();
        buffer.pop();
        MPI_Send(&element, 1, MPI_INT, emptyConsumer, 1, MPI_COMM_WORLD);
        emptyConsumer = -1;
      }
    } else {
      if (!buffer.empty()) {
        element = buffer.top();
        buffer.pop();
        MPI_Send(&element, 1, MPI_INT, proc.MPI_SOURCE, 1, MPI_COMM_WORLD);
      } else {
        emptyConsumer = proc.MPI_SOURCE;
      }
    }
    numberOfEnterings--;
  }
  return buffer.size();
}

void producer() {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  waiting();
  MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

void consumer() {
  MPI_Status status;
  int element, rank;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  waiting();
  MPI_Send(&rank, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
  MPI_Recv(&element, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
}

void waiting() {
  std::random_device dev;
  std::mt19937 gen(dev());
  double time = gen() % 1, now = MPI_Wtime();
  while (MPI_Wtime() < now + time) {
  }
}
