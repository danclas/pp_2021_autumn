// Copyright 2021 Lukashuk Diana
#include "../../../modules/task_3/lukashuk_d_monte_carlo/monte_carlo.h"

std::vector<double> getRandomNode(const std::vector<double>& lower,
                                  const std::vector<double>& upper,
                                  int points) {
  int size = static_cast<int>(upper.size());
  std::mt19937 gen(time(0));
  std::vector<std::uniform_real_distribution<double>> uniform(size);

  for (int i = 0; i < size; i++) {
    uniform[i] = std::uniform_real_distribution<double>(lower[i], upper[i]);
  }
  std::vector<double> node(points * size);
  for (int i = 0; i < points; i++) {
    for (int j = 0; j < size; j++) node[i * size + j] = uniform[j](gen);
  }
  return node;
}

std::vector<double> upperLower(const std::vector<double>& lower,
                               const std::vector<double>& upper) {
  std::vector<double> upper_lover(upper);
  int size = static_cast<int>(upper.size());
  for (int i = 0; i < size; i++) {
    upper_lover[i] -= lower[i];
  }
  return upper_lover;
}

double monteCarlo(const std::vector<double>& upper_lover,
                  const std::vector<double>& node, int points,
                  const function& func) {
  int size = static_cast<int>(upper_lover.size());
  std::vector<double> _node(size);
  double result = 0.0;
  for (int i = 0; i < points; i++) {
    for (int j = 0; j < size; j++) {
      _node[j] = node[i * size + j];
    }
    result += func(_node);
  }
  for (int i = 0; i < size; i++) {
    result *= upper_lover[i];
  }
  return result;
}

double getSequential(const std::vector<double>& upper_lover,
                     const std::vector<double>& node, int points,
                     const function& func) {
  return monteCarlo(upper_lover, node, points, func) / points;
}

double getParallel(const std::vector<double>& upper_lover,
                   const std::vector<double>& node, int points,
                   const function& func) {
  int proc_rank, proc_count;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
  int size = static_cast<int>(upper_lover.size());

  int proc_points = points / proc_count;
  int rem = points % proc_count;
  std::vector<double> proc_node(proc_points * size);

  MPI_Scatter(node.data(), proc_points * size, MPI_DOUBLE, proc_node.data(),
              proc_points * size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  proc_points += (proc_rank == 0 ? rem : 0);

  double proc_result =
      monteCarlo(upper_lover, proc_node, proc_points, func) / points;

  double result = 0.0;
  MPI_Reduce(&proc_result, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  return result;
}
