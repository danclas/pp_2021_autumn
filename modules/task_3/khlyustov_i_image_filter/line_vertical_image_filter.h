// Copyright 2021 Khlyustov Ilya
#ifndef MODULES_TASK_3_KHLYUSTOV_I_IMAGE_FILTER_LINE_VERTICAL_IMAGE_FILTER_H_
#define MODULES_TASK_3_KHLYUSTOV_I_IMAGE_FILTER_LINE_VERTICAL_IMAGE_FILTER_H_
#include <mpi.h>
#include <vector>

#define vectorF std::vector<float>

vectorF getKernel(float sigma = 3.f);
vectorF getRandomImg(int w, int h);
vectorF getSequential(const vectorF& kernel, const vectorF& img, int w, int h);
vectorF getParallel(const vectorF& kernel, const vectorF& img, int w, int h);

#endif  // MODULES_TASK_3_KHLYUSTOV_I_IMAGE_FILTER_LINE_VERTICAL_IMAGE_FILTER_H_
