// Copyright 2021 Olenin Sergey
#include "../../../modules/task_2/olenin_s_topology_hypercube/hypercube.h"

#include <mpi.h>

int getDimensionCount(int numberProc) {
  if (numberProc <= 0) return -0;

  int countDimension = 0;

  while (numberProc > 1) {
    if (numberProc % 2 == 1) return -1;

    numberProc = numberProc / 2;

    countDimension++;
  }

  return countDimension;
}

MPI_Comm getHyperCube(int countDim) {
  if (countDim <= 0) throw "Count Dimension is negative";

  MPI_Comm hypercube;

  int *dimensions = new int[countDim];
  int *periods = new int[countDim];

  for (int i = 0; i < countDim; i++) {
    dimensions[i] = 2;
    periods[i] = 1;
  }

  MPI_Cart_create(MPI_COMM_WORLD, countDim, dimensions, periods, 0, &hypercube);

  delete[] dimensions;
  delete[] periods;

  return hypercube;
}
