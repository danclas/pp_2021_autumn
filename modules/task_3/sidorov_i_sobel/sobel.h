// Copyright 2021 Sidorov Ilya
#ifndef MODULES_TASK_3_SIDOROV_I_SOBEL_SOBEL_H_
#define MODULES_TASK_3_SIDOROV_I_SOBEL_SOBEL_H_

#include <vector>
#include <string>

const int kernelX[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
const int kernelY[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

int* getRandomImage(int width, int height);
int* getSequentialSobel(int* image, int width, int height);
int* getParallelSobel(int* image, int width, int height);
int clamp(int value, int max, int min);
float calcNewPixelColorX(int* img, int x, int y, int width, int height);
float calcNewPixelColorY(int* img, int x, int y, int width, int height);


#endif  // MODULES_TASK_3_SIDOROV_I_SOBEL_SOBEL_H_
