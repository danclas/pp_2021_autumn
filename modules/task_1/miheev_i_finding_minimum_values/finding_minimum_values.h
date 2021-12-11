// Copyright 2021 Miheev Ivan
#ifndef MODULES_TASK_1_MIHEEV_I_FINDING_MINIMUM_VALUES_FINDING_MINIMUM_VALUES_H_
#define MODULES_TASK_1_MIHEEV_I_FINDING_MINIMUM_VALUES_FINDING_MINIMUM_VALUES_H_

#include <vector>

void getMatrix(std::vector<int>* matrix,
               std::vector<int>::size_type matrixSize);

std::vector<int> sequentialFindMin(const std::vector<int>& matrix,
                                   std::vector<int>::size_type matrixRows,
                                   std::vector<int>::size_type matrixColumns);
std::vector<int> parallelFindMin(const std::vector<int>& matrix,
                                 std::vector<int>::size_type matrixRows,
                                 std::vector<int>::size_type matrixColumns);

#endif  //  MODULES_TASK_1_MIHEEV_I_FINDING_MINIMUM_VALUES_FINDING_MINIMUM_VALUES_H_
