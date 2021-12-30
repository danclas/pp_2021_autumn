// Copyright 2021 Ivina Anastasiya
#include <mpi.h>
#include <vector>
#include <queue>
#include <random>
#include "../../../modules/task_3/ivina_a_alg_moore/alg_moore.h"


int randomValue(int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> range(min, max);
  return range(rd);
}

std::vector<std::vector<int>> par_shortestPathFaster(
    const std::vector<std::vector<std::pair<int, int>>> &graph, const int S,
    const int V) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> res(V + 1);
  std::vector<bool> Ver_queue(V + 1);

  res[S] = 0;

  std::queue<int> vertexQ;
  vertexQ.push(S);
  Ver_queue[S] = true;
  std::vector<std::vector<int>> buf;
  int step_counter = 0;

  if ((size == 1) || (rank != 0)) {
    while (!vertexQ.empty() && step_counter < 100) {
      step_counter++;

      int up = vertexQ.front();
      vertexQ.pop();
      Ver_queue[up] = false;

      for (int i = 0; i < static_cast<int>(graph[up].size()); i++) {
        int currentVer = graph[up][i].first;
        int weight = graph[up][i].second;
        if (res[currentVer] > res[up] + weight) {
          res[currentVer] = res[up] + weight;

          if (!Ver_queue[currentVer]) {
            vertexQ.push(currentVer);
            Ver_queue[currentVer] = true;
          }
        }
      }
    }
    if (size == 1) {
      buf.push_back(res);
    } else {
      MPI_Send(res.data(), res.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  } else {
    for (int i = 0; i < size - 1; i++) {
      std::vector<int> temp;

      for (int j = 0; j < V + 1; j++) {
        temp.push_back(0);
      }
      MPI_Recv(temp.data(), temp.size(), MPI_INT, MPI_ANY_SOURCE, 0,
               MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
      buf.push_back(temp);
    }
  }
  return buf;
}
