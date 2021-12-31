// Copyright 2021 Ivina Anastasiya
#ifndef MODULES_TASK_3_IVINA_A_ALG_MOORE_ALG_MOORE_H_
#define MODULES_TASK_3_IVINA_A_ALG_MOORE_ALG_MOORE_H_

#include <vector>
#include <utility>

int randomValue(int min, int max);
std::vector<std::vector<int>> par_shortestPathFaster(
    const std::vector<std::vector<std::pair<int, int>>> &graph, const int S, const int V);

#endif  // MODULES_TASK_3_IVINA_A_ALG_MOORE_ALG_MOORE_H_
