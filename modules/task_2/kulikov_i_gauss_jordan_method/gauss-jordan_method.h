// Copyright 2021 Kulikov Ilya
#ifndef MODULES_TASK_2_KULIKOV_I_GAUSS_JORDAN_METHOD_GAUSS_JORDAN_METHOD_H_
#define MODULES_TASK_2_KULIKOV_I_GAUSS_JORDAN_METHOD_GAUSS_JORDAN_METHOD_H_


double* get_random_matrix(int* num_vec, int sz);
double* division_in_str(double* str, double* subtrahend,
    double* coeffs, int sz, int part);
double* gauss_jordan_finding(double* str, int sz);

#endif  // MODULES_TASK_2_KULIKOV_I_GAUSS_JORDAN_METHOD_GAUSS_JORDAN_METHOD_H_
