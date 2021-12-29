// Copyright 2021 Olenin Sergey
#ifndef MODULES_TASK_2_OLENIN_S_TOPOLOGY_HYPERCUBE_HYPERCUBE_H_
#define MODULES_TASK_2_OLENIN_S_TOPOLOGY_HYPERCUBE_HYPERCUBE_H_
#include <mpi.h>

int getDimensionCount(int numberProc);
MPI_Comm getHyperCube(int countDim);

#endif  // MODULES_TASK_2_OLENIN_S_TOPOLOGY_HYPERCUBE_HYPERCUBE_H_
