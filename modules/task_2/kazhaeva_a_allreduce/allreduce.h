// Copyright 2021 Kazhaeva Anastasia
#ifndef MODULES_TASK_2_KAZHAEVA_A_ALLREDUCE_ALLREDUCE_H_
#define MODULES_TASK_2_KAZHAEVA_A_ALLREDUCE_ALLREDUCE_H_
#include <mpi.h>
#include <vector>
#include <string>

using std::vector;

template <typename T>
int OperationsBuff(void* sendbuf, void* recvbuf,
  int count, MPI_Datatype type, MPI_Op Op, MPI_Comm Comm);

int My_MPI_Allreduce(const void* sendbuf, void* recvbuf,
  int count, MPI_Datatype type, MPI_Op Op, MPI_Comm Comm);

template <typename T>
std::vector<T> GetRandomVector(int n, int max);

#endif  // MODULES_TASK_2_KAZHAEVA_A_ALLREDUCE_ALLREDUCE_H_
