// Copyright 2021 Sidorov Ilya
#ifndef MODULES_TASK_2_SIDOROV_I_HYPERCUBE_HYPERCUBE_H_
#define MODULES_TASK_2_SIDOROV_I_HYPERCUBE_HYPERCUBE_H_
#include <mpi.h>

int getLevel(int NumProc);
MPI_Comm hyperCube(int countLevel);

#endif  // MODULES_TASK_2_SIDOROV_I_HYPERCUBE_HYPERCUBE_H_
