// Copyright 2021 Gleb "belgad" Danshin
#ifndef MODULES_TASK_2_DANSHIN_G_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_
#define MODULES_TASK_2_DANSHIN_G_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_

#include <mpi.h>
#include <random>
#include <thread>  // NOLINT [build/c++11]
#include <chrono>  // NOLINT [build/c++11]
#include <iostream>

void producers_consumers(int product_number);
void hub(int product_number);
void producer();
void consumer();

#endif  // MODULES_TASK_2_DANSHIN_G_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_
