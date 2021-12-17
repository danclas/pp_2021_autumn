// Copyright 2021 Miron Krivosheev
#ifndef PARALELL_PROGRAMMING_PP_2021_AUTUMN_MODULES_TASK_1_KRIVOSHEEV_M_SYMBOL_FREQUENCY_SYMBOL_FREQUENCY_H_
#define PARALELL_PROGRAMMING_PP_2021_AUTUMN_MODULES_TASK_1_KRIVOSHEEV_M_SYMBOL_FREQUENCY_SYMBOL_FREQUENCY_H_

#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <cctype>
#include <iostream>
#include <algorithm>

double getFrequencyParallel(char s, std::string text);
double getFrequencyNonParallel(char s, std::string text);
#endif  // PARALELL_PROGRAMMING_PP_2021_AUTUMN_MODULES_TASK_1_KRIVOSHEEV_M_SYMBOL_FREQUENCY_SYMBOL_FREQUENCY_H_
