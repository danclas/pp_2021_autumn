// Copyright 2021 Preobrazhenskaya Yuliya
#ifndef MODULES_TASK_3_PREOBRAZHENSKAYA_Y_MULTI_DIMENSION_MONTE_CARLO_MULTI_DIMENSION_MONTE_CARLO_H_
#define MODULES_TASK_3_PREOBRAZHENSKAYA_Y_MULTI_DIMENSION_MONTE_CARLO_MULTI_DIMENSION_MONTE_CARLO_H_
#include <mpi.h>
#include <ctime>
#include <random>
#include <vector>
#include <functional>

double getParallelOperations(const std::vector<double>& lower,
    const std::vector<double>& upper,
    int number_of_points,
    const std::function<double(const std::vector<double>&)>& func);
double getSequentialOperations(const std::vector<double>& lower,
    const std::vector<double>& upper,
    int number_of_points,
    const std::function<double(const std::vector<double>&)>& func);

#endif  // MODULES_TASK_3_PREOBRAZHENSKAYA_Y_MULTI_DIMENSION_MONTE_CARLO_MULTI_DIMENSION_MONTE_CARLO_H_
