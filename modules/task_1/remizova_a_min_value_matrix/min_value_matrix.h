// Copyright 2021 Remizova Antonina
#ifndef MODULES_TASK_1_REMIZOVA_A_MIN_VALUE_MATRIX_MIN_VALUE_MATRIX_H_
#define MODULES_TASK_1_REMIZOVA_A_MIN_VALUE_MATRIX_MIN_VALUE_MATRIX_H_

#include <mpi.h>
#include <random>
#include <vector>

std::vector<int> getRandMatrix(int cols, int rows);
int minElemSequentialOperations(std::vector<int> m, int cols, int rows);
int minElemParallelOperations(std::vector<int> m, int cols, int rows);

#endif  // MODULES_TASK_1_REMIZOVA_A_MIN_VALUE_MATRIX_MIN_VALUE_MATRIX_H_
