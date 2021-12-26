// Copyright 2021 Votyakova Daria
#include "../../../modules/task_3/votyakova_d_trapezoidal/trapezoidal.h"

#include <mpi.h>

double SequentialTrapezoidal(int segments, ABs ab, func fn) {
  int integral_dim = static_cast<int>(ab.size());
  int it = 1;
  vector<double> height(integral_dim);
  for (int i = 0; i < integral_dim; i++) {
    height[i] = (ab[i].second - ab[i].first) / segments;
    it = it * segments;
  }
  double result = 0.0;
  vector<double> x(integral_dim);
  for (int i = 0; i < it; i++) {
    for (int j = 0; j < integral_dim; j++) {
      x[j] = ab[j].first + (i % segments) * height[j] + height[j] * 0.5;
    }
    result += fn(x);
  }

  for (int i = 0; i < integral_dim; i++) {
    result *= height[i];
  }
  return result;
}

double ParallelTrapezoidal(int segments, ABs ab, const func fn) {
  int world_size, proc_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

  int integral_dim = static_cast<int>(ab.size());
  vector<double> height(segments);
  int it_all = 1;

  for (int i = 0; i < integral_dim; i++) {
    height[i] = (ab[i].second - ab[i].first) / segments;
    it_all *= segments;
  }

  int delta = it_all / world_size;

  int start = 0;
  if (proc_rank != 0) start = proc_rank * delta + it_all % world_size;
  int end = (proc_rank + 1) * delta + it_all % world_size;

  vector<double> x(integral_dim);
  double local_result = 0.0;
  for (int j = start; j < end; j++) {
    for (int i = 0; i < integral_dim; i++) {
      x[i] = ab[i].first + (j % segments) * height[i] + height[i] * 0.5;
    }
    local_result += fn(x);
  }

  double global_result = 0.0;
  MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);

  if (proc_rank == 0) {
    for (int i = 0; i < integral_dim; i++) {
      global_result *= height[i];
    }
  }

  return global_result;
}
