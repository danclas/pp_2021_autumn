// Copyright 2021 Feoktistov Andrey
#ifndef MODULES_TASK_1_FEOKTISTOV_A_VECTOR_AVG_VECTOR_AVG_H_
#define MODULES_TASK_1_FEOKTISTOV_A_VECTOR_AVG_VECTOR_AVG_H_

#include <string>
#include <vector>

std::vector<int> generateRandomPositiveVector(int sz);
int calcParallelAverage(std::vector<int> global_vec, int count_size_vector);
int calcSequentialAverage(std::vector<int> vec);

#endif  // MODULES_TASK_1_FEOKTISTOV_A_VECTOR_AVG_VECTOR_AVG_H_
