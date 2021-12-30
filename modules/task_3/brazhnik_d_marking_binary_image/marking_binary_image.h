// Copyright 2021 Brazhnik Dmitry
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include <utility>
#include <ctime>
#include <random>
#include <vector>
#include <gtest-mpi-listener.hpp>

#ifndef MODULES_TASK_3_BRAZHNIK_D_MARKING_BINARY_IMAGE_MARKING_BINARY_IMAGE_H_
#define MODULES_TASK_3_BRAZHNIK_D_MARKING_BINARY_IMAGE_MARKING_BINARY_IMAGE_H_

std::vector<int> generate_binary_image(int w, int h);

std::pair<std::vector<int>, std::pair<std::vector<int>, int>>
step_first(const std::vector<int>& data, int w, int h, int startMarked = 0);
std::vector<int> step_second(std::vector<int> tmpMap, int w, int h, std::vector<int> nonoverlapping);
std::vector<int> set_mark(const std::vector<int>& data, int w, int h);
std::pair<std::vector<int>, int> basic_marking_binary_image(const std::vector<int>& data, int w, int h);
std::pair<std::vector<int>, int> parallel_marking_binary_image(const std::vector<int>& data, int w, int h);

#endif  // MODULES_TASK_3_BRAZHNIK_D_MARKING_BINARY_IMAGE_MARKING_BINARY_IMAGE_H_
