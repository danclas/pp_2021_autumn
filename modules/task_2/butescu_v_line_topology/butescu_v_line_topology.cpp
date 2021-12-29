// Copyright 2021 Butescu Vladimir
#include "../../../modules/task_2/butescu_v_line_topology/butescu_v_line_topology.h"
#include <mpi.h>


MPI_Comm create_line_topology(MPI_Comm comm, int ProcNum) {
    MPI_Comm Line;

    int* index = new int[ProcNum];
    int* edges = new int[2 * (ProcNum - 1)];
    int reorder = 0;

    for (int i = 0; i < ProcNum; i++) {
        if (i == 0)
            index[i] = 1;
        else if (i == ProcNum - 1)
            index[i] = index[ProcNum - 2] + 1;
        else
            index[i] = index[i - 1] + 2;
    }

    for (int i = 1, j = 1; i < 2 * (ProcNum - 1) - 1; i += 2, j++) {
        edges[i] = j - 1;
        edges[i + 1] = j + 1;
    }
    edges[0] = 1;
    edges[2 * (ProcNum - 1) - 1] = ProcNum - 2;

    MPI_Graph_create(MPI_COMM_WORLD, ProcNum, index, edges, reorder, &Line);
    return Line;
}
