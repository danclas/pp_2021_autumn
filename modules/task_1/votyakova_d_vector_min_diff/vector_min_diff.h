// Copyright 2021 Votyakova Darya
#ifndef MODULES_TASK_1_VOTYAKOVA_D_VECTOR_MIN_DIFF_VECTOR_MIN_DIFF_H_
#define MODULES_TASK_1_VOTYAKOVA_D_VECTOR_MIN_DIFF_VECTOR_MIN_DIFF_H_

#include <vector>

std::vector<int> getRandomVector(int sz);
int* getParallelMinDiff(std::vector<int> global_vec);
int* getSequentialMinDiff(std::vector<int> vec);

#endif  // MODULES_TASK_1_VOTYAKOVA_D_VECTOR_MIN_DIFF_VECTOR_MIN_DIFF_H_
