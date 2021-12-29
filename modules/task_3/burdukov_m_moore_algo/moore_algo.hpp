// Copyright 2021 Burdukov Mikhail
#ifndef MODULES_TASK_3_BURDUKOV_M_MOORE_ALGO_MOORE_ALGO_HPP_
#define MODULES_TASK_3_BURDUKOV_M_MOORE_ALGO_MOORE_ALGO_HPP_

#include <mpi.h>
#define INF 1000000
#include <random>
#include <vector>

#define matrix_t std::vector<std::vector<int>>

int moore_algorithm(const int start, const int end,
                    const matrix_t& adjacency_matrix, const int size);

#endif  // MODULES_TASK_3_BURDUKOV_M_MOORE_ALGO_MOORE_ALGO_HPP_
