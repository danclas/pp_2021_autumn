// Copyright 2021 Brazhnik Dmitry
#ifndef MODULES_TASK_2_BRAZHNIK_D_FROM_ALL_TO_ONE_FROM_ALL_TO_ONE_H_
#define MODULES_TASK_2_BRAZHNIK_D_FROM_ALL_TO_ONE_FROM_ALL_TO_ONE_H_

#include <mpi.h>
#include <vector>
#include <random>

std::vector<int> fillRandomValToVectorInt(const int size);
std::vector<float> fillRandomValToVectorFloat(const int size);
std::vector<double> fillRandomValToVectorDouble(const int size);
int getRandVal();
double getRandValDouble();
float getRandValFloat();

int customReduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);


#endif  // MODULES_TASK_2_BRAZHNIK_D_FROM_ALL_TO_ONE_FROM_ALL_TO_ONE_H_
