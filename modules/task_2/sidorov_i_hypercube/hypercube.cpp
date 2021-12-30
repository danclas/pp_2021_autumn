// Copyright 2021 Sidorov Ilya

#include "../../../modules/task_2/sidorov_i_hypercube/hypercube.h"

int getLevel(int NumProc) {
  if (NumProc <= 0) return -0;

  int Level = 0;
  while (NumProc > 1) {
    if (NumProc % 2 == 1) return -1;
    NumProc = NumProc / 2;
    Level++;
  }
  return Level;
}

MPI_Comm hyperCube(int countLevel) {
  if (countLevel <= 0) throw "incorrect level";

  MPI_Comm hypercube;
  int *periods = new int[countLevel];
  int *dims = new int[countLevel];

  for (int i = 0; i < countLevel; i++) {
    dims[i] = 2;
    periods[i] = 1;
  }

  MPI_Cart_create(MPI_COMM_WORLD, countLevel, dims, periods, 0, &hypercube);
  return hypercube;
}
