// Copyright 2021 Shabunin Vladislav
#include "../../../modules/task_2/shabunin_v_sleeping_barber/sleeping_barber.h"

#include <mpi.h>

#include <chrono>  // NOLINT [build/c++11]
#include <ctime>
#include <random>
#include <thread>  // NOLINT [build/c++11]

void wait() {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::this_thread::sleep_for(std::chrono::milliseconds(10 + gen() % 10));
}

void client(int rank) {
  int in[1] = {-1};
  int out[1] = {rank};
  MPI_Status status;

  while (in[0] < 0) {
    wait();
    MPI_Send(out, 1, MPI_INT, 1, SEAT_REQUEST, MPI_COMM_WORLD);
    MPI_Recv(in, 1, MPI_INT, 1, SEAT_RESPONSE, MPI_COMM_WORLD, &status);
    if (in[0] >= 0) {
      MPI_Send(out, 1, MPI_INT, 1, HAIRCUT_REQUEST, MPI_COMM_WORLD);
    }
  }
}
void queue(int seat_count, int runs) {
  int in[1];
  int out[1];
  int queue_length = 0;
  int* chairs = new int[seat_count];
  MPI_Status status;
  for (int i = 0; i < runs; i++) {
    MPI_Recv(in, 1, MPI_INT, MPI_ANY_SOURCE, SEAT_REQUEST, MPI_COMM_WORLD,
             &status);
    if (queue_length < seat_count) {
      out[0] = queue_length;
      MPI_Send(out, 1, MPI_INT, in[0], SEAT_RESPONSE, MPI_COMM_WORLD);

      queue_length++;
      MPI_Recv(chairs + queue_length, 1, MPI_INT, in[0], HAIRCUT_REQUEST,
               MPI_COMM_WORLD, &status);
    } else {
      out[0] = -1;
      MPI_Send(out, 1, MPI_INT, in[0], SEAT_RESPONSE, MPI_COMM_WORLD);
    }
    if (queue_length > 0) {
      MPI_Send(&queue_length, 1, MPI_INT, 0, HAIRCUT, MPI_COMM_WORLD);
      queue_length = 0;
    }
  }
}

void barber(int rank, int client_count) {
  int in[1];
  MPI_Status status;

  for (int i = 0; i < client_count; i++) {
    MPI_Recv(in, 1, MPI_INT, 1, HAIRCUT, MPI_COMM_WORLD, &status);
    wait();
  }
}
