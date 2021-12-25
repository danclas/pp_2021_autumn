// Copyright 2021 Myasnikova Varvara

#include "../../../modules/task_2/myasnikova_hypercube/myasnikova_hypercube.h"

MPI_Comm CreateHypercube(int ndims, int dims) {
  if (ndims < 2 || dims < 2) {
    throw "Error";
  }

  // The number of processes in each dimension of the lattice
  int* dims_mass = new int[ndims];
  // Determining whether the lattice is periodic (looped)
  int* periods = new int[ndims];
  // Changing the order of process ranks
  int reorder = 1;

  for (int i = 0; i < ndims; i++) {
    dims_mass[i] = dims;
    periods[i] = 1;
  }

  MPI_Comm hypercube;  // New communicator

  // Filling out the communicator
  MPI_Cart_create(MPI_COMM_WORLD, ndims, dims_mass,
    periods, reorder, &hypercube);

  return hypercube;
}

// Checking the correctness of the hypercube
bool HypercubeCorrection(MPI_Comm hypercube_comm, int ndims, int dims) {
  if (ndims < 2 || dims < 2) {
    throw "Error";
  }

  bool res = true;
  int* dims_mass = new int[ndims];
  int* periods = new int[ndims];
  // Cartesian coordinates of the process
  int* coord = new int[ndims];

  for (int i = 0; i < ndims; i++) {
    dims_mass[i] = dims;
  }

  // Getting coordinates at a given point
  MPI_Cart_get(hypercube_comm, ndims, dims_mass, periods, coord);
  for (int i = 0; i < ndims; i++) {
    if (periods[i] != 1) {
      res = false;
      break;
    }
  }

  return res;
}

// Data transfer in a hypercube
bool HypercubeData(MPI_Comm hypercube_comm, int ndims, int dims) {
  if (ndims < 2 || dims < 2) {
    throw "Error";
  }

  bool res = true;
  int ProcRank;
  // The rank of the process from which the data should be obtained
  int source;
  // The rank of the process to which the data should be sent
  int dst;
  int local_data = 0, global_data = 0, test_data = 1;
  MPI_Comm_rank(hypercube_comm, &ProcRank);

  for (int i = 0; i < ndims; i++) {
    MPI_Cart_shift(hypercube_comm, i, 1, &source, &dst);
    if ((source == MPI_PROC_NULL) || (dst == MPI_PROC_NULL)) {
      res = false;
      break;
    }
  }

  if (res) {
    if (ProcRank == 0) {
      local_data = 1;
    }

    MPI_Bcast(&local_data, 1, MPI_INT, 0, hypercube_comm);
    MPI_Reduce(&local_data, &global_data, 1, MPI_INT, MPI_SUM, 0, hypercube_comm);

    for (int i = 0; i < ndims; i++) {
      test_data *= dims;
    }

    if (test_data != global_data) {
      res = false;
    }
  }

  return res;
}
