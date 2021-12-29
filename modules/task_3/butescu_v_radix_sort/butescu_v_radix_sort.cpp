// Copyright 2021 Butescu Vladimir
#include <mpi.h>
#include <memory>
#include <random>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits>
#include <utility>
#include "../../../modules/task_3/butescu_v_radix_sort.h"

std::vector<double> Get_Random_Vector(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.99, 100.99);

    if (size < 0) { size = 0; }
    std::vector<double> vec(size);
    for (int i = 0; i < size; i++) { vec[i] = dist(gen); }
    return vec;
}

std::vector<double> mergeVectors(std::vector<double> vec1, std::vector<double> vec2) {
    // Note: vec1.size() > 0 && vec2.size > 0
    std::vector<double> res(vec1.size() + vec2.size());
    int i = 0;  // index for vec1
    int j = 0;  // index for vec2

    for (int r = 0; r < static_cast<int>(res.size()); r++) {
        if (i > static_cast<int>(vec1.size()) - 1) {
            res[r] = vec2[j++];
        } else {
            if (j > static_cast<int>(vec2.size()) - 1) {
                res[r] = vec1[i++];
            } else {
                if (vec1[i] < vec2[j]) {
                    res[r] = vec1[i++];
                } else {
                    res[r] = vec2[j++];
                }
            }
        }
    }
    return res;
}


std::vector<double> sortingByCounting(std::vector<double> vec1,
    std::vector<double> vec2, int byteNum) {
    if (vec1.size() != vec2.size()) {
        throw - 1;
    }

    int size = vec1.size();

    // byte reading
    unsigned char* byteArr = (unsigned char*)vec1.data();

    int iOffset = 0;  // offset for indices
    int counter[256] = { 0 };

    for (int i = 0; i < size; ++i) {
        counter[byteArr[8 * i + byteNum]]++;
    }

    int j = 0;
    while (j < 256) {
        if (counter[j] != 0)
            break;
        ++j;
    }

    iOffset = counter[j];
    counter[j] = 0;
    ++j;

    // calculate the correct indices
    while (j < 256) {
        int tmp = counter[j];
        counter[j] = iOffset;
        iOffset += tmp;
        ++j;
    }

    // sort by byteNum
    for (int i = 0; i < size; ++i) {
        vec2[counter[byteArr[8 * i + byteNum]]] = vec1[i];
        counter[byteArr[8 * i + byteNum]]++;
    }

    return vec2;
}

std::vector<double> radixSort(std::vector<double> vec) {
    std::vector<double> tmp(vec.size());

    tmp = sortingByCounting(vec, tmp, 0);
    vec = sortingByCounting(tmp, vec, 1);
    tmp = sortingByCounting(vec, tmp, 2);
    vec = sortingByCounting(tmp, vec, 3);
    tmp = sortingByCounting(vec, tmp, 4);
    vec = sortingByCounting(tmp, vec, 5);
    tmp = sortingByCounting(vec, tmp, 6);
    vec = sortingByCounting(tmp, vec, 7);

    return vec;
}

std::vector<double> Parallel_Radix_Sort(const std::vector<double>& vec) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<double> result;

    int size = vec.size();
    std::vector<int> sendcounts(ProcNum);
    std::vector<int> displs(ProcNum);

    int count;

    if ((ProcRank < size % ProcNum))
        count = (size / ProcNum) + 1;
    else
        count = size / ProcNum;

    std::vector<double> recvbuf(count);

    if (recvbuf.size() == 0)
        recvbuf.resize(1);

    displs[0] = 0;

    for (int i = 0; i < ProcNum; i++) {
        if (i < (size % ProcNum))
            sendcounts[i] = (size / ProcNum) + 1;
        else
            sendcounts[i] = size / ProcNum;
        if (i > 0)
            displs[i] = (displs[i - 1] + sendcounts[i - 1]);
    }

    MPI_Scatterv(vec.data(), sendcounts.data(), displs.data(),
        MPI_DOUBLE, recvbuf.data(), count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    recvbuf = radixSort(recvbuf);

    if (ProcRank != 0) {
        MPI_Send(recvbuf.data(), count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        result = recvbuf;
        MPI_Status status;
        for (int i = 1; i < ProcNum; i++) {
            recvbuf.resize(sendcounts[i]);
            MPI_Recv(recvbuf.data(), sendcounts[i], MPI_DOUBLE,
                i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            result = mergeVectors(result, recvbuf);
        }
    }
    return result;
}
