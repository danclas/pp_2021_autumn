// Copyright 2021 Votyakova Daria
#ifndef MODULES_TASK_2_VOTYAKOVA_D_JACOBY_METHOD_JACOBY_METHOD_H_
#define MODULES_TASK_2_VOTYAKOVA_D_JACOBY_METHOD_JACOBY_METHOD_H_

#include <vector>

const double eps = 0.0001;
const int max_iterations = 100;

std::vector<double> getDiagonallyDominantMatrix(int size);
std::vector<double> getB(int size);

double getError(std::vector<double> A, std::vector<double> x,
                std::vector<double> b);

std::vector<double> getJacobiSequential(std::vector<double> A,
                                        std::vector<double> b, int size);
std::vector<double> getJacobiParallel(std::vector<double> A,
                                      std::vector<double> b, int size);

#endif  // MODULES_TASK_2_VOTYAKOVA_D_JACOBY_METHOD_JACOBY_METHOD_H_
