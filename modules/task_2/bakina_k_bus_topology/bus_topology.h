// Copyright 2021 Bakina Ksenia
#ifndef MODULES_TASK_2_BAKINA_K_BUS_TOPOLOGY_BUS_TOPOLOGY_H_
#define MODULES_TASK_2_BAKINA_K_BUS_TOPOLOGY_BUS_TOPOLOGY_H_
#include <mpi.h>
#include <vector>
#include <iostream>

MPI_Comm get_Bus_Comm(const MPI_Comm comm_old);

#endif  // MODULES_TASK_2_BAKINA_K_BUS_TOPOLOGY_BUS_TOPOLOGY_H_
