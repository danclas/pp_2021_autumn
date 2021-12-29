// Copyright 2021 Tsyplakov Pavel
#ifndef MODULES_TASK_3_TSYPLAKOV_P_RADIX_SORT_W_MERGE_SORT_RADIX_SORT_W_MERGE_SORT_H_
#define MODULES_TASK_3_TSYPLAKOV_P_RADIX_SORT_W_MERGE_SORT_RADIX_SORT_W_MERGE_SORT_H_

#include <vector>
#include <string>

std::vector<double> getRandomVector(int sz);

std::vector<double> ParallelRadixSort(const std::vector<double> global_vec,
              const std::vector<double>::size_type count_size_vector);
std::vector<double> SequentialRadixSort(const std::vector<double>& vec);

#endif  // MODULES_TASK_3_TSYPLAKOV_P_RADIX_SORT_W_MERGE_SORT_RADIX_SORT_W_MERGE_SORT_H_
