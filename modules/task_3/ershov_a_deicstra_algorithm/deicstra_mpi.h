// Copyright 2021 Ershov Aleksey
#ifndef MODULES_TASK_3_ERSHOV_A_DEICSTRA_ALGORITHM_DEICSTRA_MPI_H_
#define MODULES_TASK_3_ERSHOV_A_DEICSTRA_ALGORITHM_DEICSTRA_MPI_H_

#include <string>
#include <vector>

std::vector<std::vector<int>> getRandomVector(const size_t count);

std::vector<int> getDeicstra(const std::vector<std::vector<int>>& graf,
                             const size_t count, const size_t top);
std::vector<int> getParallelDeicstra(const std::vector<std::vector<int>>& graf,
                                     const size_t count);
std::vector<int> getSequentialDeicstra(
    const std::vector<std::vector<int>>& graf, const size_t count);

#endif  // MODULES_TASK_3_ERSHOV_A_DEICSTRA_ALGORITHM_DEICSTRA_MPI_H_
