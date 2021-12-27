// Copyright 2021 Dydykin Pavel
#ifndef MODULES_TASK_3_DYDYKIN_P_RADIX_SORT_DOUBLE_SIMPLE_MERGE_RADIX_SORT_DOUBLE_SIMPLE_MERGE_H_
#define MODULES_TASK_3_DYDYKIN_P_RADIX_SORT_DOUBLE_SIMPLE_MERGE_RADIX_SORT_DOUBLE_SIMPLE_MERGE_H_
#include <mpi.h>
#include <iostream>
#include <random>
#include <list>
#include <string>
#include <sstream>
#include <vector>

std::vector<double> Get_Random_Vector(int size);
int LeftOfThePoint(double num);
int RightOfThePoint(double num);
std::vector<double> Merge(const std::vector<double>& vec_left,
    const std::vector<double>& vec_right);
int GetDigit(double num, int radix);
std::vector<double> RadixSort(const std::vector<double>& vect, int rad);
std::vector<double> Not_Parallel_Radix_Sort(const std::vector<double>& vect);
std::vector<double> Parallel_Radix_Sort(const std::vector<double>& vec);

#endif  // MODULES_TASK_3_DYDYKIN_P_RADIX_SORT_DOUBLE_SIMPLE_MERGE_RADIX_SORT_DOUBLE_SIMPLE_MERGE_H_
