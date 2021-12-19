// Copyright 2021 Dydykin Pavel
#ifndef MODULES_TASK_2_DYDYKIN_P_GAUSS_HORIZONTAL_GAUSS_HORIZONTAL_H_
#define MODULES_TASK_2_DYDYKIN_P_GAUSS_HORIZONTAL_GAUSS_HORIZONTAL_H_
#include <vector>

std::vector<double> getRandomMatrix(int m, int n);
std::vector<double> GaussNotParallel(const std::vector<double>& matrix,
    int rows, int cols);
std::vector<double> GaussParallel(const std::vector<double>& matrix,
    int rows, int cols);

#endif  // MODULES_TASK_2_DYDYKIN_P_GAUSS_HORIZONTAL_GAUSS_HORIZONTAL_H_
