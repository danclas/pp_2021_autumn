//  Copyright 2021 Maksim Orlov

#pragma once
#include <mpi.h>
#include <vector>
#include <random>

int randNumber(std::random_device* dev, std::mt19937* rng, int max = RAND_MAX);
double gaussianFunction(int i, int j, double sigma);
std::vector<std::vector<char>> sequentialGaussianFilter(std::vector<std::vector<char>> image, double sigma);
std::vector<std::vector<char>> gaussianFilter(std::vector<std::vector<char>> image, double sigma);
