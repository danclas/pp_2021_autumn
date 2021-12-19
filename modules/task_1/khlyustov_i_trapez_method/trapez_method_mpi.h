// Copyright 2021 Khlyustov Ilya
#ifndef MODULES_TASK_1_KHLYUSTOV_I_TRAPEZ_METHOD_TRAPEZ_METHOD_MPI_H_
#define MODULES_TASK_1_KHLYUSTOV_I_TRAPEZ_METHOD_TRAPEZ_METHOD_MPI_H_

#include <mpi.h>
#include <cmath>
#include <functional>

double getParallel(double a, double b, int n, const std::function<double(double)>& f);
double getSequential(double a, double b, int n, const std::function<double(double)>& f);

#endif  // MODULES_TASK_1_KHLYUSTOV_I_TRAPEZ_METHOD_TRAPEZ_METHOD_MPI_H_
