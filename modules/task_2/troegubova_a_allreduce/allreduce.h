// Copyright 2021 Troegubova Alexandra
#ifndef MODULES_TASK_2_TROEGUBOVA_A_ALLREDUCE_ALLREDUCE_H_
#define MODULES_TASK_2_TROEGUBOVA_A_ALLREDUCE_ALLREDUCE_H_

#include <mpi.h>
#include <vector>

template <typename T>
std::vector<T> getRandomVec(int size);

int ALLreduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype type, MPI_Op op, MPI_Comm comm);

#endif  // MODULES_TASK_2_TROEGUBOVA_A_ALLREDUCE_ALLREDUCE_H_


