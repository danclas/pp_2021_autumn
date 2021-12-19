// Copyright 2021 Ershov Aleksey
#include "./deicstra_mpi.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <vector>

std::vector<std::vector<int>> getRandomVector(const size_t count) {
  std::vector<std::vector<int>> graf(count, std::vector<int>(count));
  std::random_device dev;
  std::mt19937 gen(0);
  for (size_t i = 0; i < count; ++i) {
    graf[i][i] = 0;
    for (size_t j = i + 1; j < count; ++j) {
      graf[i][j] = gen() % 100;
      graf[j][i] = graf[i][j];
    }
  }
  return graf;
}

std::vector<int> getDeicstra(const std::vector<std::vector<int>>& graf,
                             const size_t count, const size_t top) {
  std::vector<bool> visitedTops(count);
  std::vector<int> dist(count, 10000);
  dist[top] = 0;
  int min_dist = 0;
  int min_vertex = top;

  while (min_dist < 10000) {
    size_t i = min_vertex;
    visitedTops[i] = true;
    for (size_t j = 0; j < count; ++j)
      if ((dist[i] + graf[i][j] < dist[j]) && (graf[i][j] != 0))
        dist[j] = dist[i] + graf[i][j];
    min_dist = 10000;
    for (size_t j = 0; j < count; ++j)
      if (!visitedTops[j] && dist[j] < min_dist) {
        min_dist = dist[j];
        min_vertex = j;
      }
  }

  return dist;
}

std::vector<int> getParallelDeicstra(const std::vector<std::vector<int>>& graf,
                                     const size_t count) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  size_t part = count / size;
  std::vector<int> tmp_result(count * part, 0);
  std::vector<int> result(count * count, 0);

  for (size_t i = 0; i < part; ++i) {
    auto tmp = getDeicstra(graf, count, (rank * part) + i);
    for (size_t k = 0, j = count * i; k < count; ++j, ++k) {
      tmp_result[j] = tmp[k];
    }
  }
  MPI_Gather(tmp_result.data(), (count * part), MPI_INT, result.data(),
             (count * part), MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (size_t i = size * part; i < count; ++i) {
      auto tmp = getDeicstra(graf, count, i);
      for (size_t k = 0, j = count * i; k < count; ++j, ++k) {
        result[j] = tmp[k];
      }
    }
  }

  return result;
}

std::vector<int> getSequentialDeicstra(
    const std::vector<std::vector<int>>& graf, const size_t count) {
  size_t top = 0;
  std::vector<int> result(count * count, 0);
  for (top = 0; top < count; ++top) {
    auto tmp = getDeicstra(graf, count, top);
    for (size_t i = 0; i < count; ++i) {
      result[top * count + i] = tmp[i];
    }
  }

  return result;
}
