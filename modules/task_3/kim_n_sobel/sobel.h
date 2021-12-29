// Copyright 2021 Kim Nikita
#ifndef MODULES_TASK_3_KIM_N_SOBEL_SOBEL_H_
#define MODULES_TASK_3_KIM_N_SOBEL_SOBEL_H_

#include <vector>

std::vector<std::vector<int>> getRandomImage(int size);

int clamp(int value, int min, int max);

int calcNewPixelColor(std::vector<std::vector<int>> image, int x, int y, int size, int kernel[3][3]);

int calcNewPixelColor(std::vector<int> image, int x, int y, int size, int kernel[3][3]);

std::vector<std::vector<int>> getSobelImage(const std::vector<std::vector<int>>& image, int size, char type);

std::vector<std::vector<int>> getSobelImageParall(const std::vector<std::vector<int>>& image, int size, char type);

#endif  // MODULES_TASK_3_KIM_N_SOBEL_SOBEL_H_
