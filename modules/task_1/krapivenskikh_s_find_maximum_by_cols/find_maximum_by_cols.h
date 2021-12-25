// Copyright 2021 Krapivenskikh Stepan
#ifndef MODULES_TASK_1_KRAPIVENSKIKH_S_FIND_MAXIMUM_BY_COLS_FIND_MAXIMUM_BY_COLS_H_
#define MODULES_TASK_1_KRAPIVENSKIKH_S_FIND_MAXIMUM_BY_COLS_FIND_MAXIMUM_BY_COLS_H_

#include <vector>

std::vector<int> getRandomMatrix(size_t matrixSize);

std::vector<int> getSequentialFunction(std::vector<int> matrix, size_t matrixRows, size_t matrixCols);
std::vector<int> getParallelFunction(std::vector<int> global_matrix, size_t matrixRows, size_t matrixCols);

#endif  // MODULES_TASK_1_KRAPIVENSKIKH_S_FIND_MAXIMUM_BY_COLS_FIND_MAXIMUM_BY_COLS_H_
