// Copyright 2021 Chelebaev Artem
#ifndef MODULES_TASK_3_CHELEBAEV_STRASSEN_ALGORITHM_STRASSEN_ALGORITHM_H_
#define MODULES_TASK_3_CHELEBAEV_STRASSEN_ALGORITHM_STRASSEN_ALGORITHM_H_

#include <vector>
#include <string>

typedef std::vector<std::vector<double>> Matrix;

Matrix createRandomMatrix(int n, int m, double max_number);

Matrix sequentialMulti(const Matrix& a, const Matrix& b);
Matrix parallelMulti(const Matrix& a, const Matrix& b);

std::vector<Matrix> divide(const Matrix& a);
Matrix connect(const Matrix& a, const Matrix& b, const Matrix& c, const Matrix& d);

Matrix msum(const Matrix& a, const Matrix& b);
Matrix msub(const Matrix& a, const Matrix& b);

bool areEqual(const Matrix& a, const Matrix& b, double delta);


#endif  // MODULES_TASK_3_CHELEBAEV_STRASSEN_ALGORITHM_STRASSEN_ALGORITHM_H_
