// Copyright 2021 Fedoseev Mikhail
#ifndef MODULES_TASK_2_FEDOSEEV_M_ODD_EVEN_ODD_EVEN_H_
#define MODULES_TASK_2_FEDOSEEV_M_ODD_EVEN_ODD_EVEN_H_

#include <vector>

std::vector<int> getRandomVector(int sz);
std::vector<int> MergeSort(std::vector<int> v1, std::vector<int> v2);
void SequentialOddEvenSort(std::vector<int>* vec);
void ParallelOddEvenSort(std::vector<int>* arr);

#endif  // MODULES_TASK_2_FEDOSEEV_M_ODD_EVEN_ODD_EVEN_H_
