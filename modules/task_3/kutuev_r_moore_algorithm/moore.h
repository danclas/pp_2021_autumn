// Copyright 2021 Kutuev Roman
#ifndef MODULES_TASK_3_KUTUEV_R_MOORE_ALGORITHM_MOORE_H_
#define MODULES_TASK_3_KUTUEV_R_MOORE_ALGORITHM_MOORE_H_

#include <mpi.h>

#include <random>
#include <vector>
const int INF = 1e9;

std::vector<int> random(int size);
std::vector<int> moore_algorithm(const std::vector<int>& graph, int size);
std::vector<int> moore_parallel(const std::vector<int>& graph, int size);

#endif  // MODULES_TASK_3_KUTUEV_R_MOORE_ALGORITHM_MOORE_H_
