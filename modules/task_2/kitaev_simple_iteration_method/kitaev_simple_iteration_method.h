// Copyright 2021 Kitaev Pavel

#ifndef MODULES_TASK_2_KITAEV_SIMPLE_ITERATION_METHOD_KITAEV_SIMPLE_ITERATION_METHOD_H_
#define MODULES_TASK_2_KITAEV_SIMPLE_ITERATION_METHOD_KITAEV_SIMPLE_ITERATION_METHOD_H_

#include <vector>

std::vector<double> SequentialSimpleIterationMethod(std::vector<double> matrix,
  std::vector<double> vect, double prec);
std::vector<double> ParallelSimpleIterationMethod(std::vector<double> matrix,
  std::vector<double> vect, double prec);

#endif  // MODULES_TASK_2_KITAEV_SIMPLE_ITERATION_METHOD_KITAEV_SIMPLE_ITERATION_METHOD_H_
