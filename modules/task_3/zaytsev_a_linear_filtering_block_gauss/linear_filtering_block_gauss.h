// Copyright 2021 Zaytsev Alexander
#ifndef MODULES_TASK_3_ZAYTSEV_A_LINEAR_FILTERING_BLOCK_GAUSS_LINEAR_FILTERING_BLOCK_GAUSS_H_
#define MODULES_TASK_3_ZAYTSEV_A_LINEAR_FILTERING_BLOCK_GAUSS_LINEAR_FILTERING_BLOCK_GAUSS_H_
#include <vector>
using std::vector;
void Gauss_Core(/*double sigma*/);
vector<double> RandomMatrix(int matrix_rows, int matrix_columns);
vector<double> SequentialOp(const vector<double>& image, int matrix_rows,
                            int matrix_columns);
vector<double> ParallelOp(const vector<double>& image, int matrix_size);

#endif  // MODULES_TASK_3_ZAYTSEV_A_LINEAR_FILTERING_BLOCK_GAUSS_LINEAR_FILTERING_BLOCK_GAUSS_H_
