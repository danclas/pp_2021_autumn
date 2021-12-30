// Copyright 2021 Fedoseyev Mikhail
#include "../../modules/task_1/fedoseev_m_integral_rectangle_method/integral_rectangle_method.h"

std::vector<std::function<double(double)> > functions{
    [](double x) { return std::sin(x); }, [](double x) { return (-x + 3.0); },
    [](double x) { return (x * x + 2.0 * x + 1.0); },
    [](double x) { return (1.5 * x * x * x + x * x + 1.3 * x + 3.0); },
    [](double x) { return std::cos(x); }};

double IntegralSequential(double x1, double y1, int z1, int f_i1) {
  double i_s = (y1 - x1) / z1;
  double sum = 0.0;
  for (int i = 0; i < z1; i++) {
    sum += functions[f_i1](x1 + i_s * (i + 0.5));
  }
  return (sum * i_s);
}

double IntegralParallel(double x2, double y2, int z2, int f_i2) {
  double i_p = (y2 - x2) / z2;
  double sum = 0.0;
  double myres, res = 0.0;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Bcast(&z2, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&x2, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&f_i2, 1, MPI_INT, 0, MPI_COMM_WORLD);
  for (int i = rank; i < z2; i += size) {
    sum += functions[f_i2](x2 + i_p * (i + 0.5));
  }
  myres = sum * i_p;
  MPI_Reduce(&myres, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  return res;
}
