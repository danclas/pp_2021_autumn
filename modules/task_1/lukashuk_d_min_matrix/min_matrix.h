// Copyright 2021 Lukashuk Diana
#ifndef MODULES_TASK_1_LUKASHUK_D_MIN_MATRIX_MIN_MATRIX_H_
#define MODULES_TASK_1_LUKASHUK_D_MIN_MATRIX_MIN_MATRIX_H_
#include <mpi.h>
#include <time.h>

#include <algorithm>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

class Matrix {
 protected:
  int x, y;
  int* m;

 public:
  Matrix();
  Matrix(int xx, int yy);
  Matrix(const Matrix& mat);
  Matrix& operator=(const Matrix& mat) {
    if (this == &mat) return *this;
    if (x + y != 0) delete[] m;
    m = new int[mat.x * mat.y];
    x = mat.x;
    y = mat.y;
    for (int i = 0; i < x * y; i++) {
      m[i] = mat.m[i];
    }
    return *this;
  }
  int get(int a);
  int Min_in_line(int nom_line);
  void set(int nom, int s);
  int Min_in_matrix_mpi();
  int Min_in_matrix_one();
  ~Matrix();
};

#endif  // MODULES_TASK_1_LUKASHUK_D_MIN_MATRIX_MIN_MATRIX_H_
