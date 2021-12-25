// Copyright 2021 Pyatckin Nickolay
#ifndef MODULES_TASK_1_PYATCKIN_N_AVG_OF_VECTOR_AVG_OF_VECTOR_H_
#define MODULES_TASK_1_PYATCKIN_N_AVG_OF_VECTOR_AVG_OF_VECTOR_H_

#include <vector>
#include <string>

std::vector<double> getRandomVector(int  sz);
double getParallelOperations(std::vector<double>& vec,
	int size_vec);
double getSequentialOperations(std::vector<double>& vec, int size_vec);

#endif  // MODULES_TASK_1_PYATCKIN_N_AVG_OF_VECTOR_AVG_OF_VECTOR_H_
