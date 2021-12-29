// Copyright 2021 Pichugin Ilya
#ifndef MODULES_TASK_2_PICHUGIN_I_GATHER_GATHER_H_
#define MODULES_TASK_2_PICHUGIN_I_GATHER_GATHER_H_
#include <mpi.h>

#include <iostream>
#include <random>
#include <vector>

template <class T>
std::vector<T> getRandomVector(int Size);

int My_New_Gather(void *sbuf, int scount, MPI_Datatype stype, void *rbuf,
                  int rcount, MPI_Datatype rtype, int root, MPI_Comm comm);

#endif  //  MODULES_TASK_2_PICHUGIN_I_GATHER_GATHER_H_
