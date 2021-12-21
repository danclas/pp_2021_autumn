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

std::vector<int> getRandomVector(int sz);
int getParallelOperations(std::vector<int> global_vec, int count_size_vector,
                          const std::string& ops);
int getSequentialOperations(std::vector<int> vec, const std::string& ops);

class Matrix {
 protected:
  int x, y;
  int* m;

 public:
  Matrix();
  Matrix(int xx, int yy);
  Matrix(const Matrix& mat);
  Matrix& operator=(const Matrix& mat);
  int get(int a);
  int Min_in_line(int nom_line);
  void set(int nom, int s);
  int Min_in_matrix_mpi();
  int Min_in_matrix_one();
  ~Matrix();
};

#endif  // MODULES_TASK_1_LUKASHUK_D_MIN_MATRIX_MIN_MATRIX_H_
