// Copyright 2021 Narlyyev Parahat
#ifndef MODULES_TASK_1_NARLYYEV_P_MAXVEC_NARLYYEV_P_MAXVEC_H_
#define MODULES_TASK_1_NARLYYEV_P_MAXVEC_NARLYYEV_P_MAXVEC_H_
#include <vector>
#include <string>

std::vector<std::vector<double>> getRandomMatrix(int m, int n);

std::vector<double> getRandomMatrixInVector(int size);

void transpose(std::vector<std::vector<double>>* a);

std::vector<double> matrix_to_vector(std::vector<std::vector<double>> a,
     int m, int n);

std::vector<double> max_el_in_dif_intervals_in_vector(std::vector<double> a,
     int n);

std::vector<double> getSeqOperations(std::vector<std::vector<double>> a);

std::vector<double> getParallelOperations(std::vector<std::vector<double>> a,
     int m, int n);
#endif  // MODULES_TASK_1_NARLYYEV_P_MAXVEC_NARLYYEV_P_MAXVEC_H_
