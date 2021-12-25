// Copyright 2021 Kazhaeva Anastasia
#include <time.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/kazhaeva_a_allreduce/allreduce.h"


template <typename T>
int OperationsBuff(void* sendbuf, void* recvbuf,
  int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
  int ProkSize;
  MPI_Comm_size(comm, &ProkSize);
  std::vector<T> buf_root;
  std::vector<T> tmp_buf(count);
  for (int i = 0; i < count; i++) {
    buf_root.push_back(static_cast<T*>(sendbuf)[i]);
  }
  for (int i = 0; i < ProkSize - 1; i++) {
    MPI_Recv(tmp_buf.data(), count, datatype,
      MPI_ANY_SOURCE, 1, comm, MPI_STATUS_IGNORE);
    if (op == MPI_MAX) {
      for (int i = 0; i < count; i++) {
        buf_root[i] = std::max(buf_root[i], tmp_buf[i]);
      }
    }
    if (op == MPI_MIN) {
      for (int i = 0; i < count; i++) {
        buf_root[i] = std::min(buf_root[i], tmp_buf[i]);
      }
    }
    if (op == MPI_SUM) {
      for (int i = 0; i < count; i++) {
        buf_root[i] += tmp_buf[i];
      }
    }
  }
  tmp_buf.clear();
  for (int i = 0; i < count; i++) {
    (static_cast<T*>(recvbuf))[i] = buf_root[i];
  }
  buf_root.clear();
  return MPI_SUCCESS;
}

int My_MPI_Allreduce(const void* sendbuf, void* recvbuf,
  int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
  int status = MPI_SUCCESS;
  int root = 0;
  int rank, size;

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  if (rank == 0) {
    std::mt19937 gen(time(0));
    root = gen() % size;
  }
  MPI_Bcast(&root, 1, MPI_INT, 0, comm);

  if (rank == root) {
    if (datatype == MPI_INT) {
      OperationsBuff<int>(const_cast<void*>(sendbuf),
        recvbuf, count, datatype, op, comm);
    } else if (datatype == MPI_FLOAT) {
      OperationsBuff<float>(const_cast<void*>(sendbuf),
        recvbuf, count, datatype, op, comm);
    } else if (datatype == MPI_DOUBLE) {
      OperationsBuff<double>(const_cast<void*>(sendbuf),
        recvbuf, count, datatype, op, comm);
    } else { status = MPI_ERR_TYPE; }
  } else {
    MPI_Send(sendbuf, count, datatype, root, 1, comm);
    MPI_Recv(recvbuf, count, datatype, MPI_ANY_SOURCE,
      1, comm, MPI_STATUS_IGNORE);
    MPI_Recv(&status, 1, MPI_INT, MPI_ANY_SOURCE, 2,
      comm, MPI_STATUS_IGNORE);
  }
  if (size != 1) {
    if (rank == root) {
      if (root == 0) {
        MPI_Send(recvbuf, count, datatype, 1, 1, comm);
        MPI_Send(&status, 1, MPI_INT, 1, 2, comm);
      } else if (root == 1) {
        MPI_Send(recvbuf, count, datatype, 0, 1, comm);
        MPI_Send(&status, 1, MPI_INT, 0, 2, comm);
      } else {
        MPI_Send(recvbuf, count, datatype, 0, 1, comm);
        MPI_Send(&status, 1, MPI_INT, 0, 2, comm);

        MPI_Send(recvbuf, count, datatype, 1, 1, comm);
        MPI_Send(&status, 1, MPI_INT, 1, 2, comm);
      }
    }
    int next = 0;
    next = rank * 2 + 2;
    if (next != root && next < size) {
      MPI_Send(recvbuf, count, datatype, next, 1, comm);
      MPI_Send(&status, 1, MPI_INT, next, 2, comm);
    }
    next = rank * 2 + 3;
    if (next != root && next < size) {
      MPI_Send(recvbuf, count, datatype, next, 1, comm);
      MPI_Send(&status, 1, MPI_INT, next, 2, comm);
    }
  }
  MPI_Barrier(comm);
  return status;
}
template <typename T>
std::vector<T> GetRandomVector(int n, int max) {
  std::vector<T> vec(n);
  std::mt19937 gen(time(0));
  for (int i = 0; i < n; i++) {
    vec[i] = gen() % max;
  }
  return vec;
}
template
std::vector<int> GetRandomVector(int n, int max);
template
std::vector<float> GetRandomVector(int n, int max);
template
std::vector<double> GetRandomVector(int n, int max);
