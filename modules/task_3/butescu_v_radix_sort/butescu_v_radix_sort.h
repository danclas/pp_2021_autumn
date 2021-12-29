// Copyright 2021 Butescu Vladimir
#ifndef MODULES_TASK_3_BUTESCU_V_RADIX_SORT_BUTESCU_V_VECTOR_AVERAGE_H_
#define MODULES_TASK_3_BUTESCU_V_RADIX_SORT_BUTESCU_V_VECTOR_AVERAGE_H_

std::vector<double> mergeVectors(std::vector<double> vec1, std::vector<double> vec2);
std::vector<double> Get_Random_Vector(int size);

std::vector<double> sortingByCounting(std::vector<double> vec1,
    std::vector<double> vec2, int byte);
std::vector<double> radixSort(std::vector<double> vec);
std::vector<double> Parallel_Radix_Sort(const std::vector<double>& vec);

#endif   // MODULES_TASK_3_BUTESCU_V_RADIX_SORT_BUTESCU_V_VECTOR_AVERAGE_H_
