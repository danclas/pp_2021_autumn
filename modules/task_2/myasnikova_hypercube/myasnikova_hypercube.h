// Copyright 2021 Myasnikova Varvara

#ifndef MODULES_TASK_2_MYASNIKOVA_HYPERCUBE_MYASNIKOVA_HYPERCUBE_H_
#define MODULES_TASK_2_MYASNIKOVA_HYPERCUBE_MYASNIKOVA_HYPERCUBE_H_

#include <mpi.h>

MPI_Comm CreateHypercube(int ndims, int dims);

bool HypercubeData(MPI_Comm hypercube_comm, int ndims, int dims);
bool HypercubeCorrection(MPI_Comm hypercube_comm, int ndims, int dims);

#endif  // MODULES_TASK_2_MYASNIKOVA_HYPERCUBE_MYASNIKOVA_HYPERCUBE_H_
