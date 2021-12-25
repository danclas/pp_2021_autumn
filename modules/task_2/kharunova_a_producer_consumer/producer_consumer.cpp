// Copyright 2021 Kharunova Alina
#include <mpi.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <queue>
#include <random>
#include <string>

#include "../../../modules/task_2/kharunova_a_producer_consumer/producer_consumer.h"

double waitingTime = 0.01;

void waiting(double seconds) {
  double time = MPI_Wtime();
  while (MPI_Wtime() - time < seconds) {
  }
}

int process(int numberOfElements) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Status status;
  MPI_Request request;
  int data;
  int emptyConsumer = -1;

  std::queue<int> buffer;
  while (numberOfElements != 0) {
    if (emptyConsumer == -1) {
      MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);
    } else {
      MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);
    }
    --numberOfElements;
    if (status.MPI_TAG == 3) {
      buffer.push(data);
      if (emptyConsumer != -1) {
        int el = buffer.front();
        buffer.pop();
        MPI_Isend(&el, 1, MPI_INT, emptyConsumer, 4, MPI_COMM_WORLD, &request);
        MPI_Request_free(&request);
        emptyConsumer = -1;
      }
    } else {
      if (!buffer.empty()) {
        int element = buffer.front();
        buffer.pop();
        MPI_Isend(&element, 1, MPI_INT, status.MPI_SOURCE, 4, MPI_COMM_WORLD,
                  &request);
        MPI_Request_free(&request);
      } else {
        emptyConsumer = status.MPI_SOURCE;
      }
    }
  }
  return buffer.size();
}

void producer() {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  waiting(waitingTime * rank);
  MPI_Send(&rank, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
}

void consumer() {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  waiting(waitingTime * rank);
  MPI_Send(&rank, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);

  int element;
  MPI_Status status;
  MPI_Recv(&element, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
}
