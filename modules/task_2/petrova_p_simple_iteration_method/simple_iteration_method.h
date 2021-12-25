// Copyright 2021 Petrova Polina
#ifndef MODULES_TASK_2_PETROVA_P_SIMPLE_ITERATION_METHOD_SIMPLE_ITERATION_METHOD_H_
#define MODULES_TASK_2_PETROVA_P_SIMPLE_ITERATION_METHOD_SIMPLE_ITERATION_METHOD_H_

#include <vector>

std::vector<double> fillRandomVector(int len);
std::vector< std::vector<double>> fillRandomMatrix(int n);
std::vector<double> seqMethod(std::vector< std::vector<double>> matrix,
    std::vector<double> b, int n);
std::vector<double> parallelMethod(const std::vector<std::vector<double> > &matrix,
    std::vector<double> b, int n);
std::vector<double> Convert(std::vector<std::vector<double> > M, int n);

#endif  // MODULES_TASK_2_PETROVA_P_SIMPLE_ITERATION_METHOD_SIMPLE_ITERATION_METHOD_H_
