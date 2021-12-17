// Copyright 2021 Zaitseva Ksenia
#ifndef MODULES_TASK_1_ZAITSEVA_K_CONJUGATE_GRADIENT_METHOD_H_
#define MODULES_TASK_1_ZAITSEVA_K_CONJUGATE_GRADIENT_METHOD_H_
#include <mpi.h>

#include <iostream>
#include <random>
#include <vector>

void RandomVec(int n, double* A);
double ScalarMult(std::vector<double> x, std::vector<double> y);
std::vector<double> conjugateGradientMethodSerial(double* A, double* b, int n);
std::vector<double> conjugateGradientMethodParallel(double* A, double* b,
                                                    int n);

#endif  // MODULES_TASK_1_ZAITSEVA_K_CONJUGATE_GRADIENT_METHOD_H_
