// Copyright 2021 Lukashuk Diana

#include "../../../modules/task_1/lukashuk_d_min_matrix/min_matrix.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

Matrix::Matrix() {
  x = 1;
  y = 1;
  m = new int[1];
}

Matrix::Matrix(int xx, int yy) {
  if (xx < 0 || yy < 0) {
    static int val[] = {2,  4,  19, 0,  -5, -4, 2,  2,  5,  -100, 20, 10, 12,
                        14, 50, 10, 90, 32, 65, 11, 50, 23, 85,   75, 10};
    m = new int[5 * 5];
    x = 5;
    y = 5;
    for (int i = 0; i < 25; i++) {
      m[i] = val[i];
    }
  } else {
    m = new int[xx * yy];
    x = xx;
    y = yy;
    std::mt19937 gen(time(0));
    for (int i = 0; i < xx * yy; i++) {
      m[i] = -1000 + (gen() % 1000);
    }
  }
}

Matrix::Matrix(const Matrix& mat) {
  if (x * y != 0) delete[] m;
  x = mat.x;
  y = mat.y;
  m = new int[x * y];
  for (int i = 0; i < x * y; i++) m[i] = mat.m[i];
}

Matrix& Matrix::operator=(const Matrix& mat) {
  if (x + y != 0) delete[] m;
  m = new int[mat.x * mat.y];
  x = mat.x;
  y = mat.y;
  for (int i = 0; i < x * y; i++) {
    m[i] = mat.m[i];
  }
  return *this;
}

int Matrix::get(int a) { return m[a]; }

int Matrix::Min_in_line(int nom_line) {
  if (nom_line > y - 1) return 0;
  int st = nom_line * x;
  int end = st + x - 1;
  int res = m[st];
  st++;
  while (st <= end) {
    if (res > m[st]) res = m[st];
    st++;
  }
  return res;
}

void Matrix::set(int nom, int s) {
  if (nom < x * y) m[nom] = s;
}

int Matrix::Min_in_matrix_mpi() {
  int rank;
  int size;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    if (size == 1) {
      Matrix mr(y, 1);
      for (int i = 0; i < y; i++) {
        mr.set(i, this->Min_in_line(i));
      }
      return mr.Min_in_line(0);
    } else {
      int nom_l_letter = 0;
      if (y > size)
        nom_l_letter = size;
      else
        nom_l_letter = y;

      Matrix mr(nom_l_letter, 1);
      mr.set(0, this->Min_in_line(0));

      MPI_Status stat;
      int get_m;

      for (int i = 1; i < nom_l_letter; i++) {
        MPI_Recv(&get_m, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
        mr.set(i, get_m);
      }
      return mr.Min_in_line(0);
    }
  } else {
    bool cl = true;
    int r;
    for (int i = rank; i < y; i = i + size - 1) {
      if (cl) {
        r = this->Min_in_line(i);
        cl = false;
      } else {
        if (r > this->Min_in_line(i)) r = this->Min_in_line(i);
      }
    }
    int nom_l_letter = 0;
    if (y > size)
      nom_l_letter = size;
    else
      nom_l_letter = y;
    if (rank < nom_l_letter) MPI_Send(&r, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
  }
}

int Matrix::Min_in_matrix_one() {
  Matrix mr(y, 1);
  for (int i = 0; i < y; i++) {
    mr.set(i, this->Min_in_line(i));
  }
  return mr.Min_in_line(0);
}

Matrix::~Matrix() {
  if (x + y != 0) delete[] m;
}
