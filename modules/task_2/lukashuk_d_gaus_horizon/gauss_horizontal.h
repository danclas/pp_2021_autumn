// Copyright 2021 Lukashuk Diana
#ifndef MODULES_TASK_2_LUKASHUK_D_GAUS_HORIZON_GAUSS_HORIZONTAL_H_
#define MODULES_TASK_2_LUKASHUK_D_GAUS_HORIZON_GAUSS_HORIZONTAL_H_
#include <mpi.h>

#include <ctime>
#include <iostream>
#include <random>
#include <vector>

std::vector<double> getRandomMatrix(int m, int n, int proc_rank = 0,
                                    int proc_count = 1);
std::vector<double> seqGauss(const std::vector<double>& matrix, int rows,
                             int cols);
std::vector<double> parGauss(const std::vector<double>& matrix, int rows,
                             int cols);

#endif  //  MODULES_TASK_2_LUKASHUK_D_GAUS_HORIZON_GAUSS_HORIZONTAL_H_
