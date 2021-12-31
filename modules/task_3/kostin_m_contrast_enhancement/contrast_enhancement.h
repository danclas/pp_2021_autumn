// Copyright 2021 Kostin Maksim
#ifndef MODULES_TASK_3_KOSTIN_M_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_
#define MODULES_TASK_3_KOSTIN_M_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_

#include <vector>

std::vector<int> getRandomVector(int sz);
int* getRandomImg(int width, int height);
int* SequentialLinHist(int* image, int width, int height, int min, int max);
int* ParallelLinHist(int* image, int width, int height);
int GetLocalMin(int* local, int width, int height);
int GetLocalMax(int* local, int width, int height);

#endif  // MODULES_TASK_3_KOSTIN_M_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_
