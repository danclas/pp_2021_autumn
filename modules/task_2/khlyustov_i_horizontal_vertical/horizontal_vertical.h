// Copyright 2021 Khlyustov Ilya
#ifndef MODULES_TASK_2_KHLYUSTOV_I_HORIZONTAL_VERTICAL_HORIZONTAL_VERTICAL_H_
#define MODULES_TASK_2_KHLYUSTOV_I_HORIZONTAL_VERTICAL_HORIZONTAL_VERTICAL_H_

#include <mpi.h>
#include <vector>

#define sizeType std::vector<int>::size_type
#define Matrix std::vector<int>

Matrix getRandomMatrix(sizeType row_count, sizeType col_count);
Matrix getSequential(const Matrix& matrix1, const Matrix& matrix2,
    sizeType row_matrix1, sizeType col_matrix1, sizeType col_matrix2);
Matrix getParallel(const Matrix& matrix1, const Matrix& matrix2,
    sizeType row_matrix1, sizeType col_matrix1);

#endif  // MODULES_TASK_2_KHLYUSTOV_I_HORIZONTAL_VERTICAL_HORIZONTAL_VERTICAL_H_
