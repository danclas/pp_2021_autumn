// Copyright 2021 Oskin Georgii
#ifndef MODULES_TASK_1_OSKIN_G_MIN_MATRIX_ROW_MIN_MATRIX_ROW_H_
#define MODULES_TASK_1_OSKIN_G_MIN_MATRIX_ROW_MIN_MATRIX_ROW_H_

#include <string>
#include <vector>

std::vector<std::vector<int>> getRandMatrix(int m, int n);
int getLocalMinimum(std::vector<int> vec, int n);
void getParallelOperations(std::vector<std::vector<int>> g_matrix, int m, int n,
                           std::vector<int> res);
void findMinNonParallel(std::vector<std::vector<int>> mat, int m, int n,
                        std::vector<int> result);

#endif  // MODULES_TASK_1_OSKIN_G_MIN_MATRIX_ROW_MIN_MATRIX_ROW_H_
