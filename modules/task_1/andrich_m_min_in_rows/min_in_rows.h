// Copyright 2021 Andrich Maria
#ifndef MODULES_TASK_1_ANDRICH_M_MIN_IN_ROWS_MIN_IN_ROWS_H_
#define MODULES_TASK_1_ANDRICH_M_MIN_IN_ROWS_MIN_IN_ROWS_H_
#include <vector>

std::vector<int> randomMatrix(int cols, int rows);
std::vector<int> getParallelOperations(std::vector<int> global_mat, int cols, int rows);
std::vector<int> getSequentialOperations(std::vector<int> mat, int cols, int rows);

#endif  // MODULES_TASK_1_ANDRICH_M_MIN_IN_ROWS_MIN_IN_ROWS_H_
