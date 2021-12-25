// Copyright 2021 Ershov Alexey
#ifndef MODULES_TASK_1_ERSHOV_A_MATRIX_MIN_LINE_MATRIX_MIN_LINE_MPI_H_
#define MODULES_TASK_1_ERSHOV_A_MATRIX_MIN_LINE_MATRIX_MIN_LINE_MPI_H_

#include <vector>

std::vector<double> getRandomVector(const std::vector<int>::size_type h,
                                    const std::vector<int>::size_type w);
std::vector<double> getSequentialMatrixMinLine(
    const std::vector<double>& matrix, std::vector<double>::size_type row_count,
    std::vector<double>::size_type column_count);
std::vector<double> getParallelMatrixMinLine(
    const std::vector<double>& matrix, std::vector<double>::size_type row_count,
    std::vector<double>::size_type column_count);

#endif  // MODULES_TASK_1_ERSHOV_A_MATRIX_MIN_LINE_MATRIX_MIN_LINE_MPI_H_
