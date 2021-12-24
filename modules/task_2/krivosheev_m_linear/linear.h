// Copyright 2021 Krivosheev Miron

#ifndef MODULES_TASK_2_KRIVOSHEEV_M_LINEAR_LINEAR_H_
#define MODULES_TASK_2_KRIVOSHEEV_M_LINEAR_LINEAR_H_

#include <mpi.h>
#include <vector>

MPI_Comm linear_topology(MPI_Comm comm);
bool is_linear_topology(MPI_Comm comm);
#endif  // MODULES_TASK_2_KRIVOSHEEV_M_LINEAR_LINEAR_H_
