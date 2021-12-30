// Copyright 2021 Shatalin Dmitriy
#include <mpi.h>
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <ctime>
#include <gtest-mpi-listener.hpp>

#ifndef MODULES_TASK_2_SHATALIN_D_ALLREDUCE_ALL_REDUCE_H_
#define MODULES_TASK_2_SHATALIN_D_ALLREDUCE_ALL_REDUCE_H_

template <typename T>
int buffer_calc(void* sendbuf, void* recvbuf, int count, MPI_Datatype type, MPI_Op Op, MPI_Comm Comm);

int MPI_allreduce_my(const void* sendbuf, void* recvbuf, int count, MPI_Datatype type, MPI_Op Op, MPI_Comm Comm);

template <typename T>
std::vector<T> vec_gen(int size);

#endif  // MODULES_TASK_2_SHATALIN_D_ALLREDUCE_ALL_REDUCE_H_
