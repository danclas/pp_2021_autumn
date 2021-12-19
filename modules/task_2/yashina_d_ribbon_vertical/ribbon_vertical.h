// Copyright 2021 Yashina Darya
#ifndef MODULES_TASK_2_YASHINA_D_RIBBON_VERTICAL_RIBBON_VERTICAL_H_
#define MODULES_TASK_2_YASHINA_D_RIBBON_VERTICAL_RIBBON_VERTICAL_H_

int* initEmptyMatrix(int rows);
int* getRandomMatrix(int rows, int cols);
void SequentialMatrixMultiplication(const int* A, const int ARows,
                                    const int ACols, const int* B,
                                    const int BRows, int* C);
void ParallelMatrixMultiplication(const int* A, const int ARows,
                                  const int ACols, const int* B,
                                  const int BRows, int* C);

#endif  // MODULES_TASK_2_YASHINA_D_RIBBON_VERTICAL_RIBBON_VERTICAL_H_
