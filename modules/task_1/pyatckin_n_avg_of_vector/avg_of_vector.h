// Copyright 2021 Pyatckin Nickolay
#ifndef MODULES_TASK_1_PYATCKIN_N_AVG_OF_VECTOR_AVG_OF_VECTOR_H_
#define MODULES_TASK_1_PYATCKIN_N_AVG_OF_VECTOR_AVG_OF_VECTOR_H_

#include <vector>

std::vector<int> getRandomVector(int  sz);
int getParallelOperations(std::vector<int>& vec,
    int size_vec);
int getSequentialOperations(std::vector<int>& vec, int size_vec);

#endif  // MODULES_TASK_1_PYATCKIN_N_AVG_OF_VECTOR_AVG_OF_VECTOR_H_
