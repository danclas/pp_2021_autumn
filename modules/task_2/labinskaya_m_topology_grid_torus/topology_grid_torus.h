// Copyright 2021 Labinskaya Marina
#include <vector>
#ifndef MODULES_TASK_2_LABINSKAYA_M_TOPOLOGY_GRID_TORUS_TOPOLOGY_GRID_TORUS_H_
#define MODULES_TASK_2_LABINSKAYA_M_TOPOLOGY_GRID_TORUS_TOPOLOGY_GRID_TORUS_H_

MPI_Comm creating_topology_grid_torus(int n, int m);
bool is_topology_grid_torus(const MPI_Comm grig_torus_comm, const int ndims, const int *dims);
#endif  // MODULES_TASK_2_LABINSKAYA_M_TOPOLOGY_GRID_TORUS_TOPOLOGY_GRID_TORUS_H_
