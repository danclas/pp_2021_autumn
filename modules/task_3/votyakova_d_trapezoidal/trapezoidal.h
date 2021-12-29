// Copyright 2021 Votyakova Daria
#ifndef MODULES_TASK_3_VOTYAKOVA_D_TRAPEZOIDAL_TRAPEZOIDAL_H_
#define MODULES_TASK_3_VOTYAKOVA_D_TRAPEZOIDAL_TRAPEZOIDAL_H_

#include <cmath>
#include <functional>
#include <utility>
#include <vector>

using std::function;
using std::pair;
using std::vector;

typedef vector<pair<double, double>> ABs;
typedef function<double(vector<double>)> func;

double SequentialTrapezoidal(int segments, ABs ab, func fn);
double ParallelTrapezoidal(int segments, ABs ab, const func fn);
#endif  // MODULES_TASK_3_VOTYAKOVA_D_TRAPEZOIDAL_TRAPEZOIDAL_H_
