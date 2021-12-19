// Copyright 2021 Elanskiy Akexandr
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <random>
#include <vector>
#include <numeric>
#include <gtest-mpi-listener.hpp>

#ifndef MODULES_TASK_3_ELANSKIY_A_CONVEX_HULL_CONSTRUCTION_JARVIS_CONVEX_HULL_CONSTRUCTION_JARVIS_H_
#define MODULES_TASK_3_ELANSKIY_A_CONVEX_HULL_CONSTRUCTION_JARVIS_CONVEX_HULL_CONSTRUCTION_JARVIS_H_

std::vector<std::pair<int, int>> gen_dots(int vectorSize);
int rotation(const std::pair<int, int>& firstVertex, const std::pair<int, int>& secondVertex,
    const std::pair<int, int>& thirdVertex);
std::vector<std::pair<int, int>> jarvis_seq(const std::vector<std::pair<int, int>>& vectorOfVertex);
std::vector<std::pair<int, int>> jarvis_par(const std::vector<std::pair<int, int>>& vectorOfVertex, int vectorSize);

#endif  // MODULES_TASK_3_ELANSKIY_A_CONVEX_HULL_CONSTRUCTION_JARVIS_CONVEX_HULL_CONSTRUCTION_JARVIS_H_
