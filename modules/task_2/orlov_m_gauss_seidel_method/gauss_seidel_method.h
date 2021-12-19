//  Copyright 2021 Maksim Orlov

#pragma once
#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <random>

int randNumber(std::random_device* dev, std::mt19937* rng, int max = RAND_MAX);
std::vector<double> sequentialGaussSeidel(std::vector<std::vector<double>> coeffMatrix,
    std::vector<double> rightSide, double eps);
std::vector<double> gaussSeidel(std::vector<std::vector<double>> coeffMatrix,
    std::vector<double> rightSide, double eps);
