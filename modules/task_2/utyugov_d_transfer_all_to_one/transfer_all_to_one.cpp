// Copyright 2021 Utyugov Denis
#include "../../../modules/task_2/utyugov_d_transfer_all_to_one/transfer_all_to_one.h"

#include <mpi.h>

#include <random>
#include <string>
#include <vector>

std::vector<int> getRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = gen() % 100;
  }
  return vec;
}

// only MPI_Send and MPI_Recv.
// Must realise MPI_INT, MPI_DOUBLE, MPI_FLOAT.

int MY_Reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
              MPI_Op op, int root, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  root = 0;

  int num = 0;
  bool key = true;
  void* val;

  if (datatype == MPI_INT) {
    val = new int[count];
  }
  if (datatype == MPI_FLOAT) {
    val = new float[count];
  } else {
    val = new double[count];
  }

  if (rank % 2 == 1) {  // Odd set data
    MPI_Send(sendbuf, count, datatype, rank - 1, 0, comm);
    key = false;
  } else {
    if (rank + 1 < size) {  // 0 + 1 < 1 false, 0 + 1 < 2 true
      MPI_Recv(val, count, datatype, rank + 1, 0, comm, MPI_STATUS_IGNORE);
      if (datatype == MPI_INT) {
        if (op == MPI_SUM) {
          for (int i = 0; i < count; i++) {
            reinterpret_cast<int*>(sendbuf)[i] +=
                reinterpret_cast<int*>(val)[i];
          }
        } else if (op == MPI_PROD) {
          for (int i = 0; i < count; i++) {
            reinterpret_cast<int*>(sendbuf)[i] *=
                reinterpret_cast<int*>(val)[i];
          }
        } else if (op == MPI_MAX) {
          for (int i = 0; i < count; i++) {
            if (reinterpret_cast<int*>(sendbuf)[i] <
                reinterpret_cast<int*>(val)[i]) {
              reinterpret_cast<int*>(sendbuf)[i] =
                  reinterpret_cast<int*>(val)[i];
            }
          }
        } else if (op == MPI_MIN) {
          for (int i = 0; i < count; i++) {
            if (reinterpret_cast<int*>(sendbuf)[i] >
                reinterpret_cast<int*>(val)[i]) {
              reinterpret_cast<int*>(sendbuf)[i] =
                  reinterpret_cast<int*>(val)[i];
            }
          }
        }
      } else if (datatype == MPI_FLOAT) {
        if (op == MPI_SUM) {
          for (int i = 0; i < count; i++) {
            reinterpret_cast<float*>(sendbuf)[i] +=
                reinterpret_cast<float*>(val)[i];
          }
        } else if (op == MPI_PROD) {
          for (int i = 0; i < count; i++) {
            reinterpret_cast<float*>(sendbuf)[i] *=
                reinterpret_cast<float*>(val)[i];
          }
        } else if (op == MPI_MAX) {
          for (int i = 0; i < count; i++) {
            if (reinterpret_cast<float*>(sendbuf)[i] <
                reinterpret_cast<float*>(val)[i]) {
              reinterpret_cast<float*>(sendbuf)[i] =
                  reinterpret_cast<float*>(val)[i];
            }
          }
        } else if (op == MPI_MIN) {
          for (int i = 0; i < count; i++) {
            if (reinterpret_cast<float*>(sendbuf)[i] >
                reinterpret_cast<float*>(val)[i]) {
              reinterpret_cast<float*>(sendbuf)[i] =
                  reinterpret_cast<float*>(val)[i];
            }
          }
        }
      } else {  // double
        if (op == MPI_SUM) {
          for (int i = 0; i < count; i++) {
            reinterpret_cast<double*>(sendbuf)[i] +=
                reinterpret_cast<double*>(val)[i];
          }
        } else if (op == MPI_PROD) {
          for (int i = 0; i < count; i++) {
            reinterpret_cast<double*>(sendbuf)[i] *=
                reinterpret_cast<double*>(val)[i];
          }
        } else if (op == MPI_MAX) {
          for (int i = 0; i < count; i++) {
            if (reinterpret_cast<double*>(sendbuf)[i] <
                reinterpret_cast<double*>(val)[i]) {
              reinterpret_cast<double*>(sendbuf)[i] =
                  reinterpret_cast<double*>(val)[i];
            }
          }
        } else if (op == MPI_MIN) {
          for (int i = 0; i < count; i++) {
            if (reinterpret_cast<double*>(sendbuf)[i] >
                reinterpret_cast<double*>(val)[i]) {
              reinterpret_cast<double*>(sendbuf)[i] =
                  reinterpret_cast<double*>(val)[i];
            }
          }
        }
      }
    }
  }
  num = 1;
  while (key == true) {  // Data cycle
    num = num * 2;

    if (num > size) {
      key = false;
      break;
    }

    if (rank % (num * 2) != 0) {
      key = false;
      MPI_Send(sendbuf, count, datatype, rank - num, 0, comm);
      break;
    } else {
      if (rank + num + 1 <= size) {
        MPI_Recv(val, count, datatype, rank + num, 0, comm, MPI_STATUS_IGNORE);
        if (datatype == MPI_INT) {
          if (op == MPI_SUM) {
            for (int i = 0; i < count; i++) {
              reinterpret_cast<int*>(sendbuf)[i] +=
                  reinterpret_cast<int*>(val)[i];
            }
          } else if (op == MPI_PROD) {
            for (int i = 0; i < count; i++) {
              reinterpret_cast<int*>(sendbuf)[i] *=
                  reinterpret_cast<int*>(val)[i];
            }
          } else if (op == MPI_MAX) {
            for (int i = 0; i < count; i++) {
              if (reinterpret_cast<int*>(sendbuf)[i] <
                  reinterpret_cast<int*>(val)[i]) {
                reinterpret_cast<int*>(sendbuf)[i] =
                    reinterpret_cast<int*>(val)[i];
              }
            }
          } else if (op == MPI_MIN) {
            for (int i = 0; i < count; i++) {
              if (reinterpret_cast<int*>(sendbuf)[i] >
                  reinterpret_cast<int*>(val)[i]) {
                reinterpret_cast<int*>(sendbuf)[i] =
                    reinterpret_cast<int*>(val)[i];
              }
            }
          }
        } else if (datatype == MPI_FLOAT) {
          if (op == MPI_SUM) {
            for (int i = 0; i < count; i++) {
              reinterpret_cast<float*>(sendbuf)[i] +=
                  reinterpret_cast<float*>(val)[i];
            }
          } else if (op == MPI_PROD) {
            for (int i = 0; i < count; i++) {
              reinterpret_cast<float*>(sendbuf)[i] *=
                  reinterpret_cast<float*>(val)[i];
            }
          } else if (op == MPI_MAX) {
            for (int i = 0; i < count; i++) {
              if (reinterpret_cast<float*>(sendbuf)[i] <
                  reinterpret_cast<float*>(val)[i]) {
                reinterpret_cast<float*>(sendbuf)[i] =
                    reinterpret_cast<float*>(val)[i];
              }
            }
          } else if (op == MPI_MIN) {
            for (int i = 0; i < count; i++) {
              if (reinterpret_cast<float*>(sendbuf)[i] >
                  reinterpret_cast<float*>(val)[i]) {
                reinterpret_cast<float*>(sendbuf)[i] =
                    reinterpret_cast<float*>(val)[i];
              }
            }
          }
        } else {  // double
          if (op == MPI_SUM) {
            for (int i = 0; i < count; i++) {
              reinterpret_cast<double*>(sendbuf)[i] +=
                  reinterpret_cast<double*>(val)[i];
            }
          } else if (op == MPI_PROD) {
            for (int i = 0; i < count; i++) {
              reinterpret_cast<double*>(sendbuf)[i] *=
                  reinterpret_cast<double*>(val)[i];
            }
          } else if (op == MPI_MAX) {
            for (int i = 0; i < count; i++) {
              if (reinterpret_cast<double*>(sendbuf)[i] <
                  reinterpret_cast<double*>(val)[i]) {
                reinterpret_cast<double*>(sendbuf)[i] =
                    reinterpret_cast<double*>(val)[i];
              }
            }
          } else if (op == MPI_MIN) {
            for (int i = 0; i < count; i++) {
              if (reinterpret_cast<double*>(sendbuf)[i] >
                  reinterpret_cast<double*>(val)[i]) {
                reinterpret_cast<double*>(sendbuf)[i] =
                    reinterpret_cast<double*>(val)[i];
              }
            }
          }
        }
      } else {
        continue;
      }
    }
  }

  if (rank == root) {
    if (datatype == MPI_INT) {
      for (int i = 0; i < count; i++) {
        reinterpret_cast<int*>(recvbuf)[i] = reinterpret_cast<int*>(sendbuf)[i];
      }
    } else if (datatype == MPI_FLOAT) {
      for (int i = 0; i < count; i++) {
        reinterpret_cast<float*>(recvbuf)[i] =
            reinterpret_cast<float*>(sendbuf)[i];
      }
    } else {
      for (int i = 0; i < count; i++) {
        reinterpret_cast<double*>(recvbuf)[i] =
            reinterpret_cast<double*>(sendbuf)[i];
      }
    }
  }

  return 0;
}
