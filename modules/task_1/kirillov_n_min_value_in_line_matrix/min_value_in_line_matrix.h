// Copyright 2021 Kirillov Nikita

#ifndef MODULES_TASK_1_KIRILLOV_N_MIN_VALUE_IN_LINE_MATRIX_MIN_VALUE_IN_LINE_MATRIX_H_
#define MODULES_TASK_1_KIRILLOV_N_MIN_VALUE_IN_LINE_MATRIX_MIN_VALUE_IN_LINE_MATRIX_H_

int** getRandomMatrix(int size_w, int size_h);
int* getParallelOperations(int* global_matrix, int width, int height);
int* getSequentialOperations(int** matrix, const int width, const int height);

#endif  // MODULES_TASK_1_KIRILLOV_N_MIN_VALUE_IN_LINE_MATRIX_MIN_VALUE_IN_LINE_MATRIX_H_
