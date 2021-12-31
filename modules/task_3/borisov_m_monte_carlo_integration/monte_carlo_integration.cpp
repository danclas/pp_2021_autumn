// Copyright [2021] <Borisov Maxim>

#include "../../../modules/task_3/borisov_m_monte_carlo_integration/monte_carlo_integration.h"

#include <random>

double sequential_monte_carlo(
    std::vector<double> up, std::vector<double> lo, int points_size,
    const std::function<double(const std::vector<double> &)> &f) {
  int d = static_cast<int>(up.size());
  double res = 0.0;
  std::mt19937 gen(time(0));
  std::vector<std::uniform_real_distribution<double>> u_gen(d);

  for (int i = 0; i < d; i++) {
    u_gen[i] = std::uniform_real_distribution<double>(lo[i], up[i]);
  }
  std::vector<double> in_dots(d);

  for (int i = 0; i < points_size; i++) {
    for (int j = 0; j < d; j++) {
      in_dots[j] = u_gen[j](gen);
    }
    res += f(in_dots);
  }
  for (int i = 0; i < d; i++) {
    res *= (up[i] - lo[i]);
  }
  res /= points_size;
  return res;
}

double parallel_monte_carlo(
    const std::vector<double> &up, const std::vector<double> &lo,
    int points_size,
    const std::function<double(const std::vector<double> &)> &f) {
  int proc_rank;
  int proc_count;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
  int d = static_cast<int>(up.size());
  double part_res = 0.0;
  double res = 0.0;
  std::vector<int> part_points_size(proc_count);
  std::vector<double> in_dots(d);
  std::mt19937 gen(time(0));
  std::vector<std::uniform_real_distribution<double>> u_gen(d);
  if (proc_rank == 0) {
    for (int i = 0; i < d; i++) {
      u_gen[i] = std::uniform_real_distribution<double>(lo[i], up[i]);
    }
  }
  MPI_Bcast(u_gen.data(), 2 * d, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  for (int i = 0; i < proc_count; i++) {
    part_points_size[i] = points_size / proc_count;
  }
  if (points_size % proc_count > 0) {
    part_points_size[proc_count - 1] += points_size % proc_count;
  }
  for (int i = 0; i < part_points_size[proc_rank]; i++) {
    for (int j = 0; j < d; j++) {
      in_dots[j] = u_gen[j](gen);
    }
    part_res += f(in_dots);
  }
  MPI_Reduce(&part_res, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (proc_rank == 0) {
    for (int i = 0; i < d; i++) {
      res *= (up[i] - lo[i]);
    }
    res /= points_size;
  }
  return res;
}
