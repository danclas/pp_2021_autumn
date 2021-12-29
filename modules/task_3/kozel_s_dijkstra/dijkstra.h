// Copyright 2021 Kozel Svetlana
#ifndef MODULES_TASK_3_KOZEL_S_DIJKSTRA_DIJKSTRA_H_
#define MODULES_TASK_3_KOZEL_S_DIJKSTRA_DIJKSTRA_H_
#include <mpi.h>

int* gen_matrix(int size, int procRank = 0);
bool check_eq(int* vec1, int* vec2, int size);
int* sequential(int* matrix, int size);
int* parallel(int* matrix, int size);

#endif  // MODULES_TASK_3_KOZEL_S_DIJKSTRA_DIJKSTRA_H_
