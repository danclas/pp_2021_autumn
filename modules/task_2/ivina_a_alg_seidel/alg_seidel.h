// Copyright 2021 Ivina Anastasiya
#ifndef MODULES_TASK_2_IVINA_A_ALG_SEIDEL_ALG_SEIDEL_H_
#define MODULES_TASK_2_IVINA_A_ALG_SEIDEL_ALG_SEIDEL_H_

#include <vector>

double randomValue(int min, int max);
std::vector<double> seqSeidel(const std::vector<std::vector<double>> &a, const std::vector<double> &b, double eps);
std::vector<double> parallSeidel(const std::vector<std::vector<double>> &a, const std::vector<double> &b, double eps);

#endif  // MODULES_TASK_2_IVINA_A_ALG_SEIDEL_ALG_SEIDEL_H_
