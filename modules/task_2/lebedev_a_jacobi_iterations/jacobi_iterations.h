// Copyright 2021 Lebedev Alexey
#ifndef MODULES_TASK_2_LEBEDEV_A_JACOBI_ITERATIONS_JACOBI_ITERATIONS_H_
#define MODULES_TASK_2_LEBEDEV_A_JACOBI_ITERATIONS_JACOBI_ITERATIONS_H_


#include "./tensor.h"


class LinearSystem {
 public:
     const int n_dims;
     const Tensor<float> A;
     const Tensor<float> b;
     const Tensor<float> x0;
     explicit LinearSystem(const int& _n_dims): n_dims(_n_dims),
                                                   A({_n_dims, _n_dims}),
                                                   b({_n_dims, 1}),
                                                   x0({_n_dims, 1})
                                                   {}
     Tensor<float> solve(const float accuracy);
};


Tensor<float> solve_parallel(const LinearSystem& sys, const float accuracy);


#endif  // MODULES_TASK_2_LEBEDEV_A_JACOBI_ITERATIONS_JACOBI_ITERATIONS_H_
