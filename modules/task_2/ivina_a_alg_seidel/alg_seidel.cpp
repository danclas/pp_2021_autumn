// Copyright 2021 Ivina Anastasiya
#include "alg_seidel.h"
#include <mpi.h>
#include <random>
#include <vector>


double randomValue(int min, int max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> range(min, max);
  return range(rd);
}

std::vector<double> seqSeidel(std::vector<std::vector<double>> &a,
                              std::vector<double> &b, double eps) {
  int n;
  std::vector<double> x;
  std::vector<double> p;
  n = b.size();

  for (int i = 0; i < n; i++) {
    x.push_back(1.0F);
    p.push_back(0.0F);
  }

  auto is_diagonal = [&]() -> bool {
    bool is_diag = true;
    for (int i = 0; i < n; i++) {
      double sum = 0;
      for (const auto e : a[i]) {
        sum += abs(e);
      }
      const auto diag = a[i][i];
      sum -= diag;
      if (sum > diag) {
        is_diag = false;
      }
    }
    return is_diag;
  };

  if (is_diagonal()) {
    double Fullsum = 0;
    do {
      p = x;
      double norm = 0;
      for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
          if (j != i) {
            sum += (a[i][j] * x[j]);
          }
        }
        x[i] = (b[i] - sum) / a[i][i];
        norm += (x[i] - p[i]) * (x[i] - p[i]);
        Fullsum = norm;
      }
    } while (!(sqrt(Fullsum) < eps));
  }
  return x;
}

std::vector<double> parallSeidel(std::vector<std::vector<double>> &a,
                                 std::vector<double> &b, double eps) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n;

  std::vector<double> x;
  std::vector<double> p;
  n = b.size();
  for (int i = 0; i < n; i++) {
    x.push_back(1.0F);
    p.push_back(0.0F);
  }

  auto is_diagonal = [&]() -> bool {
    bool is_diag = true;
    for (int i = 0; i < n; i++) {
      double sum = 0;
      for (const auto e : a[i]) {
        sum += abs(e);
      }
      const auto diag = a[i][i];
      sum -= diag;
      if (sum > diag) {
        is_diag = false;
      }
    }
    return is_diag;
  };

  if (is_diagonal()) {
    double fullSum = 0;
    int num = (n / size) * (size - 1) + ((n - (n / size) * size) - 1);
    do {
      p = x;
      double norm = 0;
      for (int i = rank; i < n; i += size) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
          if (j != i) {
            sum += (a[i][j] * x[j]);
          }
        }
        x[i] = (b[i] - sum) / a[i][i];
        norm += (x[i] - p[i]) * (x[i] - p[i]);
        if (rank == 0) {
          for (int j = 0; j < num; j++) {

            double buff[2];
            MPI_Recv(buff, 2, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                     MPI_STATUSES_IGNORE);
            x[static_cast<size_t>(buff[0])] = buff[1];
          }
        } else {
          double buff[2];
          buff[0] = i;
          buff[1] = x[i];
          MPI_Send(buff, 2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
      }
      MPI_Reduce(&norm, &fullSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      MPI_Bcast(&fullSum, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } while (!(sqrt(fullSum) < eps));
  }
  return x;
}
