// Copyright 2021 Vershinin Daniil
#include "../../../modules/task_1/vershinin_d_vector_max_diff/vector_max_diff.h"

#include <mpi.h>

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

int getSequentialOperations(std::vector<int> vec) {
  const int size = vec.size();
  int max = 0;
  if (size >= 2) {
    max = std::abs(vec[0] - vec[1]);
  } else {
    return 0;
  }

  for (int i = 0; i < size - 1; i++) {
    if (std::abs(vec[i] - vec[i + 1]) > max) {
      max = abs(vec[i] - vec[i + 1]);
    }
  }

  return max;
}

int getParallelOperations(std::vector<int> global_vec) {
  int size, rank;
  int proc_vector_size;
  const int vector_size = global_vec.size();
  int max, all_max;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> sendcounts;
  std::vector<int> displs;

  // size = 5;

  if (rank == 0) {
    int current_proc_size = size;
    int current_vector_size = vector_size;
    int current_delta = 0;

    for (int proc = 0; proc < size; proc++) {
      if (vector_size - size >= 1) {
        int count = current_vector_size / current_proc_size;
        int delta = current_vector_size % current_proc_size;

        sendcounts.push_back(count + delta);
        displs.push_back(current_delta);

        current_vector_size = current_vector_size - count - delta + 1;

        current_proc_size--;
        current_delta = current_delta + count + delta - 1;
      } else {
        if (proc < vector_size - 1) {
          sendcounts.push_back(2);
          displs.push_back(proc);
        } else {
          sendcounts.push_back(0);
          displs.push_back(0);
        }
      }
    }
  }

  MPI_Scatter(sendcounts.data(), 1, MPI_INT, &proc_vector_size, 1, MPI_INT, 0,
              MPI_COMM_WORLD);

  std::vector<int> local_vec(proc_vector_size);

  MPI_Scatterv(global_vec.data(), sendcounts.data(), displs.data(), MPI_INT,
               local_vec.data(), proc_vector_size, MPI_INT, 0, MPI_COMM_WORLD);

  max = getSequentialOperations(local_vec);

  MPI_Reduce(&max, &all_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  return all_max;
}
