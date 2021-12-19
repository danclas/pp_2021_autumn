// Copyright 2021 Sablin Alexander

#ifndef MODULES_TASK_3_SABLIN_A_TRAPEZOID_METHOD_TRAPEZOID_METHOD_H_
#define MODULES_TASK_3_SABLIN_A_TRAPEZOID_METHOD_TRAPEZOID_METHOD_H_

double trapezoidMethodSequential(double (*f)(double, double), double a_x, double b_x,
    double a_y, double b_y, const int n);
double trapezoidMethodParallel(double (*f)(double, double), double a_x, double b_x,
    double a_y, double b_y, const int n);

#endif  //  MODULES_TASK_3_SABLIN_A_TRAPEZOID_METHOD_TRAPEZOID_METHOD_H_
