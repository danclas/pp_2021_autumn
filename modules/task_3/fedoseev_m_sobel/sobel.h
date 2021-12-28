// Copyright 2021 Fedoseev Mikhail
#ifndef MODULES_TASK_3_FEDOSEEV_M_SOBEL_SOBEL_H_
#define MODULES_TASK_3_FEDOSEEV_M_SOBEL_SOBEL_H_

const int kernelX[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
const int kernelY[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

int* getRandomImage(int width, int height);
int* getSequentialSobel(int* image, int width, int height);
int* getParallelSobel(int* image, int width, int height);

#endif  // MODULES_TASK_3_FEDOSEEV_M_SOBEL_SOBEL_H_
