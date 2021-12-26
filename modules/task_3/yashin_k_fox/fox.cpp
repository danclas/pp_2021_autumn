// Copyright 2021 Yashin Kirill

#include <mpi.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <random>
#include "../../../modules/task_3/yashin_k_fox/fox.h"

#define ndims 2
#define reoder true

int procNum, rank, cart_size, block_size;
int coords[2];
MPI_Comm cart_communicator, row_communicator, column_communicator;

void random_matrix(double* matrix, int size) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size * size; i++) {
    matrix[i] = static_cast<float>(gen() % 100) / 10;
  }
}

void fox_alg(double* A_block, double* A_mblock,
            double* B_block, double* C_block) {
  for (int it = 0; it < cart_size; it++) {
    int root = (coords[0] + it) % cart_size;
    if (coords[1] == root) {
      for (int i = 0; i < block_size * block_size; i++) {
        A_block[i] = A_mblock[i];
      }
    }
    MPI_Bcast(A_block, block_size * block_size, MPI_DOUBLE, root, row_communicator);

    double temp;
    for (int i = 0; i < block_size; i++)
      for (int j = 0; j < block_size; j++) {
        temp = 0;
        for (int k = 0; k < block_size; k++) {
          temp += A_block[i * block_size + k] * B_block[k * block_size + j];
        }
        C_block[i * block_size + j] += temp;
      }

    int dest = coords[0] - 1;
    if (coords[0] == 0) {
      dest = cart_size - 1;
    }
    int source = coords[0] + 1;
    if (coords[0] == cart_size - 1) {
      source = 0;
    }
    MPI_Sendrecv_replace(B_block, block_size * block_size, MPI_DOUBLE, dest, 0,
                        source, 0, column_communicator, MPI_STATUS_IGNORE);
  }
}

int fox(double* A, double* B, double* C, int size) {
  if (size <= 0) {
    return -1;
  }

  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (procNum == 1) {
    return sequential_alg(A, B, C, size);
  } else {
    cart_size = static_cast<int>(std::sqrt(procNum));
    if (cart_size * cart_size != procNum) {
      return -1;
    }
    if (size % cart_size != 0) {
      return -1;
    }
    block_size = size / cart_size;

    int elem_in_block = block_size * block_size;
    double* A_block = new double[elem_in_block];
    double* B_block = new double[elem_in_block];
    double* C_block = new double[elem_in_block];
    double* A_mblock = new double[elem_in_block];
    for (int i = 0; i < elem_in_block; i++) {
      C_block[i] = 0;
    }

    int dim_size[2] = {cart_size, cart_size};
    int periods[2] = {false, false};
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dim_size, periods, reoder, &cart_communicator);
    MPI_Cart_coords(cart_communicator, rank, ndims, coords);
    int subdims[2] = {false, true};
    MPI_Cart_sub(cart_communicator, subdims, &row_communicator);
    subdims[0] = true;
    subdims[1] = false;
    MPI_Cart_sub(cart_communicator, subdims, &column_communicator);

    double* rowbuff = new double[block_size * size];
    if (coords[1] == 0) {
      MPI_Scatter(A, block_size * size, MPI_DOUBLE, rowbuff,
                  block_size * size, MPI_DOUBLE, 0, column_communicator);
      MPI_Scatter(B, block_size * size, MPI_DOUBLE, rowbuff,
                  block_size * size, MPI_DOUBLE, 0, column_communicator);
    }
    for (int i = 0; i < block_size; i++) {
      MPI_Scatter(&rowbuff[i * size], block_size, MPI_DOUBLE, &(A_mblock[i * block_size]),
                  block_size, MPI_DOUBLE, 0, row_communicator);
      MPI_Scatter(&rowbuff[i * size], block_size, MPI_DOUBLE, &(B_block[i * block_size]),
                  block_size, MPI_DOUBLE, 0, row_communicator);
    }
    delete[] rowbuff;

    fox_alg(A_block, A_mblock, B_block, C_block);

    rowbuff = new double[size * block_size];
    for (int i = 0; i < block_size; i++) {
      MPI_Gather(&C_block[i * block_size], block_size, MPI_DOUBLE,
                &rowbuff[i * size], block_size, MPI_DOUBLE, 0, row_communicator);
    }
    if (coords[1] == 0) {
      MPI_Gather(rowbuff, block_size * size, MPI_DOUBLE, C, block_size * size,
                MPI_DOUBLE, 0, column_communicator);
    }

    delete[] rowbuff;
    delete[] A_block;
    delete[] B_block;
    delete[] C_block;
    delete[] A_mblock;

    return 0;
  }
}

int sequential_alg(double* A, double* B, double* C, int size) {
  if (size <= 0) {
    return -1;
  }
  for (int i = 0; i < size * size; i++) {
    C[i] = 0;
  }
  double temp;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      temp = 0;
      for (int k = 0; k < size; k++) {
        temp += A[i * size + k] * B[k * size + j];
      }
      C[i * size + j] += temp;
    }
  }
  return 0;
}
