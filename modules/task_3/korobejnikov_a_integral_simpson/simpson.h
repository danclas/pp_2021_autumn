// Copyright 2021 Korobejnikov Aleksandr
#ifndef MODULES_TASK_3_KOROBEJNIKOV_A_INTEGRAL_SIMPSON_SIMPSON_H_
#define MODULES_TASK_3_KOROBEJNIKOV_A_INTEGRAL_SIMPSON_SIMPSON_H_
#include <vector>
#include <algorithm>
#include <utility>

double ParallelVersion(double (*func)(std::vector<double>),
        std::vector <std::pair<double, double>> v, std::vector <int> distr);
double SequentialVersion(double (*func)(std::vector<double>),
        std::vector <std::pair<double, double>> v, std::vector <int> distr);

#endif  // MODULES_TASK_3_KOROBEJNIKOV_A_INTEGRAL_SIMPSON_SIMPSON_H_
