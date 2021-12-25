// Copyright 2021 Belyaev Ilya
#ifndef MODULES_TASK_1_BELYAEV_I_TRAPEZOID_METHOD1_TRAPEZOID_METHOD_H_
#define MODULES_TASK_1_BELYAEV_I_TRAPEZOID_METHOD1_TRAPEZOID_METHOD_H_

double getParallel(double a, double b, int n,
    const std::function<double(double)>& f);
double getIntegral(double a, double b, int n,
    const std::function<double(double)>& f);

#endif  // MODULES_TASK_1_BELYAEV_I_TRAPEZOID_METHOD1_TRAPEZOID_METHOD_H_
