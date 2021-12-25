// Copyright 2021 Pyatckin Nikolai
#ifndef MODULES_TASK_3_PYATCKIN_N_INTEGRALS_RECTANGLE_INTEGRALS_RECTANGLE_H_
#define MODULES_TASK_3_PYATCKIN_N_INTEGRALS_RECTANGLE_INTEGRALS_RECTANGLE_H_
#include <vector>
#include <algorithm>
#include <utility>
#include <functional>

double ParallelIntegral(std::function<double(const std::vector<double>)> f,
    std::vector <std::pair<double, double>> v, const int num = 100);
double SeqIntegral(std::function<double(const std::vector<double>)> f,
    std::vector <std::pair<double, double>> v, const int num = 100);

#endif  // MODULES_TASK_3_PYATCKIN_N_INTEGRALS_RECTANGLE_INTEGRALS_RECTANGLE_H_
