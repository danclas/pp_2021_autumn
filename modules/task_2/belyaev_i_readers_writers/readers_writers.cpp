// Copyright 2021 Belyaev Ilya
#include "../../../modules/task_2/belyaev_i_readers_writers/readers_writers.h"
#include <mpi.h>
#include <chrono> // NOLINT [build/c++11]
#include <thread> // NOLINT [build/c++11]
#include <random>
#include <algorithm>
#include <string>




void write_s() {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::this_thread::sleep_for(std::chrono::milliseconds(15 + gen() % 15));
}



void read_s() {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::this_thread::sleep_for(std::chrono::milliseconds(15 + gen() % 15));
}


void stop(int procn) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::this_thread::sleep_for
  (std::chrono::milliseconds(procn * (30 + gen() % 10)));
}

void writers(int rank) {
  int inBuf[1] = {0};
  int outBuf[1] = {0};
  MPI_Status status;
  stop(rank);

  while (true) {
    MPI_Send(outBuf, 1, MPI_INT, 0, Query, MPI_COMM_WORLD);
    MPI_Recv(inBuf, 1, MPI_INT, 0, Answer, MPI_COMM_WORLD, &status);
    if (inBuf[0] != -1)
      break;
    else
      stop(rank);
  }

  while (true) {
    MPI_Send(outBuf, 1, MPI_INT, 0, A_Query, MPI_COMM_WORLD);
    MPI_Recv(inBuf, 1, MPI_INT, 0, A_Answer, MPI_COMM_WORLD, &status);
    if (inBuf[0] != -1)
      break;
    else
      stop(rank);
  }

  MPI_Send(outBuf, 1, MPI_INT, 0, Free, MPI_COMM_WORLD);
  write_s();
  MPI_Send(outBuf, 1, MPI_INT, 0, A_Free, MPI_COMM_WORLD);
  MPI_Send(outBuf, 1, MPI_INT, 0, END, MPI_COMM_WORLD);
  return;
}

void readers(int rank) {
  int RC = 0;
  int inBuf[1] = {0};
  int outBuf[1] = {0};
  MPI_Status status;
  stop(rank);

  while (true) {
    MPI_Send(outBuf, 1, MPI_INT, 0, Query, MPI_COMM_WORLD);
    MPI_Recv(inBuf, 1, MPI_INT, 0, Answer, MPI_COMM_WORLD, &status);
    if (inBuf[0] != -1)
      break;
    else
      stop(rank);
  }
  MPI_Send(outBuf, 1, MPI_INT, 0, Free, MPI_COMM_WORLD);
  while (true) {
    MPI_Send(outBuf, 1, MPI_INT, 0, R_Query, MPI_COMM_WORLD);
    MPI_Recv(inBuf, 1, MPI_INT, 0, R_Answer, MPI_COMM_WORLD, &status);
    if (inBuf[0] != -1)
      break;
    else
      stop(rank);
  }
  RC = inBuf[0];
  RC++;
  if (RC == 1) {
    while (true) {
      MPI_Send(outBuf, 1, MPI_INT, 0, A_Query, MPI_COMM_WORLD);
      MPI_Recv(inBuf, 1, MPI_INT, 0, A_Answer, MPI_COMM_WORLD,
               &status);
      if (inBuf[0] != -1)
        break;
      else
        stop(rank);
    }
  }
  outBuf[0] = RC;
  MPI_Send(outBuf, 1, MPI_INT, 0, R_Free, MPI_COMM_WORLD);
  read_s();
  while (true) {
    MPI_Send(outBuf, 1, MPI_INT, 0, R_Query, MPI_COMM_WORLD);
    MPI_Recv(inBuf, 1, MPI_INT, 0, R_Answer, MPI_COMM_WORLD, &status);
    if (inBuf[0] != -1)
      break;
    else
      stop(rank);
  }
  RC = inBuf[0];
  RC--;
  if (RC == 0) {
    MPI_Send(outBuf, 1, MPI_INT, 0, A_Free, MPI_COMM_WORLD);
  }
  outBuf[0] = RC;
  MPI_Send(outBuf, 1, MPI_INT, 0, R_Free, MPI_COMM_WORLD);
  MPI_Send(outBuf, 1, MPI_INT, 0, END, MPI_COMM_WORLD);
  return;
}



void storage(int rank, int procs) {
  int inBuf[1] = {0};
  int outBuf[1] = {0};
  int RWnumber;
  MPI_Status status;
  bool RC = true;
  bool Access = true;
  int ReadCount = 0;
  bool S = true;
  int* runs = new int[procs - 1];
  for (int i = 0; i < procs - 1; i++) {
    runs[i] = 0;
  }
  while (true) {
    if (procs != 1) {
      MPI_Recv(inBuf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
               MPI_COMM_WORLD, &status);
    } else {
      return;
    }
    RWnumber = status.MPI_SOURCE;
    if (status.MPI_TAG == Query) {
      if (S == true) {
        S = false;
        outBuf[0] = 1;
        MPI_Send(outBuf, 1, MPI_INT, RWnumber, Answer, MPI_COMM_WORLD);
      } else {
        outBuf[0] = -1;
        MPI_Send(outBuf, 1, MPI_INT, RWnumber, Answer, MPI_COMM_WORLD);
      }
    }
    if (status.MPI_TAG == Free) {
      if (S == false) {
        S = true;
      }
    }
    if (status.MPI_TAG == R_Query) {
      if (RC == true) {
        RC = false;
        outBuf[0] = ReadCount;
        MPI_Send(outBuf, 1, MPI_INT, RWnumber, R_Answer, MPI_COMM_WORLD);
      } else {
        outBuf[0] = -1;
        MPI_Send(outBuf, 1, MPI_INT, RWnumber, R_Answer, MPI_COMM_WORLD);
      }
    }
    if (status.MPI_TAG == R_Free) {
      if (RC == false) {
        RC = true;
        ReadCount = inBuf[0];
      }
    }
    if (status.MPI_TAG == A_Query) {
      if (Access == true) {
        Access = false;
        outBuf[0] = 1;
        MPI_Send(outBuf, 1, MPI_INT, RWnumber, A_Answer,
                 MPI_COMM_WORLD);
      } else {
        outBuf[0] = -1;
        MPI_Send(outBuf, 1, MPI_INT, RWnumber, A_Answer,
                 MPI_COMM_WORLD);
      }
    }
    if (status.MPI_TAG == A_Free) {
      if (Access == false) {
        Access = true;
      }
    }
    if (status.MPI_TAG == END) {
      runs[RWnumber - 1] += 1;
    }
    int runsCnt = 0;
    for (int i = 0; i < procs - 1; i++) {
      runsCnt += runs[i];
    }
    read_s();
    if (runsCnt == procs - 1) {
      break;
    }
  }
  return;
}
