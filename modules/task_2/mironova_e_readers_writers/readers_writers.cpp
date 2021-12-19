// Copyright 2021 Mironova Ekaterina
#include "../../../modules/task_2/mironova_e_readers_writers/readers_writers.h"
#include <mpi.h>
#include <string>
#include <random>
#include <algorithm>
#include <chrono> // NOLINT [build/c++11]
#include <thread> // NOLINT [build/c++11]

void read() {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::this_thread::sleep_for(std::chrono::milliseconds(10 + gen() % 10));
}
void wait() {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::this_thread::sleep_for
    (std::chrono::milliseconds(15 + gen() % 10));
}

void write() {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::this_thread::sleep_for(std::chrono::milliseconds(10 + gen() % 10));
}

int wait_for_access(int request_tag, int response_tag) {
    int buff = 0;
    MPI_Status status;
    while (true) {
        MPI_Send(&buff, 1, MPI_INT, 0, request_tag, MPI_COMM_WORLD);
        MPI_Recv(&buff, 1, MPI_INT, 0, response_tag, MPI_COMM_WORLD, &status);
        if (buff != -1) {
            break;
        }
        wait();
    }
    return buff;
}

void release_access(int request_tag, int buff = 0) {
    MPI_Send(&buff, 1, MPI_INT, 0, request_tag, MPI_COMM_WORLD);
}

void reader(int rank) {
    int RC = 0;
    wait();
    wait_for_access(S_REQUEST, S_RESPONSE);
    release_access(S_RELEASE);
    RC = wait_for_access(RC_REQUEST, RC_RESPONSE);
    RC++;
    if (RC == 1) {
        wait_for_access(ACCESS_REQUEST, ACCESS_RESPONSE);
    }
    release_access(RC_RELEASE, RC);
    read();
    RC = wait_for_access(RC_REQUEST, RC_RESPONSE);
    RC--;
    if (RC == 0) {
        release_access(ACCESS_RELEASE);
    }
    release_access(RC_RELEASE, RC);
    release_access(FINISH);
}

void writer(int rank) {
    wait();
    wait_for_access(S_REQUEST, S_RESPONSE);
    wait_for_access(ACCESS_REQUEST, ACCESS_RESPONSE);
    release_access(S_RELEASE);
    write();
    release_access(ACCESS_RELEASE);
    release_access(FINISH);
}

void manager(int rank, int procs) {
    int in_buff = 0, out_buff = 0;
    MPI_Status status;
    bool RC = true;
    bool Access = true;
    int ReadCount = 0;
    bool S = true;
    int active_procs = procs - 1;
    while (true) {
        if (procs != 1) {
            MPI_Recv(&in_buff, 1, MPI_INT, MPI_ANY_SOURCE,
                MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        } else {
            return;
        }
        switch (status.MPI_TAG) {
            case (S_REQUEST):
                if (S == true) {
                    S = false;
                    out_buff = 1;
                    MPI_Send(&out_buff, 1, MPI_INT, status.MPI_SOURCE,
                        S_RESPONSE, MPI_COMM_WORLD);
                } else {
                    out_buff = -1;
                    MPI_Send(&out_buff, 1, MPI_INT, status.MPI_SOURCE,
                        S_RESPONSE, MPI_COMM_WORLD);
                }
                break;
            case (S_RELEASE):
                if (S == false) {
                    S = true;
                }
                break;
            case (RC_REQUEST):
                if (RC == true) {
                    RC = false;
                    MPI_Send(&ReadCount, 1, MPI_INT, status.MPI_SOURCE,
                        RC_RESPONSE, MPI_COMM_WORLD);
                } else {
                    out_buff = -1;
                    MPI_Send(&out_buff, 1, MPI_INT, status.MPI_SOURCE,
                        RC_RESPONSE, MPI_COMM_WORLD);
                }
                break;
            case (RC_RELEASE):
                if (RC == false) {
                    RC = true;
                    ReadCount = in_buff;
                }
                break;
            case (ACCESS_REQUEST):
                if (Access == true) {
                    Access = false;
                    out_buff = 1;
                    MPI_Send(&out_buff, 1, MPI_INT, status.MPI_SOURCE,
                        ACCESS_RESPONSE, MPI_COMM_WORLD);
                } else {
                    out_buff = -1;
                    MPI_Send(&out_buff, 1, MPI_INT, status.MPI_SOURCE,
                        ACCESS_RESPONSE, MPI_COMM_WORLD);
                }
                break;
            case (ACCESS_RELEASE):
                if (Access == false) {
                    Access = true;
                }
                break;
            case (FINISH):
                active_procs--;
                break;
        }
        if (active_procs == 0) {
            break;
        }
    }
    return;
}
