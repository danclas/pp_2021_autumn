// Copyright 2021 Cheremushkin Kirill

#include <mpi.h>
#include <string>
#include <iostream>
#include <cstring>
#include <random>
#include "../../../modules/task_1/cheremushkin_k_podshet_slov/count_words.h"
std::string create_sting(int sw) {
    if (sw == 0) {
        return "Battlefield 4 best game in the series";
    }
    if (sw == 1) {
        return "Welcome to Dubai";
    }
    return "error";
}

int counting(std::string S) {  // Linear Counting
    if (S.empty()) {
        return -1;
    }
    int ProcRank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int count = 0;
    if (ProcRank == 0) {
        count = 1;
    }
    for (int i = 0; i < static_cast<int>(S.size()); i++) {
        if (S[i] == ' ')
            count++;
    }
    return count;
}

int parallel_counting(std::string S) {  // Parallel counting
    MPI_Status status;
    int ProcNum = 0, ProcRank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);  // Getting the rank of a process
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);  // Obtaining the number of processes
    int send_count;
    int ssend_count;
    if (ProcRank == 0) {  // if the initial process
        if (S.size() % ProcNum != 0 && ProcNum != 1)
            ssend_count = S.size() / ProcNum + S.size() % ProcNum;
        else
            ssend_count = S.size() / ProcNum;
        send_count = S.size() / ProcNum;
    }
    MPI_Bcast(&send_count, 1, MPI_INT, 0, MPI_COMM_WORLD);  // Sending 0 and receiving
    MPI_Bcast(&ssend_count, 1, MPI_INT, 0, MPI_COMM_WORLD);  //  by others the number of characters per process
    if (send_count == 0)
        return 0;
    if (ssend_count == 0)
        return 0;
    std::string ss;
    if (ProcRank == ProcNum - 1 && ProcNum != 1)
        ss.resize(ssend_count);
    else
        ss.resize(send_count);
    std::string* st = new std::string[ProcNum];
    if (ProcRank == 0) {
        int i;
        int c = 0;
        for (i = 0; i < static_cast<int>(S.size()-(S.size() % ProcNum)); i++) {
            if (i == 0) {
                st[c] += S[i];
                continue;
            }
            if (i % (S.size() / ProcNum) != 0) {  // fff
                st[c] += S[i];
            } else {
                c++;
                st[c] += S[i];
            }
        }
        while (S[i]) {
            st[c] += S[i];
            i++;
        }
    }
    if (ProcRank == 0) {
        for (int i = 1; i < ProcNum; i++) {
            if (i == ProcNum - 1)
                MPI_Send(&(st[i][0]), ssend_count, MPI_CHAR, i, 3, MPI_COMM_WORLD);
            else
                MPI_Send(&(st[i][0]), send_count, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } else {
        if (ProcRank == ProcNum - 1)
            MPI_Recv(&(ss[0]), ssend_count, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        else
            MPI_Recv(&(ss[0]), send_count, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    int count;
    if (ProcRank == 0)
        count = counting(st[0]);
    else
        count = counting(ss);
    int total_count = 0;
    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return total_count;
}
