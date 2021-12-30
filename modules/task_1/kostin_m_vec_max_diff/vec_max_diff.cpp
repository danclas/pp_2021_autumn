// Copyright 2021 Kostin Maksim
#include "../../../modules/task_1/kostin_m_vec_max_diff/vec_max_diff.h"
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

int getSequentialMaxDiff(std::vector<int> vec) {
  int max_diff = -1;
  if (vec.size() > 1) {
    for (int i = 0; i < vec.size() - 1; i++) {
      if (abs(vec[i] - vec[i + 1]) > max_diff) {
        max_diff = abs(vec[i] - vec[i + 1]);
      }
    }
  } else {
    return 0;
  }
  return max_diff;
}

int getParallelMaxDiff(std::vector<int> global_vec) {
  int size, rank;
  int proc_vec_size;
  const int vec_size = global_vec.size();
  int local_max, global_max;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> sendcounts;
  std::vector<int> displs;

  if (rank == 0) {
    int curr_proc_size = size;
    int curr_vec_size = vec_size;
    int curr_rem = 0;
    for (int proc = 0; proc < size; proc++) {
      if (vec_size - size > 0) {
        int proc_number = curr_vec_size / curr_proc_size;
        int rem = curr_vec_size % curr_proc_size;
        sendcounts.push_back(proc_number + rem);
        displs.push_back(curr_rem);
        curr_vec_size = curr_vec_size - proc_number - rem + 1;
        curr_proc_size--;
        curr_rem = curr_rem + proc_number + rem - 1;
      } else {
        if (proc < vec_size - 1) {
          sendcounts.push_back(2);
          displs.push_back(proc);
        } else {
          sendcounts.push_back(0);
          displs.push_back(0);
        }
      }
    }
  }

  MPI_Scatter(sendcounts.data(), 1, MPI_INT, &proc_vec_size, 1, MPI_INT, 0,
              MPI_COMM_WORLD);

  std::vector<int> local_vec(proc_vec_size);

  MPI_Scatterv(global_vec.data(), sendcounts.data(), displs.data(), MPI_INT,
               local_vec.data(), proc_vec_size, MPI_INT, 0, MPI_COMM_WORLD);

  local_max = getSequentialMaxDiff(local_vec);

  MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  return global_max;
}
