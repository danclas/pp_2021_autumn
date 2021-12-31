// Copyright [2021] <Borisov Maxim>
#ifndef MODULES_TASK_3_BORISOV_M_MONTE_CARLO_INTEGRATION_MONTE_CARLO_INTEGRATION_H_
#define MODULES_TASK_3_BORISOV_M_MONTE_CARLO_INTEGRATION_MONTE_CARLO_INTEGRATION_H_

#include <mpi.h>
#include <functional>
#include <vector>
#include <random>

double sequential_monte_carlo(
    std::vector<double> up, std::vector<double> lo, int points_size,
    const std::function<double(const std::vector<double> &)> &f);

double parallel_monte_carlo(
    const std::vector<double> &up, const std::vector<double> &lo,
    int points_size,
    const std::function<double(const std::vector<double> &)> &f);

#endif  // MODULES_TASK_3_BORISOV_M_MONTE_CARLO_INTEGRATION_MONTE_CARLO_INTEGRATION_H_
