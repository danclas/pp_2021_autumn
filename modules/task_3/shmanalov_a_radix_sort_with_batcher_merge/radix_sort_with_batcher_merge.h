// Copyright 2021 Shmanalov Alexander
#ifndef MODULES_TASK_3_SHMANALOV_A_RADIX_SORT_WITH_BATCHER_MERGE_RADIX_SORT_WITH_BATCHER_MERGE_H_
#define MODULES_TASK_3_SHMANALOV_A_RADIX_SORT_WITH_BATCHER_MERGE_RADIX_SORT_WITH_BATCHER_MERGE_H_

#include <vector>

std::vector<int> generationRandomVector(int size);
std::vector<int> sequentialRadixSort(std::vector<int> array);
std::vector<int> vectorShuffle(std::vector<int> array);
std::vector<int> OddMerge(const std::vector<int>& arr1,
                          const std::vector<int>& arr2);
std::vector<int> EvenMerge(const std::vector<int>& arr1,
                           const std::vector<int>& arr2);
std::vector<int> oddEvenMerge(std::vector<int> arr1, std::vector<int> arr2,
                              int evenCount, int oddCount);
std::vector<int> parallelRadixSortMergeBatcher(std::vector<int> globalArr,
                                               int sizeArr);

#endif  // MODULES_TASK_3_SHMANALOV_A_RADIX_SORT_WITH_BATCHER_MERGE_RADIX_SORT_WITH_BATCHER_MERGE_H_
