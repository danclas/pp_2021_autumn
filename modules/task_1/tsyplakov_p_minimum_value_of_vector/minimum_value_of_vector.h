// Copyright 2021 Tsyplakov Pavel
#ifndef MODULES_TASK_1_TSYPLAKOV_P_MINIMUM_VALUE_OF_VECTOR_MINIMUM_VALUE_OF_VECTOR_H_
#define MODULES_TASK_1_TSYPLAKOV_P_MINIMUM_VALUE_OF_VECTOR_MINIMUM_VALUE_OF_VECTOR_H_

#include <vector>

std::vector<int> getRandomVector(const std::vector<int>::size_type sz);
int getParallelOperations(const std::vector<int>& global_vec, const std::vector<int>::size_type count_size_vector);
int getSequentialOperations(const std::vector<int>& vec);

#endif  // MODULES_TASK_1_TSYPLAKOV_P_MINIMUM_VALUE_OF_VECTOR_MINIMUM_VALUE_OF_VECTOR_H_
