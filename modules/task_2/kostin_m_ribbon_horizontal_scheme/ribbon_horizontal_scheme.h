// Copyright 2021 Kostin Maksim
#ifndef MODULES_TASK_2_KOSTIN_M_RIBBON_HORIZONTAL_SCHEME_RIBBON_HORIZONTAL_SCHEME_H_
#define MODULES_TASK_2_KOSTIN_M_RIBBON_HORIZONTAL_SCHEME_RIBBON_HORIZONTAL_SCHEME_H_

#include <vector>

std::vector<int> getRandomVector(int sz);
std::vector<int> getRandomMatrix(int row, int column);
int* VecToInt(int* arr, std::vector<int> vec, int sz);
std::vector<int> IntToVec(std::vector<int> vec, int* arr, int sz);
std::vector<int> ParallelMatrixMultiplication(std::vector<int> A, int row_a,
  int col_a, std::vector<int> B, int row_b, int col_b);
std::vector<int> SequentialMatrixMultiplication(std::vector<int> A,
  int row_a, int col_a, std::vector<int> B, int row_b, int col_b);

#endif  // MODULES_TASK_2_KOSTIN_M_RIBBON_HORIZONTAL_SCHEME_RIBBON_HORIZONTAL_SCHEME_H_
