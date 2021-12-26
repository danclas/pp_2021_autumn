// Copyright 2021 Denisova Julia
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <vector>
#include <gtest-mpi-listener.hpp>

#ifndef MODULES_TASK_3_DENISOVA_ODD_EVEN_BATCHER_SORT_ODD_EVEN_BATCHER_SORT_H_
#define MODULES_TASK_3_DENISOVA_ODD_EVEN_BATCHER_SORT_ODD_EVEN_BATCHER_SORT_H_

std::vector<int> random_vector(int size);
int partition(std::vector<int>* v, int l, int r);
void q_sort(std::vector<int>* v, int l, int r);
void odd_even_merge(std::vector<int> l, std::vector<int> r);
void allocation(std::vector<int> v);
void build_allocation(int procNum);
void q_sort_batcher(std::vector<int>* v);

#endif  // MODULES_TASK_3_DENISOVA_ODD_EVEN_BATCHER_SORT_ODD_EVEN_BATCHER_SORT_H_
