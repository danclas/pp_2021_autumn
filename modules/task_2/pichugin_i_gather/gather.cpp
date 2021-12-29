// Copyright 2021 Pichugin Ilya
#include "../../../modules/task_2/pichugin_i_gather/gather.h"

template <class T>
std::vector<T> getRandomVector(int Size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<T> vec(Size);

  for (int i = 0; i < Size; i++) {
    vec[i] = static_cast<T>(gen() % 100);
  }

  return vec;
}

template std::vector<int> getRandomVector<int>(int Size);
template std::vector<float> getRandomVector<float>(int Size);
template std::vector<double> getRandomVector<double>(int Size);

int My_New_Gather(void* sbuf, int scount, MPI_Datatype stype, void* rbuf,
                  int rcount, MPI_Datatype rtype, int root, MPI_Comm comm) {
  int Rank, Size;
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);

  if (Rank == root) {
    MPI_Status Status;

    if ((stype != rtype) || (scount != rcount)) {
      throw -1;
    }

    if (rtype == MPI_INT) {
      for (int i = root * rcount; i < rcount * (root + 1); i++) {
        static_cast<int*>(rbuf)[i] = static_cast<int*>(sbuf)[i - root * rcount];
      }

      for (int i = 0; i < Size; i++) {
        if (i != root) {
          MPI_Recv(static_cast<int*>(rbuf) + rcount * i, rcount, rtype,
                   MPI_ANY_SOURCE, i, comm, &Status);
        }
      }
    }

    if (rtype == MPI_DOUBLE) {
      for (int i = root * rcount; i < rcount * (root + 1); i++) {
        static_cast<double*>(rbuf)[i] =
            static_cast<double*>(sbuf)[i - root * rcount];
      }

      for (int i = 0; i < Size; i++) {
        if (i != root) {
          MPI_Recv(static_cast<double*>(rbuf) + rcount * i, rcount, rtype,
                   MPI_ANY_SOURCE, i, comm, &Status);
        }
      }
    }

    if (rtype == MPI_FLOAT) {
      for (int i = root * rcount; i < rcount * (root + 1); i++) {
        static_cast<float*>(rbuf)[i] =
            static_cast<float*>(sbuf)[i - root * rcount];
      }

      for (int i = 0; i < Size; i++) {
        if (i != root) {
          MPI_Recv(static_cast<float*>(rbuf) + rcount * i, rcount, rtype,
                   MPI_ANY_SOURCE, i, comm, &Status);
        }
      }
    }

    if (rtype != MPI_FLOAT || rtype == MPI_INT || rtype == MPI_DOUBLE) {
      return -1;
    }

  } else {
    MPI_Send(sbuf, scount, stype, root, Rank, comm);
  }

  return 0;
}
