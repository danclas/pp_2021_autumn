// Copyright 2021 Kraev Nikita
#include "../../../modules/task_2/kraev_n_topology_ruler/topology_ruler.h"

MPI_Comm create_topology_ruler(const MPI_Comm world) {
    int nnodes;
    int reorder = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &nnodes);
    int size = (nnodes - 1) * 2;
    int* index = new int[nnodes];
    int* edges = new int[size];

    index[0] = 1;
    edges[0] = 1;
    if (nnodes > 1) {
        for (int i = 1; i < nnodes - 1; i++) index[i] = index[i - 1] + 2;
        index[nnodes - 1] = index[nnodes - 2] + 1;

        for (int i = 1; i < (nnodes - 1) * 2; i++) {
            if (i % 2 == 0)
                edges[i] = edges[i - 2] + 1;
            else
                edges[i] = edges[i - 1] - 1;
        }
    }
    MPI_Comm ruler;
    MPI_Graph_create(world, nnodes, index, edges, reorder, &ruler);
    return ruler;
}
