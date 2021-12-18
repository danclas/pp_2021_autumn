// Copyright 2021 Fedoseev Mikhail
#include "../../../modules/task_2/fedoseyev_m_odd_even/odd_even.h"

#include <mpi.h>

#include <algorithm>
#include <random>
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

std::vector<int> MergeSort(std::vector<int> v1, std::vector<int> v2) {
  int i = 0, j = 0, k = 0;
  int n1 = v1.size(), n2 = v2.size();
  std::vector<int> result(n1 + n2);

  while (i < n1 && j < n2) {
    if (v1[i] < v2[j]) {
      result[k] = v1[i];
      i++;
      k++;
    } else {
      result[k] = v2[j];
      j++;
      k++;
    }
  }
  if (i == n1) {
    while (j < n2) {
      result[k] = v2[j];
      j++;
      k++;
    }
  } else {
    while (i < n1) {
      result[k] = v1[i];
      i++;
      k++;
    }
  }
  return result;
}

void SequentialOddEvenSort(std::vector<int>* vec) {
  size_t N = (*vec).size();
  for (size_t i = 0; i < N; i++) {
    for (size_t j = (i % 2) ? 0 : 1; j + 1 < N; j += 2) {
      if ((*vec)[j] > (*vec)[j + 1]) {
        std::swap((*vec)[j], (*vec)[j + 1]);
      }
    }
  }
}

void ParallelOddEvenSort(std::vector<int>* arr) {
  std::vector<int> local_vec;
  std::vector<int> temp_vec;
  int m, N = (*arr).size();
  int rank, size;
  int delta, remainder;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    delta = N / size;
    remainder = N % size;
    if (remainder != 0) {
      (*arr).resize(N + size - remainder);
      for (int i = N; i < N + size - remainder; i++) (*arr)[i] = 0;
      delta = delta + 1;
    }

    MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_vec.resize(delta);
    MPI_Scatter((*arr).data(), delta, MPI_INT, local_vec.data(), delta, MPI_INT,
                0, MPI_COMM_WORLD);
    SequentialOddEvenSort(&local_vec);
  } else {
    MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
    local_vec.resize(delta);
    MPI_Scatter((*arr).data(), delta, MPI_INT, local_vec.data(), delta, MPI_INT,
                0, MPI_COMM_WORLD);
    SequentialOddEvenSort(&local_vec);
  }

  int step = 1;
  while (step < size) {
    if (rank % (2 * step) == 0) {
      if (rank + step < size) {
        MPI_Recv(&m, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, &status);
        temp_vec.resize(m);
        MPI_Recv(temp_vec.data(), m, MPI_INT, rank + step, 0, MPI_COMM_WORLD,
                 &status);
        local_vec = MergeSort(local_vec, temp_vec);
        delta = delta + m;
        fflush(stdout);
      }
    } else {
      int near = rank - step;
      MPI_Send(&delta, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
      MPI_Send(local_vec.data(), delta, MPI_INT, near, 0, MPI_COMM_WORLD);
      break;
    }
    step = step * 2;
  }
  if (rank == 0) {
    (*arr) = local_vec;
    if (N % size) {
      for (int i = N; i < N + size - remainder; i++)
        (*arr).erase((*arr).begin());
    }
  }
}
