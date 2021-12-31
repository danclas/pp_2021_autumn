// Copyright 2021 Alimov Mikhail
#ifndef MODULES_TASK_1_ALIMOV_M_NE_PORADOK_VECTOR_NE_PORADOK_VECTOR_H_
#define MODULES_TASK_1_ALIMOV_M_NE_PORADOK_VECTOR_NE_PORADOK_VECTOR_H_
#include <mpi.h>
#include <string>

void Rand(int* vec, int n);

int getNumNonMonotonyElems(int* input_vec, int size);

int fun(int* a, const int n);

#endif  // MODULES_TASK_1_ALIMOV_M_NE_PORADOK_VECTOR_NE_PORADOK_VECTOR_H_
