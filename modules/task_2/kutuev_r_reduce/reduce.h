// Copyright 2021 Kutuev Roman

#ifndef MODULES_TASK_2_KUTUEV_R_REDUCE_REDUCE_H_
#define MODULES_TASK_2_KUTUEV_R_REDUCE_REDUCE_H_

#include <mpi.h>

#include <cstring>
#include <iostream>

int my_reduce(void*, void*, int, MPI_Datatype, MPI_Op, int, MPI_Comm);

#endif  //  MODULES_TASK_2_KUTUEV_R_REDUCE_REDUCE_H_
