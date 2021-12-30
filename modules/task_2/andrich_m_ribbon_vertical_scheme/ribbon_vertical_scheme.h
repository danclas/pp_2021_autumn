// Copyright 2021 Andrich Maria
#ifndef MODULES_TASK_2_ANDRICH_M_RIBBON_VERTICAL_SCHEME_RIBBON_VERTICAL_SCHEME_H_
#define MODULES_TASK_2_ANDRICH_M_RIBBON_VERTICAL_SCHEME_RIBBON_VERTICAL_SCHEME_H_

int* initEmptyMatrix(int rows);
int* getRandomMatrix(int rows, int cols);
void SequentialMatrixMultiplication(const int* A, const int ARows,
                                    const int ACols, const int* B,
                                    const int BRows, int* C);
void ParallelMatrixMultiplication(const int* A, const int ARows,
                                  const int ACols, const int* B,
                                  const int BRows, int* C);

#endif  // MODULES_TASK_2_ANDRICH_M_RIBBON_VERTICAL_SCHEME_RIBBON_VERTICAL_SCHEME_H_
