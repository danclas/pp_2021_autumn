// Copyright 2021 Kitaev Pavel

#ifndef MODULES_TASK_3_KITAEV_ALGORITHM_MOORES_KITAEV_ALGORITHM_MOORES_H_
#define MODULES_TASK_3_KITAEV_ALGORITHM_MOORES_KITAEV_ALGORITHM_MOORES_H_

#include <vector>

std::vector<int> GetRandomGraph(int size);
std::vector<int> SequentialMoores(std::vector<int>graph, int size);
std::vector<int> ParallelMoores(std::vector<int> graph, int size_graph);

#endif  // MODULES_TASK_3_KITAEV_ALGORITHM_MOORES_KITAEV_ALGORITHM_MOORES_H_
