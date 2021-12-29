// Copyright 2021 Vanyushkov Maxim
#ifndef MODULES_TASK_3_VANYUSHKOV_M_ODD_EVEN_SORT_ODD_EVEN_SORT_H_
#define MODULES_TASK_3_VANYUSHKOV_M_ODD_EVEN_SORT_ODD_EVEN_SORT_H_

#include <mpi.h>

void vec_gen(int* vec, int len);
void copy_vec(int* copy_vec, int* vec, int len);
void q_sort(int* vec, int l, int r);
void odd_even_sort(int* vec, int len);

#endif  // MODULES_TASK_3_VANYUSHKOV_M_ODD_EVEN_SORT_ODD_EVEN_SORT_H_
