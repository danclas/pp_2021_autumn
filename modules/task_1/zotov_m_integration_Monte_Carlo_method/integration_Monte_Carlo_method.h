// Copyright 2021 Zotov Maksim
#ifndef MODULES_TASK_1_ZOTOV_M_INTEGRATION_MONTE_CARLO_METHOD_INTEGRATION_MONTE_CARLO_METHOD_H_
#define MODULES_TASK_1_ZOTOV_M_INTEGRATION_MONTE_CARLO_METHOD_INTEGRATION_MONTE_CARLO_METHOD_H_


#include <vector>

double integralMonteCarlo(double a, double b, int n, double(*f)(double));

double integralParallel(double a, double b, int n, double(*f)(double));

double f1(double x);

double f2(double x);

double f3(double x);

double f4(double x);

double f5(double x);

#endif  // MODULES_TASK_1_ZOTOV_M_INTEGRATION_MONTE_CARLO_METHOD_INTEGRATION_MONTE_CARLO_METHOD_H_
