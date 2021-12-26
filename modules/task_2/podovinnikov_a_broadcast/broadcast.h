// Copyright 2021 Podovinnikov Artyom
#include <mpi.h>
#ifndef MODULES_TASK_2_PODOVINNIKOV_A_BROADCAST_BROADCAST_H_
#define MODULES_TASK_2_PODOVINNIKOV_A_BROADCAST_BROADCAST_H_
int Broadcast(void* sendbuf, int count, MPI_Datatype type, int root,
              MPI_Comm comm);
int randInt(int min, int max);

#endif  // MODULES_TASK_2_PODOVINNIKOV_A_BROADCAST_BROADCAST_H_
