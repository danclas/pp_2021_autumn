// Copyright 2021 Fedoseyev Mikhail
#ifndef MODULES_TASK_1_FEDOSEEV_M_INTEGRAL_RECTANGLE_METHOD_INTEGRAL_RECTANGLE_METHOD_H_
#define MODULES_TASK_1_FEDOSEEV_M_INTEGRAL_RECTANGLE_METHOD_INTEGRAL_RECTANGLE_METHOD_H_
#include <mpi.h>

#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

double IntegralSequential(double x1, double y1, int z1, int f_i1);
double IntegralParallel(double x2, double y2, int z2, int f_i2);

#endif  // MODULES_TASK_1_FEDOSEEV_M_INTEGRAL_RECTANGLE_METHOD_INTEGRAL_RECTANGLE_METHOD_H_
