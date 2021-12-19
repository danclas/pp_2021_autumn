// Copyright 2021 Abuyassen Albara

#ifndef MODULES_TASK_1_ABUYASSEN_A_MIN_VEC_MIN_VEC_H_
#define MODULES_TASK_1_ABUYASSEN_A_MIN_VEC_MIN_VEC_H_

#include <vector>

std::vector<int> getRandomVector(int  sz);
int getParallelOperations(std::vector<int> global_vec,
    int count_size_vector);
int getSequentialOperations(std::vector<int> vec);

#endif  // MODULES_TASK_1_ABUYASSEN_A_MIN_VEC_MIN_VEC_H_
