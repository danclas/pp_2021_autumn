  // Copyright 2021 Elanskiy Akexandr
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <gtest-mpi-listener.hpp>

#ifndef MODULES_TASK_1_ELANSKIY_A_MAX_VALUES_IN_ROWS_MAX_VALUES_IN_ROWS_H_
#define MODULES_TASK_1_ELANSKIY_A_MAX_VALUES_IN_ROWS_MAX_VALUES_IN_ROWS_H_

std::vector<int> mat_gen(int rows, int cols);
std::vector<int> max_in_rows_seq(std::vector<int> m, int cols);
std::vector<int> max_in_rows_par(std::vector<int> m, int rows, int cols);

#endif  // MODULES_TASK_1_ELANSKIY_A_MAX_VALUES_IN_ROWS_MAX_VALUES_IN_ROWS_H_
