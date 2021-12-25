// Copyright 2021 Troegubova Alexandra
#include <mpi.h>
#include <vector>
#include <random>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cmath>
#include "../../../modules/task_2/troegubova_a_allreduce/allreduce.h"

template<typename T>
std::vector<T> getRandomVec(int size) {
    std::mt19937 gen(std::random_device().operator()());
    std::uniform_int_distribution<> range(-10, 10);
    std::vector<T> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = static_cast<T>(range(gen)) / 2;
    }
    return vec;
}

template
std::vector<int> getRandomVec<int>(int size);

template
std::vector<float> getRandomVec<float>(int size);

template
std::vector<double> getRandomVec<double>(int size);

int ALLreduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype type, MPI_Op op, MPI_Comm comm) {
    MPI_Barrier(MPI_COMM_WORLD);
    int proc_count, proc_rank;
    MPI_Comm_size(comm, &proc_count);
    MPI_Comm_rank(comm, &proc_rank);

    int root;
    if (proc_rank == 0) {
        std::mt19937 gen(std::random_device().operator()());
        std::uniform_int_distribution<> range(0, (proc_count - 1));
        root = range(gen);
        if (root != 0) {
            MPI_Send(&root, 1, MPI_INT, root, proc_rank, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&root, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    double depth_f = log2(static_cast<double>(proc_count));
    int depth = static_cast<int>(depth_f) + ( (static_cast<double>(static_cast<int>(depth_f))) == depth_f ? 0 : 1);
    int dest = 0;
    double n_val = log2(static_cast<double>(proc_rank + 1));
    int nitial_val = static_cast<int>(n_val) + ( (static_cast<double>(static_cast<int>(n_val))) == n_val ? 0 : 1);
    int degree = static_cast<int>(pow(2, nitial_val));
    for (int i = nitial_val; i < depth; i++) {
        dest = degree + proc_rank;
        if (dest >= proc_count)
            break;
        degree *= 2;
        if (dest == root)
            continue;
        MPI_Send(&root, 1, MPI_INT, dest, proc_rank, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (proc_rank == root) {
        if (root)
            nitial_val = 0;

        if (type == MPI_INT) {
            for (int i = 0; i < count; i++)
                reinterpret_cast<int*>(recvbuf)[i] = reinterpret_cast<int*>(sendbuf)[i];
            int *buf_int = new int[count];
            for (int i = 1; i < proc_count; i++) {
                MPI_Recv(buf_int, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, MPI_STATUS_IGNORE);
                for (int k = 0; k < count; k++) {
                    if (op == MPI_MAX) {
                        if (buf_int[k] > reinterpret_cast<int*>(recvbuf)[k])
                            reinterpret_cast<int*>(recvbuf)[k] = buf_int[k];
                        continue;
                    }
                    if (op == MPI_MIN) {
                        if (buf_int[k] < reinterpret_cast<int*>(recvbuf)[k])
                            reinterpret_cast<int*>(recvbuf)[k] = buf_int[k];
                        continue;
                    }
                    if (op == MPI_SUM) {
                        reinterpret_cast<int*>(recvbuf)[k] += buf_int[k];
                        continue;
                    }
                    if (op == MPI_PROD) {
                        reinterpret_cast<int*>(recvbuf)[k] *= buf_int[k];
                        continue;
                    }
                }
            }
            delete[] buf_int;
        } else {
            if (type == MPI_FLOAT) {
                for (int i = 0; i < count; i++)
                    reinterpret_cast<float*>(recvbuf)[i] = reinterpret_cast<float*>(sendbuf)[i];
                float *buf_float = new float[count];
                for (int i = 1; i < proc_count; i++) {
                    MPI_Recv(buf_float, count, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, MPI_STATUS_IGNORE);
                    for (int k = 0; k < count; k++) {
                        if (op == MPI_MAX) {
                            if (buf_float[k] > reinterpret_cast<float*>(recvbuf)[k])
                                reinterpret_cast<float*>(recvbuf)[k] = buf_float[k];
                            continue;
                        }
                        if (op == MPI_MIN) {
                            if (buf_float[k] < reinterpret_cast<float*>(recvbuf)[k])
                                reinterpret_cast<float*>(recvbuf)[k] = buf_float[k];
                            continue;
                        }
                        if (op == MPI_SUM) {
                            reinterpret_cast<float*>(recvbuf)[k] += buf_float[k];
                            continue;
                        }
                        if (op == MPI_PROD) {
                            reinterpret_cast<float*>(recvbuf)[k] *= buf_float[k];
                            continue;
                        }
                    }
                }
                delete[] buf_float;
            } else {
                for (int i = 0; i < count; i++)
                    reinterpret_cast<double*>(recvbuf)[i] = reinterpret_cast<double*>(sendbuf)[i];
                double *buf_double = new double[count];
                for (int i = 1; i < proc_count; i++) {
                    MPI_Recv(buf_double, count, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, MPI_STATUS_IGNORE);
                    for (int k = 0; k < count; k++) {
                        if (op == MPI_MAX) {
                            if (buf_double[k] > reinterpret_cast<double*>(recvbuf)[k])
                                reinterpret_cast<double*>(recvbuf)[k] = buf_double[k];
                            continue;
                        }
                        if (op == MPI_MIN) {
                            if (buf_double[k] < reinterpret_cast<double*>(recvbuf)[k])
                                reinterpret_cast<double*>(recvbuf)[k] = buf_double[k];
                            continue;
                        }
                        if (op == MPI_SUM) {
                            reinterpret_cast<double*>(recvbuf)[k] += buf_double[k];
                            continue;
                        }
                        if (op == MPI_PROD) {
                            reinterpret_cast<double*>(recvbuf)[k] *= buf_double[k];
                            continue;
                        }
                    }
                }
                delete[] buf_double;
            }
        }
    } else {
        if (root > proc_rank) {
            int i = 0;
            while (1) {
                int buf = pow(2, i) - 1;
                if (buf >= proc_count)
                    break;
                if (buf == proc_rank) {
                    nitial_val++;
                    break;
                }
                i++;
            }
        }
        MPI_Send(sendbuf, count, type, root, proc_rank, comm);
        MPI_Recv(recvbuf, count, type, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, MPI_STATUS_IGNORE);
    }

    dest = 0;
    degree = static_cast<int>(pow(2, nitial_val));
    for (int i = nitial_val; i < depth; i++) {
        if (root < degree) {
            dest = degree + proc_rank;
        } else {
            if (proc_rank == root) {
                dest = degree - 1;
            } else {
                dest = degree + proc_rank;
                if (dest >= root)
                    dest++;
            }
        }
        if (proc_count <= dest)
            break;
        MPI_Send(recvbuf, count, type, dest, proc_rank, comm);
        degree *= 2;
    }
    return 0;
}

