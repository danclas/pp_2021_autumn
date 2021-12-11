// Copyright 2021 Uglinskii Bogdan

#ifndef MODULES_TASK_3_UGLINSKII_B_CANNON_ALG_CANNON_H_
#define MODULES_TASK_3_UGLINSKII_B_CANNON_ALG_CANNON_H_
#include <mpi.h>

double* CreateEasyMatrix(int size);
double* CreateZeroMatrix(int size);
double* CreateEmptyMatrix(int size);
double* CreateRandomMatrix(int size);
double* SummMatix(int size, double* A, double* B);
double* SeqMulti(int size, double* A, double* B);
MPI_Comm CreateTopology(int block_num);
double* ParallelMulti(int size, double* A, double* B);

#endif  // MODULES_TASK_3_UGLINSKII_B_CANNON_ALG_CANNON_H_
