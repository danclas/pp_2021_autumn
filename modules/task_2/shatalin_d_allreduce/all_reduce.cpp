// Copyright 2021 Shatalin Dmitriy
#include "../../../modules/task_2/shatalin_d_allreduce/all_reduce.h"

template <typename T>
int buffer_calc(void* sendbuf, void* recvbuf, int count, MPI_Datatype type, MPI_Op Op, MPI_Comm Comm) {
    std::vector<T> root_buff;
    std::vector<T> tmp_buff(count);

    for (int i = 0; i < count; i++) {
        root_buff.push_back(static_cast<T*>(sendbuf)[i]);
    }

    int size;
    MPI_Comm_size(Comm, &size);

    while (--size) {
        MPI_Recv(tmp_buff.data(), count, type, MPI_ANY_SOURCE, 1, Comm, MPI_STATUS_IGNORE);
        if (Op == MPI_SUM) {
            for (int i = 0; i < count; i++) {
                root_buff[i] += tmp_buff[i];
            }
        } else if (Op == MPI_PROD) {
            for (int i = 0; i < count; i++) {
                root_buff[i] *= tmp_buff[i];
            }
        } else if (Op == MPI_MAX) {
            for (int i = 0; i < count; i++) {
                if (root_buff[i] < tmp_buff[i]) {
                    root_buff[i] = tmp_buff[i];
                }
            }
        } else if (Op == MPI_MIN) {
            for (int i = 0; i < count; i++) {
                if (root_buff[i] > tmp_buff[i]) {
                    root_buff[i] = tmp_buff[i];
                }
            }
        } else {
            return MPI_ERR_UNSUPPORTED_OPERATION;
        }
    }

    tmp_buff.clear();
    for (int i = 0; i < count; i++) {
        (static_cast<T*>(recvbuf))[i] = root_buff[i];
    }

    root_buff.clear();
    return MPI_SUCCESS;
}

int MPI_allreduce_my(const void* sendbuf, void* recvbuf, int count, MPI_Datatype type, MPI_Op Op, MPI_Comm Comm) {
    int size;
    int rank;
    MPI_Comm_size(Comm, &size);
    MPI_Comm_rank(Comm, &rank);

    int root;
    if (rank == 0) {
        std::mt19937 gen(time(0));
        root = gen() % size;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, Comm);

    int status = MPI_SUCCESS;
    if (rank == root) {
        if (type == MPI_INT) {
            status = buffer_calc<int>(const_cast<void*>(sendbuf), recvbuf, count, type, Op, Comm);
        } else if (type == MPI_FLOAT) {
            status = buffer_calc<float>(const_cast<void*>(sendbuf), recvbuf, count, type, Op, Comm);
        } else if (type == MPI_DOUBLE) {
            status = buffer_calc<double>(const_cast<void*>(sendbuf), recvbuf, count, type, Op, Comm);
        } else {
            status = MPI_ERR_TYPE;
        }
    } else {
        MPI_Send(sendbuf, count, type, root, 1, Comm);
        MPI_Recv(recvbuf, count, type, MPI_ANY_SOURCE, 1, Comm, MPI_STATUS_IGNORE);
        MPI_Recv(&status, 1, MPI_INT, MPI_ANY_SOURCE, 2, Comm, MPI_STATUS_IGNORE);
    }

    int shift = ((size - root) + rank) % size;
    int begin = ceil(logf(shift + 1) / logf(2));
    for (int i = begin; powf(2, i) + shift < size; i++) {
        int send_rank = (static_cast<int>(powf(2, i)) + rank) % size;
        MPI_Send(recvbuf, count, type, send_rank, 1, Comm);
        MPI_Send(&status, 1, MPI_INT, send_rank, 2, Comm);
    }

    return status;
}

template <typename T>
std::vector<T> vec_gen(int size) {
    std::vector<T> vec(size);
    std::mt19937 gen(time(0));
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 100;
    }

    return vec;
}
template
std::vector<int> vec_gen(int size);
template
std::vector<float> vec_gen(int size);
template
std::vector<double> vec_gen(int size);
