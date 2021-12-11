// Copyright 2021 Kamenev Ilya
#ifndef MODULES_TASK_2_KAMENEV_I_BROADCAST_BROADCAST_H_
#define MODULES_TASK_2_KAMENEV_I_BROADCAST_BROADCAST_H_

#include <float.h>
#include <mpi.h>
#include <climits>
#include <vector>

template <typename T>
std::vector<T>getRandomVector(int n, int max);

int Bcast(void* sendbuf, void* outbuf, int count, MPI_Datatype datatype,
          int root, MPI_Op op, MPI_Comm comm);

#endif  // MODULES_TASK_2_KAMENEV_I_BROADCAST_BROADCAST_H_
