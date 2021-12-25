// Copyright 2021 Groshev Nikolay
#ifndef MODULES_TASK_2_GROSHEV_N_REDUCE_REDUCE_H_
#define MODULES_TASK_2_GROSHEV_N_REDUCE_REDUCE_H_

#include <mpi.h>
#include <algorithm>

void my_reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm);
void sum(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
         MPI_Op op, int root, MPI_Comm comm, int my_rank);
void prod(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
          MPI_Op op, int root, MPI_Comm comm, int my_rank);
void max(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
         MPI_Op op, int root, MPI_Comm comm, int my_rank);
void min(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
         MPI_Op op, int root, MPI_Comm comm, int my_rank);

#endif  //  MODULES_TASK_2_GROSHEV_N_REDUCE_REDUCE_H_
