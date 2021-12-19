// Copyright 2021 Pinezhanin Evgeny
#ifndef MODULES_TASK_3_PINEZHANIN_E_LINEAR_HISTOGRAM_STRETCHING_LINEAR_HISTOGRAM_STRETCHING_H_
#define MODULES_TASK_3_PINEZHANIN_E_LINEAR_HISTOGRAM_STRETCHING_LINEAR_HISTOGRAM_STRETCHING_H_

#include <vector>

typedef unsigned char uchar;

std::vector<uchar> getRandomMatrix(int height, int width, int a, int b);
std::vector<uchar> LinearHistogramStretchingParallel(std::vector<uchar> image);
std::vector<uchar> LinearHistogramStretchingSequential(std::vector<uchar> image);

#endif  // MODULES_TASK_3_PINEZHANIN_E_LINEAR_HISTOGRAM_STRETCHING_LINEAR_HISTOGRAM_STRETCHING_H_
