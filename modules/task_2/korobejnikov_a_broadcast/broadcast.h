// Copyright 2021 Korobejnikov Alexander
#ifndef MODULES_TASK_2_KOROBEJNIKOV_A_BROADCAST_BROADCAST_H_
#define MODULES_TASK_2_KOROBEJNIKOV_A_BROADCAST_BROADCAST_H_

#include<mpi.h>
#include<vector>

int My_Bcast(void *buffer, int count,
        MPI_Datatype datatype, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_KOROBEJNIKOV_A_BROADCAST_BROADCAST_H_
