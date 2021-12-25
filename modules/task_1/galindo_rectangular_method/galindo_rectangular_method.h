// copyright 2021 Javier Galindo
#ifndef MODULES_TASK_1_GALINDO_RECTANGULAR_METHOD_GALINDO_RECTANGULAR_METHOD_H_
#define MODULES_TASK_1_GALINDO_RECTANGULAR_METHOD_GALINDO_RECTANGULAR_METHOD_H_

#include <mpi.h>
#include <cmath>
#include <functional>
#include <stdexcept>

double integralSeqential(std::function<double(double)> integrable_function, double a, double b, size_t count);
double integralParallel(std::function<double(double)> integrable_function, double a, double b, size_t count);


#endif  // MODULES_TASK_1_GALINDO_RECTANGULAR_METHOD_GALINDO_RECTANGULAR_METHOD_H_

