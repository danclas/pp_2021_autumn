// Copyright 2021 Bakina Ksenia
#ifndef MODULES_TASK_3_BAKINA_K_RADIX_SORT_SIMPLE_MERGE_RADIX_SORT_SIMPLE_MERGE_H_
#define MODULES_TASK_3_BAKINA_K_RADIX_SORT_SIMPLE_MERGE_RADIX_SORT_SIMPLE_MERGE_H_
#include <mpi.h>
#include <ctime>
#include <vector>
#include <iostream>
#include <algorithm>

void get_random_vector(std::vector<int>* vector);
std::vector<int> merge(const std::vector<int>& array1, const std::vector<int>& array2);
void sort_of_rank(std::vector<int>* array, int64_t rank);
void linear_radix_sort(std::vector<int>* array);
std::vector<int> parallel_radix_sort(const std::vector<int>& array);

#endif  // MODULES_TASK_3_BAKINA_K_RADIX_SORT_SIMPLE_MERGE_RADIX_SORT_SIMPLE_MERGE_H_
