// Copyright [2021] <Borisov Maxim>
#include "../../../modules/task_2/borisov_m_scatter/scatter.h"
#include <cstring>

int my_scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
               MPI_Comm comm) {
    int proc_count;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

    int sendtype_size;
    int recvtype_size;

    if (sendcount != recvcount ||
            (sendcount <= 0 || recvcount <= 0) || root < 0)
        return MPI_ERR_COUNT;

    MPI_Type_size(sendtype, &sendtype_size);
    MPI_Type_size(recvtype, &recvtype_size);

    if (proc_rank == root) {
        memcpy(recvbuf, reinterpret_cast<int8_t*>(sendbuf) +
                root * recvcount * recvtype_size,
                sendcount * recvtype_size);
        for (int i = 0; i < proc_count; i++) {
            if (i == root)
                continue;
            MPI_Send(reinterpret_cast<int8_t*>(sendbuf) +
                    i * sendcount * sendtype_size,
                    sendcount, sendtype, i, 0, comm);
        }
    } else {
        MPI_Status status;
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0 , comm, &status);
    }

    return MPI_SUCCESS;
}
