// Copyright 2021 Shabunin Vladislav
#include "../../../modules/task_3/shabunin_v_shell_sort/shell_sort.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

std::vector<int> getRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = gen() % 100;
  }
  return vec;
}

std::vector<int> merge(std::vector<int>* v1, std::vector<int>* v2) {
  int i = 0, j = 0, k = 0;
  int n1 = (*v1).size(), n2 = (*v2).size();
  std::vector<int> result(n1 + n2);

  while (i < n1 && j < n2)
    if ((*v1)[i] < (*v2)[j]) {
      result[k] = (*v1)[i];
      i++;
      k++;
    } else {
      result[k] = (*v2)[j];
      j++;
      k++;
    }
  if (i == n1) {
    while (j < n2) {
      result[k] = (*v2)[j];
      j++;
      k++;
    }
  } else {
    while (i < n1) {
      result[k] = (*v1)[i];
      i++;
      k++;
    }
  }
  return result;
}

void swap(std::vector<int>& vec, int i, int j) {
  int temp = vec[i];
  vec[i] = vec[j];
  vec[j] = temp;
}

void getSequentialShellSort(std::vector<int>* vec) {
  int i, j, increment, temp, size = (*vec).size();
  increment = 3;
  while (increment > 0) {
    for (i = 0; i < size; i++) {
      j = i;
      temp = (*vec)[i];
      while ((j >= increment) && ((*vec)[j - increment] > temp)) {
        (*vec)[j] = (*vec)[j - increment];
        j = j - increment;
      }
      (*vec)[j] = temp;
    }
    if (increment / 2 != 0)
      increment = increment / 2;
    else if (increment == 1)
      increment = 0;
    else
      increment = 1;
  }
}

void getParallelShellSort(std::vector<int>* vec) {
  std::vector<int> chunk;
  std::vector<int> other;
  int m = (*vec).size(), n = (*vec).size();
  int rank, size;
  int delta;
  int i;
  int step;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    int r;
    delta = n / size;
    r = n % size;

    if (r != 0) {
      (*vec).resize(n + size - r);
      for (i = n; i < n + size - r; i++) (*vec)[i] = 0;
      delta = delta + 1;
    }

    MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
    chunk.resize(delta);
    MPI_Scatter((*vec).data(), delta, MPI_INT, chunk.data(), delta, MPI_INT, 0,
                MPI_COMM_WORLD);
    getSequentialShellSort(&chunk);
  } else {
    MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
    chunk.resize(delta);
    MPI_Scatter((*vec).data(), delta, MPI_INT, chunk.data(), delta, MPI_INT, 0,
                MPI_COMM_WORLD);
    getSequentialShellSort(&chunk);
  }

  step = 1;

  while (step < size) {
    if (rank % (2 * step) == 0) {
      if (rank + step < size) {
        MPI_Recv(&m, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, &status);
        other.resize(m);
        MPI_Recv(other.data(), m, MPI_INT, rank + step, 0, MPI_COMM_WORLD,
                 &status);
        chunk = merge(&chunk, &other);
        delta = delta + m;
        fflush(stdout);
      }
    } else {
      int near = rank - step;
      MPI_Send(&delta, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
      MPI_Send(chunk.data(), delta, MPI_INT, near, 0, MPI_COMM_WORLD);
      break;
    }
    step = step * 2;
  }
  if (rank == 0) {
    (*vec) = chunk;
    if (n % size) {
      for (i = n; i < n + size - n % size; i++) (*vec).erase((*vec).begin());
    }
  }
}
