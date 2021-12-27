// Copyright 2021 Sovetov Egor
#ifndef MODULES_TASK_3_SOVETOV_E_QUICK_SORT_QUICK_SORT_H_
#define MODULES_TASK_3_SOVETOV_E_QUICK_SORT_QUICK_SORT_H_

#include <mpi.h>

#include <vector>

bool SortWektor(const std::vector<int> wektor);
std::vector<int> GetWektorRandom(int razmerWektor);
void QuickSortNO_MPI(std::vector<int>* wektor, int levy, int prav);
void QuickSortMPI(std::vector<int>* wektor);

#endif  // MODULES_TASK_3_SOVETOV_E_QUICK_SORT_QUICK_SORT_H_
