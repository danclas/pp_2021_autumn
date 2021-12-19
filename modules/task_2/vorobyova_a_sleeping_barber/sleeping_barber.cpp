// Copyright 2021 Vorobyova Anna
#include "../../../modules/task_2/vorobyova_a_sleeping_barber/sleeping_barber.h"

#include <mpi.h>

#include <queue>
#include <random>

double getRandomTime() {
  std::random_device rd;
  std::mt19937 gen(rd());
  return (10.0 + static_cast<double>(gen() % 20u)) / 100.0;
}

void wait(double s) {
  double start = MPI_Wtime();
  while (MPI_Wtime() - start < s) {
  }
}

void Barber(int line_length, int to_serve) {
  std::queue<int> line;
  int served_clients = 0;

  while (served_clients != to_serve) {
    int got_clients = 0;
    int free = 0;
    MPI_Status status;

    MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &got_clients, &status);
    int client_rank;

    while (got_clients) {
      MPI_Recv(&client_rank, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
               &status);

      if (static_cast<int>(line.size()) < line_length) {
        line.push(client_rank);
        free = 1;
        MPI_Send(&free, 1, MPI_INT, client_rank, 0, MPI_COMM_WORLD);
      } else {
        free = 0;
        MPI_Send(&free, 1, MPI_INT, client_rank, 0, MPI_COMM_WORLD);
      }

      MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &got_clients, &status);
    }

    if (!line.empty()) {
      client_rank = line.front();
      line.pop();

      int done = 0;
      MPI_Send(&done, 1, MPI_INT, client_rank, 0, MPI_COMM_WORLD);
      wait(0.01);

      done = 1;
      MPI_Send(&done, 1, MPI_INT, client_rank, 0, MPI_COMM_WORLD);
      served_clients++;
    }
  }
}

void Client(int rank) {
  wait(getRandomTime());

  MPI_Status status;
  int done = 0;
  while (done != 1) {
    MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    int free = 0;
    MPI_Recv(&free, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    if (free) {
      MPI_Recv(&free, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      MPI_Recv(&done, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    } else {
      wait(getRandomTime());
    }
  }
}
