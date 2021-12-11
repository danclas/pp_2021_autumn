// Copyright 2021 Uglinskii Bogdan
#include "../../../modules/task_3/uglinskii_b_cannon_alg/Cannon.h"
#include <random>
#include <ctime>

double* CreateZeroMatrix(int size) {
  double* matrix = new double[size * size];
  for (int i = 0; i < size * size; i++) {
    matrix[i] = 0;
  }
  return matrix;
}

double* CreateEmptyMatrix(int size) {
  double* matrix = new double[size * size];
  return matrix;
}
double* CreateEasyMatrix(int size) {
  double* matrix = new double[size * size];

  for (int i = 0; i < size * size; i++) {
    matrix[i] = 1 + i % size;
  }
  return matrix;
}

double* CreateRandomMatrix(int size) {
  std::mt19937 gen(time(0));
  std::uniform_real_distribution<> urd(-10, 10);
  double* matrix = new double[size * size];
  for (int i = 0; i < size*size; i++) {
    matrix[i] = urd(gen);
  }
  return matrix;
}

double* SummMatix(int size, double* A, double* B) {
  double* C = CreateZeroMatrix(size);
  for (int i = 0; i < size * size; i++) {
    C[i] = A[i] + B[i];
  }
  return C;
}

double* SeqMulti(int size, double* A, double* B) {
  double* C = CreateZeroMatrix(size);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      for (int l = 0; l < size; l++) {
        C[i * size + j] += A[i * size + l] * B[l * size + j];
      }
    }
  }
  return C;
}

MPI_Comm CreateTopology(int block_num) {
  int ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (block_num > 0 && (block_num * block_num > ProcNum)) {
    return MPI_COMM_NULL;
  }

  int dims[2] = {block_num, block_num};
  int periods[2] = {1, 1};
  int reorder = 0;
  MPI_Comm new_comm;
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &new_comm);

  return new_comm;
}

double* ParallelMulti(int size, double* A, double* B) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if ((static_cast<int>(std::sqrt(ProcNum)) *
           static_cast<int>(std::sqrt(ProcNum)) !=
       ProcNum) ||
      (size % static_cast<int>(sqrt(ProcNum)) != 0)) {
    if (ProcRank ==
        0) {
      return SeqMulti(size, A, B);
    } else {
      return CreateZeroMatrix(size);
    }
  }

  double* C = CreateZeroMatrix(size);

  int block_num = static_cast<int>(std::sqrt(ProcNum));
  int block_size = size / block_num;

  int global_sizes[2] = {size, size};
  int subsizes[2] = {block_size, block_size};
  int starts[2] = {0, 0};

  MPI_Datatype type, resized_type;
  MPI_Type_create_subarray(2, global_sizes, subsizes, starts, MPI_ORDER_C,
                           MPI_DOUBLE, &type);
  MPI_Type_create_resized(type, 0, block_size * sizeof(double), &resized_type);
  MPI_Type_commit(&resized_type);

  int* send_counts = new int[ProcNum];
  int* displs = new int[ProcNum];

  if (ProcRank == 0) {
    for (int i = 0; i < ProcNum; i++) {
      send_counts[i] = 1;
    }
    int cur_disp = 0;
    for (int i = 0; i < block_num; i++) {
      for (int j = 0; j < block_num; j++) {
        displs[i * block_num + j] = cur_disp;
        cur_disp += 1;
      }
      cur_disp += (block_size - 1) * block_num;
    }
  }

  MPI_Comm grid_comm = CreateTopology(block_num);

  double* local_A = CreateEmptyMatrix(block_size);
  double* local_B = CreateEmptyMatrix(block_size);

  MPI_Scatterv(&(A[0]), send_counts, displs, resized_type, &(local_A[0]),
               (size * size) / ProcNum, MPI_DOUBLE, 0, grid_comm);
  MPI_Scatterv(&(B[0]), send_counts, displs, resized_type, &(local_B[0]),
               (size * size) / ProcNum, MPI_DOUBLE, 0, grid_comm);

  int my_coords[2];
  MPI_Cart_coords(grid_comm, ProcRank, 2, my_coords);

  MPI_Status status;
  int L, R;
  MPI_Cart_shift(grid_comm, 1, my_coords[0], &L, &R);
  MPI_Sendrecv_replace(&(local_A[0]), block_size * block_size, MPI_DOUBLE, L, 1,
                       R, 1, grid_comm, &status);

  int UP, D;
  MPI_Cart_shift(grid_comm, 0, my_coords[1], &UP, &D);
  MPI_Sendrecv_replace(&(local_B[0]), block_size * block_size, MPI_DOUBLE, UP,
                       1, D, 1, grid_comm, &status);

  double* local_C = CreateZeroMatrix(block_size);
  double* temp_res = CreateZeroMatrix(block_size);

  for (int q = 0; q < block_num; q++) {
    temp_res = SeqMulti(block_size, local_A, local_B);

    local_C = SummMatix(block_size, local_C, temp_res);

    MPI_Cart_shift(grid_comm, 1, 1, &L, &R);
    MPI_Cart_shift(grid_comm, 0, 1, &UP, &D);

    MPI_Sendrecv_replace(&(local_A[0]), block_size * block_size, MPI_DOUBLE, L,
                         1, R, 1, grid_comm, &status);
    MPI_Sendrecv_replace(&(local_B[0]), block_size * block_size, MPI_DOUBLE, UP,
                         1, D, 1, grid_comm, &status);
  }
  MPI_Gatherv(&(local_C[0]), size * size / ProcNum, MPI_DOUBLE, &(C[0]),
              send_counts, displs, resized_type, 0, grid_comm);
  return C;
}
