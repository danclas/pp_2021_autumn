// Copyright 2021 Ershov Alexey
#ifndef MODULES_TASK_2_ERSHOV_A_REDUCE_REDUCE_MPI_H_
#define MODULES_TASK_2_ERSHOV_A_REDUCE_REDUCE_MPI_H_

#include <mpi.h>

#include <vector>

struct Matrix {
  double* data;
  size_t weight;
  size_t height;
  Matrix();
  Matrix(double* data, size_t weight, size_t height);
  Matrix(const Matrix& a);
  ~Matrix();
  Matrix& operator=(const Matrix& right);
  void setMatrix(double* data, size_t weight, size_t height);
};

Matrix getRandomMatrix(const size_t m, const size_t n);

template <typename T>
T* getRandomVector(size_t size);

int reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype type,
           MPI_Op op, int root, MPI_Comm comm);

void sumInt(void* buf, void* tmp, const int count);

void maxInt(void* buf, void* tmp, const int count);

void minInt(void* buf, void* tmp, const int count);

void prodInt(void* buf, void* tmp, const int count);

void sumFloat(void* buf, void* tmp, const int count);

void maxFloat(void* buf, void* tmp, const int count);

void minFloat(void* buf, void* tmp, const int count);

void prodFloat(void* buf, void* tmp, const int count);

void sumDouble(void* buf, void* tmp, const int count);

void maxDouble(void* buf, void* tmp, const int count);

void minDouble(void* buf, void* tmp, const int count);

void prodDouble(void* buf, void* tmp, const int count);

#endif  // MODULES_TASK_2_ERSHOV_A_REDUCE_REDUCE_MPI_H_
