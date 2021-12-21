// Copyright 2021 Bakina Ksenia
#ifndef MODULES_TASK_1_BAKINA_K_MATRIX_MAX_MAX_MATRIX_H_
#define MODULES_TASK_1_BAKINA_K_MATRIX_MAX_MAX_MATRIX_H_
#include <vector>

void get_random_matrix(std::vector<std::vector<int>>* mtrx, const int row_count, const int column_count);
int linear_search_max_el(std::vector<int>* sub_matrix, const int size);
int parallel_search_max_el(std::vector<std::vector<int>>* matrix, int row_count, int column_count);

#endif  // MODULES_TASK_1_BAKINA_K_MATRIX_MAX_MAX_MATRIX_H_
