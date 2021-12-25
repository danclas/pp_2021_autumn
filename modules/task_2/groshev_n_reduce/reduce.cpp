// Copyright 2021 Groshev Nikolay
#include "../../../modules/task_2/groshev_n_reduce/reduce.h"

void my_reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm) {
  int ProcNum;
  int ProcRank;
  MPI_Comm_size(comm, &ProcNum);
  MPI_Comm_rank(comm, &ProcRank);

  if (op == MPI_SUM) {
    sum(sendbuf, recvbuf, count, datatype, op, root, comm, ProcRank);
  } else if (op == MPI_PROD) {
    prod(sendbuf, recvbuf, count, datatype, op, root, comm, ProcRank);
  } else if (op == MPI_MAX) {
    max(sendbuf, recvbuf, count, datatype, op, root, comm, ProcRank);
  } else if (op == MPI_MIN) {
    min(sendbuf, recvbuf, count, datatype, op, root, comm, ProcRank);
  }
}

void sum(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
         MPI_Op op, int root, MPI_Comm comm, int my_rank) {
  int ProcNum;
  int* buffer = new int[count];
  for (int i = 0; i < count; i++) {
    buffer[i] = 0;
  }
  MPI_Comm_size(comm, &ProcNum);
  if (my_rank == root) {
    for (int j = 0; j < count; j++) {
      static_cast<int*>(buffer)[j] += static_cast<int*>(sendbuf)[j];
    }
    for (int i = 1; i < ProcNum; i++) {
      MPI_Recv(recvbuf, count, datatype, MPI_ANY_SOURCE, 0, comm,
               MPI_STATUS_IGNORE);
      for (int j = 0; j < count; j++) {
        static_cast<int*>(buffer)[j] += static_cast<int*>(recvbuf)[j];
      }
    }
    for (int j = 0; j < count; j++) {
      static_cast<int*>(recvbuf)[j] = static_cast<int*>(buffer)[j];
    }
  } else {
    MPI_Send(sendbuf, count, datatype, root, 0, comm);
  }
}

void prod(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
          MPI_Op op, int root, MPI_Comm comm, int my_rank) {
  int ProcNum;
  int* buffer = new int[count];
  for (int i = 0; i < count; i++) {
    buffer[i] = 1;
  }
  MPI_Comm_size(comm, &ProcNum);
  if (my_rank == root) {
    for (int j = 0; j < count; j++) {
      static_cast<int*>(buffer)[j] *= static_cast<int*>(sendbuf)[j];
    }
    for (int i = 1; i < ProcNum; i++) {
      MPI_Recv(recvbuf, count, datatype, MPI_ANY_SOURCE, 0, comm,
               MPI_STATUS_IGNORE);
      for (int j = 0; j < count; j++) {
        static_cast<int*>(buffer)[j] *= static_cast<int*>(recvbuf)[j];
      }
    }
    for (int j = 0; j < count; j++) {
      static_cast<int*>(recvbuf)[j] = static_cast<int*>(buffer)[j];
    }
  } else {
    MPI_Send(sendbuf, count, datatype, root, 0, comm);
  }
}

void max(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
         MPI_Op op, int root, MPI_Comm comm, int my_rank) {
  int ProcNum;
  int* buffer = new int[count];
  for (int i = 0; i < count; i++) {
    buffer[i] = -2147483647;
  }
  MPI_Comm_size(comm, &ProcNum);
  if (my_rank == root) {
    for (int j = 0; j < count; j++) {
      if (static_cast<int*>(buffer)[j] < static_cast<int*>(sendbuf)[j]) {
        static_cast<int*>(buffer)[j] = static_cast<int*>(sendbuf)[j];
      }
    }
    for (int i = 1; i < ProcNum; i++) {
      MPI_Recv(recvbuf, count, datatype, MPI_ANY_SOURCE, 0, comm,
               MPI_STATUS_IGNORE);
      for (int j = 0; j < count; j++) {
        if (static_cast<int*>(buffer)[j] < static_cast<int*>(recvbuf)[j]) {
          static_cast<int*>(buffer)[j] = static_cast<int*>(recvbuf)[j];
        }
      }
    }
    for (int j = 0; j < count; j++) {
      static_cast<int*>(recvbuf)[j] = static_cast<int*>(buffer)[j];
    }
  } else {
    MPI_Send(sendbuf, count, datatype, root, 0, comm);
  }
}

void min(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
         MPI_Op op, int root, MPI_Comm comm, int my_rank) {
  int ProcNum;
  int* buffer = new int[count];
  for (int i = 0; i < count; i++) {
    buffer[i] = 2147483647;
  }
  MPI_Comm_size(comm, &ProcNum);
  if (my_rank == root) {
    for (int j = 0; j < count; j++) {
      if (static_cast<int*>(buffer)[j] > static_cast<int*>(sendbuf)[j]) {
        static_cast<int*>(buffer)[j] = static_cast<int*>(sendbuf)[j];
      }
    }
    for (int i = 1; i < ProcNum; i++) {
      MPI_Recv(recvbuf, count, datatype, MPI_ANY_SOURCE, 0, comm,
               MPI_STATUS_IGNORE);
      for (int j = 0; j < count; j++) {
        if (static_cast<int*>(buffer)[j] > static_cast<int*>(recvbuf)[j]) {
          static_cast<int*>(buffer)[j] = static_cast<int*>(recvbuf)[j];
        }
      }
    }
    for (int j = 0; j < count; j++) {
      static_cast<int*>(recvbuf)[j] = static_cast<int*>(buffer)[j];
    }
  } else {
    MPI_Send(sendbuf, count, datatype, root, 0, comm);
  }
}
