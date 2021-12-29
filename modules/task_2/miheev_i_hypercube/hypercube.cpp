// Copyright 2021 Miheev Ivan
#include "../../../modules/task_2/miheev_i_hypercube/hypercube.h"

MPI_Comm hypercube(int dimensions_hybercube, int size) {
  if (dimensions_hybercube < 2 || size < 2) {
    throw std::logic_error("dims or size");
  }
  int ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  std::vector<int> dims(dimensions_hybercube, size);
  std::vector<int> periods(dimensions_hybercube, 1);

  MPI_Comm hypercube;
  MPI_Cart_create(MPI_COMM_WORLD, dimensions_hybercube, dims.data(),
                  periods.data(), true, &hypercube);

  return hypercube;
}
