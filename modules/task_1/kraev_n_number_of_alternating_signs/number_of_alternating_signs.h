// Copyright 2021 Kraev Nikita

#ifndef MODULES_TASK_1_KRAEV_N_NUMBER_OF_ALTERNATING_SIGNS_NUMBER_OF_ALTERNATING_SIGNS_H_
#define MODULES_TASK_1_KRAEV_N_NUMBER_OF_ALTERNATING_SIGNS_NUMBER_OF_ALTERNATING_SIGNS_H_

#include <vector>

std::vector<int> getRandomVector(const int size);
int findingNumberOfSignAlternations(const std::vector<int>& vec);
int parallelFindingNumberOfSignAlternations(const std::vector<int>& vec,
    const int size);

#endif  // MODULES_TASK_1_KRAEV_N_NUMBER_OF_ALTERNATING_SIGNS_NUMBER_OF_ALTERNATING_SIGNS_H_
