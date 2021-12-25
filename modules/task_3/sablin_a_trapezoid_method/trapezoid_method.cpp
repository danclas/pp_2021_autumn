// Copyright 2021 Sablin Alexander

#include <mpi.h>
#include "../../../modules/task_3/sablin_a_trapezoid_method/trapezoid_method.h"


double trapezoidMethodSequential(double (*f)(double, double), double a_x, double b_x,
    double a_y, double b_y, const int n) {
  double ans = 0;
  double x_h = (b_x - a_x) / n;
  double y_h = (b_y - a_y) / n;

  for (double x = a_x + x_h; x < b_x; x += x_h) {
      double cur_ans = (f(x, a_y) + f(x, b_y)) / 2;
      for (double y = a_y + y_h; y < b_y; y += y_h) {
          cur_ans += f(x, y);
      }
      ans += cur_ans;
  }

  ans += (f(a_x, a_y) + f(a_x, b_y) + f(b_x, a_y) + f(b_x, b_y)) / 4;
  for (double y = a_y + y_h; y < b_y; y += y_h) {
      ans += (f(a_x, y) + f(b_x, y)) / 2;
  }

  return ans * x_h * y_h;
}

double trapezoidMethodParallel(double (*f)(double, double), double a_x, double b_x,
    double a_y, double b_y, const int n) {
  int ProcRank, ProcNum;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  double ans_all;
  double ans = 0.0;

  double h_x = (b_x - a_x) / n;
  double h_y = (b_y - a_y) / n;
  double x;

  x = a_x + h_x * (ProcRank + 1);

  while (x < b_x) {
      double cur_ans = (f(x, a_y) + f(x, b_y)) / 2;
      for (double y = a_y + h_y; y < b_y; y += h_y) {
          cur_ans += f(x, y);
      }
      ans += cur_ans;
      x += h_x * ProcNum;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&ans, &ans_all, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (ProcRank == 0) {
      ans_all += (f(a_x, a_y) + f(a_x, b_y) + f(b_x, a_y) + f(b_x, b_y)) / 4;
      for (double y = a_y + h_y; y < b_y; y += h_y) {
          ans_all += (f(a_x, y) + f(b_x, y)) / 2;
      }
  }
  MPI_Barrier(MPI_COMM_WORLD);

  return ans_all * h_x * h_y;
}
