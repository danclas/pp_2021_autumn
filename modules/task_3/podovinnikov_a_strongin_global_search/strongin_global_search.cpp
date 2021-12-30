// Copyright 2021 Podovinnikov Artyom
#include "../../../modules/task_3/podovinnikov_a_strongin_global_search/strongin_global_search.h"

#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>

#define eps 1.2e-02

double seqStrongin(double a, double b, double (*Q)(double)) {
  double Rt, Ri;
  std::vector<double> x;
  std::vector<double> Qx;

  x.push_back(a);
  Qx.push_back(Q(a));
  x.push_back(b);
  Qx.push_back(Q(b));

  double QQ, xx;
  if (Qx[0] <= Qx[1]) {
    QQ = Qx[0];
    xx = x[0];
  } else {
    QQ = Qx[1];
    xx = x[1];
  }

  int k = 2;

  int t = 0;
  int idx;

  double deltaQi;
  double deltaxi;
  double Lk, lk;
  double r = 2.0;
  double xt;
  bool flag = true;
  while (flag) {
    double temp;
    lk = 100;
    for (int i = 0; i < k - 1; i++) {
      deltaQi = Qx[i + 1] - Qx[i];
      deltaxi = x[i + 1] - x[i];
      temp = std::abs(deltaQi) / deltaxi;
      if (temp > lk) {
        lk = temp;
      }
    }

    if (lk > 0) Lk = r * lk;
    if (lk == 0) Lk = 1;

    Rt = 100;
    for (int i = 0; i < k - 1; i++) {
      deltaQi = Qx[i + 1] - Qx[i];
      deltaxi = x[i + 1] - x[i];

      Ri = (Qx[i] + Qx[i + 1] -
            (1.0 + ((deltaQi / deltaxi) / Lk) * ((deltaQi / deltaxi) / Lk)) *
                Lk * deltaxi);
      Ri = Ri * 1.0 / 2.0;
      if (Ri < Rt) {
        Rt = Ri;
        t = i;
      }
    }

    if (x[t + 1] - x[t] < eps) {
      flag = false;
    } else {
      xt = 1.0 / 2.0 * (x[t] + x[t + 1] - deltaQi / Lk);
      idx = 0;

      while (xt > x[idx]) {
        idx++;
      }
      auto iter1 = x.cbegin();
      auto iter2 = Qx.cbegin();
      x.emplace(iter1 + idx, xt);
      Qx.emplace(iter2 + idx, Q(xt));

      if (Qx[idx] < QQ) {
        QQ = Qx[idx];
        xx = x[idx];
      }

      k = k + 1;
    }
  }

  return xx;
}
double MPI_Strongin(double a, double b, double (*Q)(double)) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;

  std::vector<double> gl_vec;

  double h = std::abs(b - a) / (static_cast<double>(size));
  gl_vec.push_back(a);
  for (int i = 1; i < size; i++) {
    gl_vec.push_back(a + i * h);
    gl_vec.push_back(a + i * h);
  }
  gl_vec.push_back(b);

  int l_N = 2;

  if (rank == 0) {
    for (int i = 1; i < size; i++) {
      MPI_Send(&gl_vec[0] + i * l_N, l_N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }
  }
  std::vector<double> l_vec(l_N);
  if (rank == 0) {
    l_vec = std::vector<double>(gl_vec.begin(), gl_vec.begin() + l_N);
  } else {
    MPI_Status status;
    MPI_Recv(&l_vec[0], l_N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
  }
  double answer = 0.0;
  double l_answer = seqStrongin(l_vec[0], l_vec[1], Q);
  l_answer = Q(l_answer);
  MPI_Op op_code;
  op_code = MPI_MIN;
  MPI_Reduce(&l_answer, &answer, 1, MPI_DOUBLE, op_code, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    for (int i = 1; i < size; i++) {
      MPI_Send(&answer, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(&answer, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
  }
  return answer;
}
