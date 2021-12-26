// Copyright 2021 Medvedeva Karina
#ifndef MODULES_TASK_3_MEDVEDEVA_K_GAUSS_BLOCK_PARTITION_GAUSS_BLOCK_PARTITION_H_
#define MODULES_TASK_3_MEDVEDEVA_K_GAUSS_BLOCK_PARTITION_GAUSS_BLOCK_PARTITION_H_

#include <vector>

std::vector<int> getRandomMatrix(std::vector<int>::size_type row_count, std::vector<int>::size_type column_count);
std::vector<int> getParallelOperations(const std::vector<int>& matrix,
    std::vector<int>::size_type matrix_size);
std::vector<int> getSequentialOperations(const std::vector<int>& matrix,
    std::vector<int>::size_type row_count,
    std::vector<int>::size_type column_count);
void getGaussKernel(double sigma);

#endif  // MODULES_TASK_3_MEDVEDEVA_K_GAUSS_BLOCK_PARTITION_GAUSS_BLOCK_PARTITION_H_
