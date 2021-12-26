// Copyright [2021] <Borisov Maxim>

#ifndef MODULES_TASK_2_BORISOV_M_SCATTER_SCATTER_H_
#define MODULES_TASK_2_BORISOV_M_SCATTER_SCATTER_H_
#include <mpi.h>

int my_scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype,
        void* recvbuf, int revccount, MPI_Datatype recvtype,
        int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_BORISOV_M_SCATTER_SCATTER_H_
