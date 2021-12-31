// Copyright 2021 Sovetov Egor


#include <mpi.h>
#include <algorithm>
#include <functional>
#include <vector>
#include "../../../modules/task_2/sovetov_e_scatter/scatter.h"

int Scatter(void* send_data, int send_count, MPI_Datatype send_datatype,
            void* recv_data, int recv_count, MPI_Datatype recv_datatype,
            int prod, MPI_Comm communicator) {
  if (send_count <= 0 || recv_count <= 0 || prod < 0) throw -1;
  int razmer, bees;
  int razmerElementa, razmerElement;
  MPI_Comm_size(communicator, &razmer);
  MPI_Comm_rank(communicator, &bees);

  MPI_Type_size(send_datatype, &razmerElement);
  MPI_Type_size(recv_datatype, &razmerElementa);

  if (bees == prod) {
    memcpy(
        recv_data,
        reinterpret_cast<char*>(send_data) + prod * send_count * razmerElementa,
        send_count * razmerElement);
    for (int num = 0; num < razmer; num++) {
      if (num == prod) continue;
      MPI_Send(
          reinterpret_cast<char*>(send_data) + num * send_count * razmerElement,
          send_count, send_datatype, num, 0, communicator);
    }
  } else {
    MPI_Status status;
    MPI_Recv(recv_data, recv_count, recv_datatype, prod, 0, communicator,
             &status);
  }
  return MPI_SUCCESS;
}

int FasterScatter(void* send_data, int send_count, MPI_Datatype send_datatype,
                  void* recv_data, int recv_count, MPI_Datatype recv_datatype,
                  int prod, MPI_Comm communicator) {
  if (send_count <= 0 || recv_count <= 0 || prod < 0) throw -1;
  int razmer, bees;
  int razmerElement;
  MPI_Comm_size(communicator, &razmer);
  MPI_Comm_rank(communicator, &bees);

  MPI_Type_size(send_datatype, &razmerElement);

  int razmerLocal = 1;
  int num = 1;
  int raber;
  if (razmerLocal == razmer) {
    memcpy(recv_data, reinterpret_cast<char*>(send_data),
           send_count * razmerElement);
  } else {
    while (num <= razmer) {
      if (num == 1 && bees == 0) {
        memcpy(recv_data, reinterpret_cast<char*>(send_data),
               send_count * razmerElement);
      }
      if (bees >= num - 1 && bees < razmerLocal && bees != 0) {
        raber = bees - static_cast<int>(pow(2.0, num - 2));
        MPI_Status status;
        MPI_Recv(recv_data, send_count, send_datatype, raber, 0, communicator,
                 &status);
      }
      if (bees < razmerLocal) {
        raber = bees + static_cast<int>(pow(2.0, num - 1));
        if (raber >= razmer) break;
        MPI_Send(reinterpret_cast<char*>(send_data) +
                     raber * send_count * razmerElement,
                 send_count, send_datatype, raber, 0, communicator);
      }
      razmerLocal += static_cast<int>(pow(2.0, num - 1));
      num++;
    }
  }
  return MPI_SUCCESS;
}

int MyTest(std::vector<int> mrot,
           std::function<int(void*, int, MPI_Datatype, void*, int, MPI_Datatype,
                             int, MPI_Comm)>
               func) {
  int razmer, bees;
  MPI_Comm_size(MPI_COMM_WORLD, &razmer);
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);
  int ji = mrot.size();
  int MaximResult = 0;
  std::vector<int> rez(ji / razmer);
  func(mrot.data(), ji / razmer, MPI_INT, rez.data(), ji / razmer, MPI_INT, 0,
       MPI_COMM_WORLD);

  int maximum = *std::max_element(rez.begin(), rez.end());
  MPI_Reduce(&maximum, &MaximResult, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return MaximResult;
}
