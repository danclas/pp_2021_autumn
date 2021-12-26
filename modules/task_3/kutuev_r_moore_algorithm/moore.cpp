// Copyright 2021 Kutuev Roman

#include "../../../modules/task_3/kutuev_r_moore_algorithm/moore.h"

std::vector<int> random(int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<> urd(1, INF);
  std::vector<int> graph(size * size);
  for (int i = 0; i < size; i++) {
    graph[i * size + i] = 0;
    for (int j = i + 1; j < size; j++) {
      graph[i * size + j] = urd(gen);
      graph[j * size + i] = urd(gen);
    }
  }
  return graph;
}

std::vector<int> moore_algorithm(const std::vector<int>& graph, int size) {
  int max_value = graph[0];
  for (int i = 0; i < size * size; i++) {
    if (max_value < graph[i]) {
      max_value = graph[i];
    }
  }
  std::vector<int> marks(size, max_value);
  marks[0] = 0;
  for (int i = 0; i < size - 1; i++) {
    for (int v = 0; v < size; v++) {
      for (int u = 0; u < size; u++) {
        if (marks[v] > marks[u] + graph[u * size + v]) {
          marks[v] = marks[u] + graph[u * size + v];
        }
      }
    }
  }
  return marks;
}

std::vector<int> moore_parallel(const std::vector<int>& graph, int size) {
  int ProcNum, ProcRank;
  int max_value = graph[0];
  for (int i = 0; i < size * size; i++) {
    if (max_value < graph[i]) {
      max_value = graph[i];
    }
  }
  std::vector<int> marks(size, max_value);
  marks[0] = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  std::vector<int> bounds(ProcNum + 1);
  bounds[0] = 0;
  for (int i = 1; i < ProcNum; i++) {
    if (i < size % ProcNum) {
      bounds[i] += size / ProcNum + 1 + bounds[i - 1];
    } else {
      bounds[i] += size / ProcNum + bounds[i - 1];
    }
  }
  bounds[ProcNum] = size;
  for (int i = 0; i < size - 1; i++) {
    for (int v = bounds[ProcRank]; v < bounds[ProcRank + 1]; v++) {
      for (int u = 0; u < size; u++) {
        if (marks[v] > marks[u] + graph[u * size + v]) {
          marks[v] = marks[u] + graph[u * size + v];
        }
      }
    }
    MPI_Allreduce(MPI_IN_PLACE, marks.data(), size, MPI_INT, MPI_MIN,
                  MPI_COMM_WORLD);
  }
  return marks;
}
