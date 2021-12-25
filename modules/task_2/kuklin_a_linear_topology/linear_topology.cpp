// Copyright 2021 Kuklin Andrey
#include <mpi.h>
#include "../../../modules/task_2/kuklin_a_linear_topology/linear_topology.h"

MPI_Comm GetlinearTopologyComm(MPI_Comm comm) {
  int procRank, procSize;

  MPI_Comm_size(comm, &procSize);
  MPI_Comm_rank(comm, &procRank);

  int sizeOfEdges = 2 * procSize - 2;
  int *index, *edges;
  index = new int[procSize];
  edges = new int[sizeOfEdges];

  index[0] = 1;
  if (procSize > 1) {
    for (int i = 1; i < procSize - 1; i++) {
      index[i] = index[i - 1] + 2;
    }
    index[procSize - 1] = index[procSize - 2] + 1;
  }

  edges[0] = 1;
  if (procSize > 1) {
    for (int i = 1; i < procSize - 1; i++) {
      edges[2 * i - 1] = i - 1;
      edges[2 * i] = i + 1;
    }
    edges[sizeOfEdges - 1] = procSize - 2;
  }

  MPI_Comm linear;
  MPI_Graph_create(comm, procSize, index, edges, false, &linear);

  delete []index;
  delete []edges;

  return linear;
}
