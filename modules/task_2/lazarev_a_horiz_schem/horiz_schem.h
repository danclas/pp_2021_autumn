// Copyright 2021 Lazarev Alexey
#ifndef MODULES_TASK_2_LAZAREV_A_HORIZ_SCHEM_HORIZ_SCHEM_H_
#define MODULES_TASK_2_LAZAREV_A_HORIZ_SCHEM_HORIZ_SCHEM_H_
#include <mpi.h>

#include <iostream>
#include <random>
#include <vector>

std::vector<int> random_vector(int size);
std::vector<int> transposition(std::vector<int> b, int size);
std::vector<int> multiplication(std::vector<int> a, std::vector<int> b,
                                int size);
std::vector<int> multiplication_parallel(std::vector<int> A,
                                         const std::vector<int>& B, int data);
#endif  // MODULES_TASK_2_LAZAREV_A_HORIZ_SCHEM_HORIZ_SCHEM_H_
