// Copyright 2021 Zotov Maksim
#ifndef MODULES_TASK_2_ZOTOV_M_BUBBLE_SORT_ODD_EVEN_ALGORITHM_BUBBLE_SORT_ODD_EVEN_ALGORITHM_H_
#define MODULES_TASK_2_ZOTOV_M_BUBBLE_SORT_ODD_EVEN_ALGORITHM_BUBBLE_SORT_ODD_EVEN_ALGORITHM_H_

#include <vector>

std::vector<int> BubbleSortSequential(std::vector<int> pData);

std::vector<int> getRandomVector(int size);

std::vector<int> BubbleSortParallel(std::vector<int> pData);

void mergAndSort(std::vector<int>* const firstArr, std::vector<int>* const secondArr);


#endif  // MODULES_TASK_2_ZOTOV_M_BUBBLE_SORT_ODD_EVEN_ALGORITHM_BUBBLE_SORT_ODD_EVEN_ALGORITHM_H_
