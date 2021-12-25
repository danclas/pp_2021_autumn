// Copyright 2021 Chornyi Yurii
#pragma once

#ifndef FIND_MATRIX_MIN
#define FIND_MATRIX_MIN

#include <vector>

std::vector<int> fillMatrixWithRandom(const size_t numberOfRows, const size_t numberOfColumns);
int singleFindMinimum(std::vector<int> matrix);

int parallelFindMinimum(std::vector<int> matrix, const size_t numberOfRows, const size_t numberOfColumns);

#endif  // FIND_MATRIX_MIN
