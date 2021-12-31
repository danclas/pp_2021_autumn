// Copyright 2021 Bakalina Darya
#include <mpi.h>
#include <iostream>
#include <random>
#include "../../../modules/task_2/bakalina_d_multiplication_ribbon_vertical_scheme/multiplication_mtrx_by_vec.h"

int check_equality(int* v1, int* v2, int size_n) {
    int flag = 1;
    for (int i = 0; i < size_n; i++) {
        if (v1[i] == v2[i])
            flag = 1;
        else
            flag = 0;
    }
    return flag;
}

int* lin_multiply(int** mtrx, int* v, int size_n, int size_m) {
    int* res_vctr = 0;
    res_vctr = new int[size_m];
    for (int i = 0; i < size_m; i++) {
        res_vctr[i] = 0;
    }
    for (int i = 0; i < size_m; i++) {
        for (int j = 0; j < size_n; j++) {
            res_vctr[i] += mtrx[i][j] * v[j];
        }
    }
    return res_vctr;
}

int* multiplication(int* mtrx, int* v, int size_n, int ProcSndNumbr, int SndNumrVctr) {
    int* multyp_res = 0;
    int* ProcResNum = 0;
    int id = 0;
    ProcResNum = new int[ProcSndNumbr];
    multyp_res = new int[size_n];
    for (int i = 0; i < size_n; i++) {
        multyp_res[i] = 0;
    }
    for (int i = 0; i < SndNumrVctr; i++) {
        for (int j = 0; j < size_n; j++) {
            ProcResNum[id] = mtrx[id] * v[i];
            id++;
        }
    }
    if (ProcSndNumbr > size_n) {
        for (int i = 0; i < size_n; i++) {
            for (int j = 0; j < SndNumrVctr; j++) {
                multyp_res[i] += ProcResNum[i + j * size_n];
            }
        }
        return multyp_res;
    }
    return ProcResNum;
}

int** create_random_matrix(const int size_n, const int size_m) {
    int** mtrx = 0;
    mtrx = new int* [size_m];
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < size_m; i++) {
        mtrx[i] = new int[size_n];
        for (int j = 0; j < size_n; j++) {
            for (int j = 0; j < size_m; j++) {
                mtrx[i][j] = gen() % 10;
            }
        }
    }
    return mtrx;
}

int* create_random_vector(const int len) {
    std::random_device dev;
    std::mt19937 gen(dev());
    int* v = 0;
    v = new int[len];
    for (int i = 0; i < len; i++) {
        v[i] = gen() % 10;
    }
    return v;
}

int* ribbon_partition(int** mtrx, int size_n, int size_m) {
    int* mtrx_v = 0;
    mtrx_v = new int[(size_m * size_n)];
    for (int i = 0; i < (size_m * size_n); i++) {
        mtrx_v[i] = 0;
    }
    for (int i = 0; i < size_n; i++) {
        for (int j = 0; j < size_m; j++) {
            mtrx_v[i * size_m + j] = mtrx[j][i];
        }
    }
    return mtrx_v;
}

int* paral_multiply(int** mtrx, int* v, int size_n, int size_m) {
    int ProcNum = 0, ProcRank = 0;
    int* mtrx_v = 0;
    int* res = 0, * multy_res = 0;
    int* rcvbufF_mtrx = 0, * rcvbufF_vctr = 0;
    int* assmbl_buff = 0;
    int* ProcSndNumbr = 0;
    int* ProcSndIndx = 0;
    int* SndNumrVctr = 0;
    int* SndIndxVctr = 0;
    int quotient = 0, resd = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        mtrx_v = ribbon_partition(mtrx, size_n, size_m);
        assmbl_buff = new int[size_n * ProcNum];
        for (int i = 0; i < size_n * ProcNum; i++) {
            assmbl_buff[i] = 0;
        }
        multy_res = new int[size_n];
        for (int i = 0; i < size_n; i++) {
            multy_res[i] = 0;
        }
    }
    MPI_Bcast(&size_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&size_m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    quotient = size_m / ProcNum;
    resd = size_m % ProcNum;
    res = new int[size_n];
    for (int i = 0; i < size_n; i++) {
        res[i] = 0;
    }
    ProcSndNumbr = new int[ProcNum];
    ProcSndIndx = new int[ProcNum];
    SndNumrVctr = new int[ProcNum];
    SndIndxVctr = new int[ProcNum];
    for (int i = 0; i < resd; i++) {
        ProcSndNumbr[i] = size_n * (quotient + 1);
        ProcSndIndx[i] = size_n * i * (quotient + 1);
        SndNumrVctr[i] = quotient + 1;
        SndIndxVctr[i] = i * (quotient + 1);
    }
    for (int i = resd; i < ProcNum; i++) {
        ProcSndNumbr[i] = size_n * quotient;
        ProcSndIndx[i] = size_n * (resd + i * quotient);
        SndNumrVctr[i] = quotient;
        SndIndxVctr[i] = resd + i * quotient;
    }
    rcvbufF_mtrx = new int[ProcSndNumbr[ProcRank]];
    rcvbufF_vctr = new int[SndNumrVctr[ProcRank]];
    MPI_Scatterv(v, SndNumrVctr, SndIndxVctr, MPI_INT, rcvbufF_vctr,
        SndNumrVctr[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(mtrx_v, ProcSndNumbr, ProcSndIndx, MPI_INT, rcvbufF_mtrx,
        ProcSndNumbr[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
    res = multiplication(rcvbufF_mtrx, rcvbufF_vctr, size_n, ProcSndNumbr[ProcRank], SndNumrVctr[ProcRank]);
    MPI_Gather(res, size_n, MPI_INT, assmbl_buff, size_n, MPI_INT, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) {
        for (int i = 0; i < size_n; i++) {
            for (int j = 0; j < ProcNum; j++) {
                multy_res[i] += assmbl_buff[i + j * size_n];
            }
        }
    }
    delete res;
    delete ProcSndNumbr;
    delete ProcSndIndx;
    delete rcvbufF_vctr;
    delete assmbl_buff;
    delete SndNumrVctr;
    delete SndIndxVctr;
    delete rcvbufF_mtrx;
    return multy_res;
}
