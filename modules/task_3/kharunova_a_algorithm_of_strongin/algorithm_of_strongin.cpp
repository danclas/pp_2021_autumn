// Copyright 2021 Kharunova Alina

#include <mpi.h>

#include <math.h>
#include <random>
#include <vector>
#include <ctime>

#include "../../../modules/task_3/kharunova_a_algorithm_of_strongin/algorithm_of_strongin.h"

double funtion(double x) { return x*x; }

double linAlgorithm(double x0, double x1, double eps) {
  std::vector<double> x;
  std::vector<double> y;
  double lipshM = 0, lipshm, R = 0;
  int interval = 0;

  if (x1 > x0) {
    x.push_back(x0);
    x.push_back(x1);
  } else {
    x.push_back(x1);
    x.push_back(x0);
  }

  while (1) {
    for (int i = 0; i < static_cast<int>(x.size()); i++) {
      y.push_back(funtion(x[i]));
    }
    for (int i = 0; i < static_cast<int>(x.size()) - 1; i++) {
      double lipsh = std::abs((y[i + 1] - y[i]) / (x[i + 1] - x[i]));
      if (lipsh > lipshM) {
        lipshM = lipsh;
        lipshm = 2 * lipsh;
        double tempR =
            lipshm * (x[i + 1] - x[i]) +
            pow((y[i + 1] - y[i]), 2) / (lipshm * (x[i + 1] - x[i])) -
            2 * (y[i + 1] + y[i]);
        if (tempR > R) {
          R = tempR;
          interval = i;
        }
      }
    }
    if (x[interval + 1] - x[interval] <= eps) {
      return y[interval + 1];
    }
    double newX;
    newX = (x[interval + 1] - x[interval]) / 2 + x[interval]
                     + (y[interval + 1] - y[interval]) / (2 * lipshm);
    x.push_back(newX);
    sort(x.begin(), x.end());
    y.clear();
  }
}

double paralAlgorithm(const double x0, const double x1, double eps) {
  int size, rank;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    std::vector<double> x;
    x.push_back(x0);
    x.push_back(x1);

    while (1) {
      sort(x.begin(), x.end());

      int part = static_cast<int>(x.size() - 1) / size;
      int remain = static_cast<int>(x.size() - 1) % size;

      for (int i = 1; i < size; ++i) {
        MPI_Send(&x[0] + remain + i * part, 1, MPI_DOUBLE, i, 0,
                 MPI_COMM_WORLD);
      }

      double lipshM = 0, lipshm, lipsh;
      for (int i = 0; i < part + remain; ++i) {
        lipsh =
            std::abs((funtion(x[i + 1]) - funtion(x[i])) / (x[i + 1] - x[i]));
        if (lipsh > lipshM) {
          lipshM = lipsh;
          lipshm = 2 * lipsh;
        }
      }
      if (part > 0) {
        for (int i = 1; i < size; i++) {
          MPI_Recv(&lipsh, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD,
                   &status);
          if (lipsh > lipshM) {
            lipshM = lipsh;
            lipshm = 2 * lipsh;
          }
        }
      }

      int interval = 0;
      double tempR, R = 0;
      for (int i = 0; i < static_cast<int>(x.size()) - 1; i++) {
        tempR = lipshm * (x[i + 1] - x[i]) +
                 pow((funtion(x[i + 1]) -
                 funtion(x[i])), 2) / (lipshm * (x[i + 1] - x[i])) -
                 2 * (funtion(x[i + 1]) + funtion(x[i]));
        if (tempR > R) {
          R = tempR;
          interval = i;
        }
      }

      if (x[interval + 1] - x[interval] <= eps) {
        for (int i = 1; i < size; ++i)
          MPI_Send(&x[0], 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        return x[interval + 1];
      }

      double newX = (x[interval] + x[interval + 1]) / 2 -
                    (funtion(x[interval + 1]) -
                    funtion(x[interval])) / (2 * lipshm);
      x.push_back(newX);
    }
  } else {
    int part;
    while (1) {
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status, MPI_DOUBLE, &part);

      std::vector<double> x(part + 1);
      MPI_Recv(&x[0], 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      if (status.MPI_TAG == 1) return 0;

      double lipshM = 0, lipshm = 1.0, lipsh;
      if (part != 0) {
        for (int i = 0; i < static_cast<int>(x.size()) - 1; ++i) {
          lipsh = (std::abs(funtion(x[i + 1]) - funtion(x[i]))) /
                        (x[i + 1] - x[i]);
          if (lipsh > lipshM) {
            lipshM = lipsh;
            lipshm = 2 * lipshM;
          }
        }
      }
      MPI_Send(&lipshm, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    return 0;
  }
}
