// Copyright 2021 Shelepin Nikita
#ifndef MODULES_TASK_3_SHELEPIN_N_SIMPSON_SIMPSON_H_
#define MODULES_TASK_3_SHELEPIN_N_SIMPSON_SIMPSON_H_

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

double getSequentialSimpson(
    const std::function<double(std::vector<double>)>& f,
    const std::vector<std::pair<double, double>>& limits,
    const std::vector<int>& n);

double getParallelSimpson(const std::function<double(std::vector<double>)>& f,
                          const std::vector<std::pair<double, double>>& limits,
                          const std::vector<int>& n);

#endif  // MODULES_TASK_3_SHELEPIN_N_SIMPSON_SIMPSON_H_
