// Copyright 2021 Poplavsky Anton
#ifndef MODULES_TASK_2_POPLAVSKY_A_GAUSS_JORDAN_GAUSS_JORDAN_H_
#define MODULES_TASK_2_POPLAVSKY_A_GAUSS_JORDAN_GAUSS_JORDAN_H_
#include <mpi.h>

#include <cstring>
#include <ctime>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

double* GaussJordan(int, int*, int, double*, double*, double*);

#endif  // MODULES_TASK_2_POPLAVSKY_A_GAUSS_JORDAN_GAUSS_JORDAN_H_
