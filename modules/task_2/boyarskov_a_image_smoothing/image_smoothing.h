// Copyright 2021 Boyarskov Anton
#ifndef MODULES_TASK_2_BOYARSKOV_A_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_
#define MODULES_TASK_2_BOYARSKOV_A_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_

int* getRandomImage(int width, int height);
int* getSequentialGauss(int* image, int width, int height, int radius);
int* getParallelGauss(int* image, int width, int height, int radius);

#endif  // MODULES_TASK_2_BOYARSKOV_A_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_
