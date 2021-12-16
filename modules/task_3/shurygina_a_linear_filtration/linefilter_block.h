// Copyright 2021 Shurygina A
#ifndef MODULES_TASK_3_SHURYGINA_A_LINEFILTER_BLOCK_LINEFILTER_BLOCK_H_
#define MODULES_TASK_3_SHURYGINA_A_LINEFILTER_BLOCK_LINEFILTER_BLOCK_H_

#include <vector>

std::vector<int> createRandomImg(int width, int height);
int simpleGaussianFilter(std::vector<int>& img, int height,
	int width, int radius,
	double sigma, std::vector<double>& kernel, int row, int cols);
std::vector<int> parallelSegmentFilter( std::vector<int>& img, int height,
	int width, int radius,
	double sigma, std::vector<double>& kernel, int begin, int add);
std::vector<int> parallelGaussianFilter(std::vector<int>& img, int height,
	int width, int radius, double sigma,
	std::vector<double>& kernel);
void show(std::vector<int> lol, int height, int width);
std::vector<double> createGaussianKernel(int size, double sigma);
int findBoundaries(int data, int left, int right);

#endif  // MODULES_TASK_3_SHURYGINA_A_LINEFILTER_BLOCK_LINEFILTER_BLOCK_H_
