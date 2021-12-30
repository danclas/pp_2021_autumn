// Copyright 2021 Krapivenskikh Stepan
#include <mpi.h>
#include "../../../modules/task_2/krapivenskikh_s_star_topology/star_topology.h"

MPI_Comm createStarTopology() {
    MPI_Barrier(MPI_COMM_WORLD);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* arrayOfIndex = new int[size];
    int* arrayOfEdge = new int[size * 2 - 2];

    arrayOfIndex[0] = size - 1;
    for (int k = 1, j = 0; k < size; k++) {
        arrayOfEdge[j++] = k;
    }
    for (int i = 1, j = size - 1; i < size; i++) {
        arrayOfIndex[i] = arrayOfIndex[i - 1] + 1;
        arrayOfEdge[j++] = 1;
    }

    MPI_Comm starComm;
    MPI_Graph_create(MPI_COMM_WORLD, size, arrayOfIndex, arrayOfEdge, 1, &starComm);

    return starComm;
}
