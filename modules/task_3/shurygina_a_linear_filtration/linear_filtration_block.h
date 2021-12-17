// Copyright 2021 Shurygina A

#ifndef MODULES_TASK_3_SHURYGINA_A_LINEAR_FILTRATION_LINEAR_FILTRATION_BLOCK_H_


#include <vector>

std::vector<int> createRandomImg(int width, int height);
int simpleGaussianFilter(const std::vector<int>& img, int height,
int width, int radius,
double sigma, const std::vector<double>& kernel, int row, int cols);
std::vector<int> parallelSegmentFilter(const std::vector<int>& img, int height,
int width, int radius,
double sigma, const std::vector<double>& kernel, int begin, int add);
std::vector<int> parallelGaussianFilter(const std::vector<int>& img, int height,
int width, int radius, double sigma,
const std::vector<double>& kernel);
std::vector<double> createGaussianKernel(int size, double sigma);
int findBoundaries(int data, int left, int right);
void show(const std::vector<int> img, int height, int width);

#endif  // MODULES_TASK_3_SHURYGINA_A_LINEAR_FILTRATION_LINEFILTER_BLOCK_H_
