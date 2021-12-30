// Copyright 2021 Abdullin Konstantin
#ifndef MODULES_TASK_3_ABDULLIN_K_SHELL_SORT_BATCHER_MERGE_SHELL_SORT_BATCHER_MERGE_H_
#define MODULES_TASK_3_ABDULLIN_K_SHELL_SORT_BATCHER_MERGE_SHELL_SORT_BATCHER_MERGE_H_
#include <vector>

std::vector<double> RandomVector(int size);
std::vector <double> Shell_sort(std::vector <double> data);
void build_network(std::vector<int> ranks);
void create_connection(std::vector<int> up_ranks, std::vector<int> down_ranks);
std::vector<double> Parallel_sort(std::vector<double> data);

#endif  // MODULES_TASK_3_ABDULLIN_K_SHELL_SORT_BATCHER_MERGE_SHELL_SORT_BATCHER_MERGE_H_
