// Copyright 2021 Abdullin Konstantin
#include <mpi.h>
#include <ctime>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/abdullin_k_shell_sort_batcher_merge/shell_sort_batcher_merge.h"

std::vector<std::pair<int, int>> Comparators;

std::vector<double> RandomVector(int size) {
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> random(0, 100);

    std::vector<double> result(size);

  for (int i = 0; i < size; i++)
    result[i] = random(gen);

  return result;
}

std::vector <double> Shell_sort(std::vector <double> data) {
  size_t size = data.size();
  for (int d = size / 2; d > 0; d /= 2)
    for (int i = d; i < size; i++)
      for (int j = i; j >= d && data[j] < data[j - d]; j -= d)
          std::swap(data[j], data[j - d]);
  return data;
}

void build_network(std::vector<int> ranks) {
  int size = ranks.size();
  if (size == 1)
    return;

  std::vector<int> up_ranks { ranks.begin(), ranks.begin() + size / 2};
  std::vector<int> down_ranks { ranks.begin() + size / 2, ranks.end() };

  build_network(up_ranks);
  build_network(down_ranks);
  create_connection(up_ranks, down_ranks);
}

void create_connection(std::vector<int> up_ranks, std::vector<int> down_ranks) {
  int size = up_ranks.size() + down_ranks.size();
  std::vector<int> ranks(size);

  if (size == 1) {
      return;
  } else {
      if (size == 2) {
          Comparators.push_back({ up_ranks.front(), down_ranks.front() });
          return;
      }
  }

  std::vector<int> odd_up_ranks, even_up_ranks;
  for (int i = 0; i < up_ranks.size(); i++) {
    if (i % 2 == 1)
      odd_up_ranks.push_back(up_ranks[i]);
    else
      even_up_ranks.push_back(up_ranks[i]);
  }

  std::vector<int> odd_down_ranks, even_down_ranks;
  for (int i = 0; i < down_ranks.size(); i++) {
    if (i % 2 == 1)
      odd_down_ranks.push_back(down_ranks[i]);
    else
      even_down_ranks.push_back(down_ranks[i]);
  }

  create_connection(odd_up_ranks, odd_down_ranks);
  create_connection(even_up_ranks, even_down_ranks);

  merge(up_ranks.begin(), up_ranks.end(), down_ranks.begin(),
    down_ranks.end(), ranks.begin());

  for (int i = 1; i + 1 < size; i += 2)
    Comparators.push_back({ ranks[i], ranks[i + 1] });
}

const int Tag = 0;
const int root = 0;

std::vector<double> Parallel_sort(std::vector<double> data) {
  int data_size = data.size();

  int rank, commSize;
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;

  if (commSize <= 0)
    throw - 1;

  if (commSize == 1 || commSize >= data_size)
    return Shell_sort(data);

  int new_data_size = data_size;
  if (data_size % commSize != 0) {
    new_data_size += commSize - (data_size % commSize);
    for (int i = 0; i < commSize - (data_size % commSize); i++)
      data.push_back(110.0);
  }

  int partSize = new_data_size / commSize;
  std::vector<double> part(partSize), otherPart(partSize), tmp(partSize);
  std::vector<int> ranks(commSize);

  for (int i = 0; i < commSize; i++)
    ranks[i] = i;
  build_network(ranks);

  MPI_Scatter(data.data(), partSize, MPI_DOUBLE, part.data(),
    partSize, MPI_DOUBLE, root, MPI_COMM_WORLD);
  part = Shell_sort(part);

  for (auto& comparator : Comparators) {
    if (rank == comparator.first) {
      MPI_Send(part.data(), partSize, MPI_DOUBLE,
        comparator.second, Tag, MPI_COMM_WORLD);
      MPI_Recv(otherPart.data(), partSize, MPI_DOUBLE,
        comparator.second, Tag, MPI_COMM_WORLD, &status);
      int partIndex = 0, otherPartIndex = 0;
      for (int tmpIndex = 0; tmpIndex < partSize; tmpIndex++) {
        if (part[partIndex] < otherPart[otherPartIndex]) {
          tmp[tmpIndex] = part[partIndex];
          partIndex++;
        } else {
          tmp[tmpIndex] = otherPart[otherPartIndex];
          otherPartIndex++;
        }
      }
      swap(part, tmp);
    } else if (rank == comparator.second) {
      MPI_Recv(otherPart.data(), partSize, MPI_DOUBLE,
        comparator.first, Tag, MPI_COMM_WORLD, &status);
      MPI_Send(part.data(), partSize, MPI_DOUBLE,
        comparator.first, Tag, MPI_COMM_WORLD);

      int partIndex = partSize - 1, otherPartIndex = partSize - 1;
      for (int tmpIndex = partSize - 1; tmpIndex >= 0; tmpIndex--) {
        if (part[partIndex] > otherPart[otherPartIndex]) {
          tmp[tmpIndex] = part[partIndex];
          partIndex--;
        } else {
          tmp[tmpIndex] = otherPart[otherPartIndex];
          otherPartIndex--;
        }
      }
      swap(part, tmp);
    }
  }
  MPI_Gather(part.data(), partSize, MPI_DOUBLE, data.data(),
    partSize, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == 0 && new_data_size != data_size) {
    for (int i = 0; i < commSize - (data_size % commSize); i++)
      data.pop_back();
  }
  return data;
}

