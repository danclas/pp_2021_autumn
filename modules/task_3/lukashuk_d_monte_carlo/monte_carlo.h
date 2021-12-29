// Copyright 2021 Lukashuk Diana
#ifndef MODULES_TASK_3_LUKASHUK_D_MONTE_CARLO_MONTE_CARLO_H_
#define MODULES_TASK_3_LUKASHUK_D_MONTE_CARLO_MONTE_CARLO_H_
#include <mpi.h>
#include <cmath>
#include <ctime>
#include <random>
#include <vector>
#include <iostream>
#include <functional>

#define function std::function<double(const std::vector<double>&)>

std::vector<double> getRandomNode(const std::vector<double>& lower,
                                  const std::vector<double>& upper, int points);
std::vector<double> upperLower(const std::vector<double>& lower,
                               const std::vector<double>& upper);
double getSequential(const std::vector<double>& upper_lover,
                     const std::vector<double>& node, int points,
                     const function& func);
double getParallel(const std::vector<double>& upper_lover,
                   const std::vector<double>& node, int points,
                   const function& func);

#endif  // MODULES_TASK_3_LUKASHUK_D_MONTE_CARLO_MONTE_CARLO_H_
