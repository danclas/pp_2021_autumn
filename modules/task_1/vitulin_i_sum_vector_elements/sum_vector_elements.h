// Copyright 2021 Vitulin Ivan 381908-1
#ifndef MODULES_TASK_1_VITULIN_I_SUM_VECTOR_ELEMENTS_SUM_VECTOR_ELEMENTS_H_
#define MODULES_TASK_1_VITULIN_I_SUM_VECTOR_ELEMENTS_SUM_VECTOR_ELEMENTS_H_

#include <vector>

std::vector<int> getRandomVectorElements(int vector_size, int k);
int getSumOfVectorElementsSequentially(std::vector<int> vect);
int getSumOfVectorElementsParallelly(std::vector<int> global_vector, int count_size_vec);

#endif  // MODULES_TASK_1_VITULIN_I_SUM_VECTOR_ELEMENTS_SUM_VECTOR_ELEMENTS_H_
