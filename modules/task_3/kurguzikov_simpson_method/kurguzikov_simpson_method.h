// Copyright 2021 Kurguzikov Kirill
#ifndef MODULES_TASK_3_KOROBEJNIKOV_A_SIMPSON_SIMPSON_H_
#define MODULES_TASK_3_KOROBEJNIKOV_A_SIMPSON_SIMPSON_H_
#include <vector>
#include <algorithm>
#include <utility>

double ParallelVersion(double (*func)(std::vector<double>),
        std::vector <std::pair<double, double>> v, std::vector <int> distr);
double SequentialVersion(double (*func)(std::vector<double>),
        std::vector <std::pair<double, double>> v, std::vector <int> distr);

#endif  // MODULES_TASK_3_KURGUZIKOV_SIMPSON_METHOD_KURGUZIKOV_SIMPSON_METHOD_H_
