// Copyright 2021 Barysheva Maria
#ifndef MODULES_TASK_2_BARYSHEVA_M_TAPE_HOR_CHART_TAPE_HOR_CHART_H_
#define MODULES_TASK_2_BARYSHEVA_M_TAPE_HOR_CHART_TAPE_HOR_CHART_H_

#include <vector>

using std::vector;

vector<int> getRandomMatrix(int n, int m);
vector<int> SequentialMatrixMultiplication(vector<int> A, vector<int> B,
                                           int A_n, int A_m, int B_m);
vector<int> ParallelMatrixMultiplication(vector<int> A, vector<int> B, int A_n,
                                         int A_m);

#endif  // MODULES_TASK_2_BARYSHEVA_M_TAPE_HOR_CHART_TAPE_HOR_CHART_H_
