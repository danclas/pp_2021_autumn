// Copyright 2021 Shelepin Nikita
#include "../../../modules/task_3/shelepin_n_simpson/simpson.h"

#include <mpi.h>
#include <cmath>

double getSequentialSimpson(
    const std::function<double(std::vector<double>)>& f,
    const std::vector<std::pair<double, double>>& limits,
    const std::vector<int>& n) {
  int dim = n.size();
  std::vector<double> h(dim);
  int64_t counter = 1;
  for (int i = 0; i < dim; ++i) {
    h[i] = (limits[i].second - limits[i].first) / n[i];
    counter *= n[i];
  }
  double result = 0.0;
  for (int i = 0; i < counter; ++i) {
    std::vector<std::vector<double>> params(dim);
    int temp = i;
    for (int j = 0; j < dim; ++j) {
      params[j].push_back(limits[j].first + temp % n[j] * h[j]);
      params[j].push_back(limits[j].first + temp % n[j] * h[j] + h[j] / 2);
      params[j].push_back(limits[j].first + temp % n[j] * h[j] + h[j] / 2);
      params[j].push_back(limits[j].first + temp % n[j] * h[j] + h[j] / 2);
      params[j].push_back(limits[j].first + temp % n[j] * h[j] + h[j] / 2);
      params[j].push_back(limits[j].first + temp % n[j] * h[j] + h[j]);
      temp /= n[j];
    }
    std::vector<double> point;
    for (int i = 0; i < pow(6, dim); ++i) {
      int temp = i;
      for (int j = 0; j < dim; ++j) {
        point.push_back(params[j][temp % 6]);
        temp /= 6;
      }
      result += f(point);
      point.clear();
    }
    params.clear();
  }
  for (int i = 0; i < dim; ++i) {
    result *= h[i] / 6.0;
  }
  return result;
}

double getParallelSimpson(const std::function<double(std::vector<double>)>& f,
                          const std::vector<std::pair<double, double>>& limits,
                          const std::vector<int>& n) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int dim = limits.size();
  std::vector<double> h(dim);

  int total_count;
  if (rank == 0) {
    total_count = 1;
    for (int i = 0; i < dim; ++i) {
      h[i] = (limits[i].second - limits[i].first) / n[i];
      total_count *= n[i];
    }
  }
  MPI_Bcast(&total_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(h.data(), dim, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  int rem = total_count % size;
  int delta = total_count / size + (rank < rem ? 1 : 0);
  int displ = 0;
  if (rank < rem) {
    displ = rank * delta;
  } else {
    displ = rem * (delta + 1) + (rank - rem) * delta;
  }

  double local_result = 0.0;
  for (int i = displ; i < delta + displ; ++i) {
    std::vector<std::vector<double>> params(dim);
    int temp = i;
    for (int j = 0; j < dim; ++j) {
      params[j].push_back(limits[j].first + temp % n[j] * h[j]);
      params[j].push_back(limits[j].first + temp % n[j] * h[j] + h[j] / 2);
      params[j].push_back(limits[j].first + temp % n[j] * h[j] + h[j] / 2);
      params[j].push_back(limits[j].first + temp % n[j] * h[j] + h[j] / 2);
      params[j].push_back(limits[j].first + temp % n[j] * h[j] + h[j] / 2);
      params[j].push_back(limits[j].first + temp % n[j] * h[j] + h[j]);
      temp /= n[j];
    }

    std::vector<double> point;
    for (int i = 0; i < pow(6, dim); ++i) {
      int temp = i;
      for (int j = 0; j < dim; ++j) {
        point.push_back(params[j][temp % 6]);
        temp /= 6;
      }
      local_result += f(point);
      point.clear();
    }
    params.clear();
  }

  double global_result = 0.0;
  MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < dim; ++i) {
      global_result *= h[i] / 6.0;
    }
  }
  return global_result;
}
