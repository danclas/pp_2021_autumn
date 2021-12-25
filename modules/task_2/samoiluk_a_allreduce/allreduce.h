// Copyright 2021 Samoiluk Anastasiya
#ifndef MODULES_TASK_2_SAMOILUK_A_ALLREDUCE_ALLREDUCE_H_
#define MODULES_TASK_2_SAMOILUK_A_ALLREDUCE_ALLREDUCE_H_

#include <mpi.h>

int Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype type,
    MPI_Op Op, MPI_Comm comm);

template<class T>
T* getRandomVec(int size, int border);

#endif  // MODULES_TASK_2_SAMOILUK_A_ALLREDUCE_ALLREDUCE_H_
