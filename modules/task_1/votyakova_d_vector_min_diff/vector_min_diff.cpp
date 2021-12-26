// Copyright 2021 Votyakova Darya
#include "../../../modules/task_1/votyakova_d_vector_min_diff/vector_min_diff.h"

#include <mpi.h>

#include <climits>
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

int* getSequentialMinDiff(std::vector<int> vec) {
  int* difMin = new int[3];
  difMin[2] = INT_MAX;
  if (vec.size() != 0) {
    for (int i = 0; i < static_cast<int>(vec.size()) - 1; i++) {
      if (difMin[2] > abs(vec[i] - vec[i + 1])) {
        difMin[0] = vec[i];
        difMin[1] = vec[i + 1];
        difMin[2] = abs(vec[i] - vec[i + 1]);
      }
    }
  }

  return difMin;
}

int* getParallelMinDiff(std::vector<int> global_vec) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int vector_size;
  if (rank == 0) {
    vector_size = global_vec.size();
  }
  MPI_Bcast(&vector_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int delta = (vector_size + size - 1) / size;
  int rem = (vector_size + size - 1) % size;

  std::vector<int> local_vector;

  if (delta && size != 1) {
    int* local_mindiff;
    int pos = delta + rem - 1;

    if (rank == 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Send(global_vec.data() + pos, delta, MPI_INT, proc, 0,
                 MPI_COMM_WORLD);
        pos += delta - 1;
      }

      local_vector = std::vector<int>(global_vec.begin(),
                                      global_vec.begin() + delta + rem);
    } else {
      local_vector = std::vector<int>(delta);
      MPI_Status status;
      MPI_Recv(local_vector.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD,
               &status);
    }

    local_mindiff = getSequentialMinDiff(local_vector);

    int* global_mindiff;
    global_mindiff = new int[3 * size];

    MPI_Gather(local_mindiff, 3, MPI_INT, global_mindiff, 3, MPI_INT, 0,
               MPI_COMM_WORLD);

    int* temp = new int[3];
    if (rank == 0) {
      temp[0] = global_mindiff[0];
      temp[1] = global_mindiff[1];
      temp[2] = global_mindiff[2];
      for (int i = 1; i < size; i++) {
        if (global_mindiff[2 + i * 3] < temp[2]) {
          temp[0] = global_mindiff[i * 3];
          temp[1] = global_mindiff[1 + i * 3];
          temp[2] = global_mindiff[2 + i * 3];
        }
      }
    }

    return temp;
  } else {
    return getSequentialMinDiff(global_vec);
  }
}
