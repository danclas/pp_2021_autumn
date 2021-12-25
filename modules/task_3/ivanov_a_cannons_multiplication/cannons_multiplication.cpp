// Copyright 2021 Ivanov Arkadiy
#include <random>
#include "../../../modules/task_3/ivanov_a_cannons_multiplication/cannons_multiplication.h"

// can generate in range [-maxValue, -minValue] && [minValue, maxValue] IN: minValue, maxValue > 0
int generateRngValue(int minValue, int maxValue, int* feedback) {
    (void)feedback;  // to supress potential warnings
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>
        dist(static_cast<unsigned int>(minValue), static_cast<unsigned int>(maxValue));
    return static_cast<int>(dist(rng)) * ((static_cast<int>(dist(rng)) & 0x01) ? -1 : 1);
}

// generates: {minValue + feedback, minValue + feedback + 1, minValue + feedback + 2, ...}
int generateRisingValue(int minValue, int empty, int* feedback) {
    (void)empty;
    return minValue + ((*feedback)++);
}

// generates: {maxValue - feedback, maxValue - feedback - 1, maxValue - feedback - 2, ...}
int generateFallingValue(int empty, int maxValue, int* feedback) {
    (void)empty;
    return maxValue - ((*feedback)++);
}

// generates v1, v2, v1, v2, v1, v2, v1, v2, ... with rule feedback % 2 == 0 -> v1 else v2
int generateJumpingValue(int v1, int v2, int* feedback) {
    return ((*feedback)++ & 0x01) ? v2 : v1;
}

bool isMultAcceptable(int* ms) {
    // rule 1: A[NxN] && B[NxN] && N!=0
    if (ms[0] != ms[1] || ms[0] != ms[2] || ms[0] != ms[3])
        return false;
    if (ms[1] != ms[2] || ms[1] != ms[3])
        return false;
    if (ms[2] != ms[3])
        return false;
    if (ms[0] == 0)
        return false;
    return true;
}

int nearestSqrt(int procCount) {
    // find nearest sqrt to procCount
    int sqr = 1;
    while (sqr * sqr <= procCount)
        sqr++;
    sqr--;
    return sqr;
}

matrix<double> cannonsMultiplication(matrix<double>* A, matrix<double>* B) {
    matrix<double> ans;
    // get MPI info
    int procRank = 0, procCount = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    // if procCount == 1
    if (procCount == 1)
        return (*A) * (*B);

    // sending matrix sizes
    int matSizes[4] = { 0, 0, 0, 0 };
    if (procRank == 0) {
        matSizes[0] = A->getNumRows();
        matSizes[1] = A->getNumColums();
        matSizes[2] = B->getNumRows();
        matSizes[3] = B->getNumColums();
    }
    MPI_Bcast(reinterpret_cast<void*>(matSizes), 4, MPI_INT, 0, MPI_COMM_WORLD);

    // check if parallel multiplication is acceptable A[nxn] B[nxn] n>0
    if (!isMultAcceptable(matSizes))
        return ans;

    // !!!!
    int sqrtProcCount = nearestSqrt(procCount);
    // rule that n % sqr must be == 0
    // so blocks could have the same size
    while (matSizes[0] % sqrtProcCount != 0)
        sqrtProcCount--;
    procCount = sqrtProcCount * sqrtProcCount;
    if (procRank >= procCount)
        return ans;
    if (procCount == 1)
        return (*A) * (*B);
    // !!!!

    // creating communicator with Cartesian(Decartova) topology
    MPI_Comm MPI_COMM_CART;
    int dims[2];
    dims[0] = dims[1] = sqrtProcCount;
    int periods[2] = { 1, 1 };
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &MPI_COMM_CART);

    // preparing space for submatrices of matrices A, B and C
    if (matSizes[0] % sqrtProcCount != 0)
        return ans;
    int blockSideLen = matSizes[0] / sqrtProcCount;
    matrix<double> a(blockSideLen, blockSideLen);
    matrix<double> b(blockSideLen, blockSideLen);
    matrix<double> c(blockSideLen, blockSideLen);
    // std::cout << "Prepared space on rank " << procRank << std::endl;

    // root process sends blocks for other processes
    if (procRank == 0) {
        int baseProcCoords[2];
        int coordsOfMatrA[2];
        int coordOfMatrB[2];
        for (int i = 1; i < procCount; i++) {
            MPI_Cart_coords(MPI_COMM_CART, i, 2, baseProcCoords);
            // left shift of blocks of matrix A
            coordsOfMatrA[0] = baseProcCoords[0];
            coordsOfMatrA[1] = (baseProcCoords[0] + baseProcCoords[1]) % sqrtProcCount;
            // up shift of block of matrix B
            coordOfMatrB[0] = (baseProcCoords[1] + baseProcCoords[0]) % sqrtProcCount;
            coordOfMatrB[1] = baseProcCoords[1];
            // copying blocks to send
            for (int j = 0; j < blockSideLen; j++) {
                for (int k = 0; k < blockSideLen; k++) {
                    a[j][k] =
                        (*A)[coordsOfMatrA[0] * blockSideLen + j][coordsOfMatrA[1] * blockSideLen + k];
                    b[j][k] =
                        (*B)[coordOfMatrB[0] * blockSideLen + j][coordOfMatrB[1] * blockSideLen + k];
                }
            }
            // sending blocks
            MPI_Send(a.data(), blockSideLen * blockSideLen, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            MPI_Send(b.data(), blockSideLen * blockSideLen, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        }
        // setting blocks for root proc
        for (int i = 0; i < blockSideLen; i++) {
            for (int j = 0; j < blockSideLen; j++) {
                a[i][j] = (*A)[i][j];
                b[i][j] = (*B)[i][j];
            }
        }
    } else {
        MPI_Status status;
        MPI_Recv(a.data(), blockSideLen * blockSideLen,
            MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(b.data(), blockSideLen * blockSideLen,
            MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    // std::cout << "Received all data on rank = " << procRank << std::endl;

    // main computation cycle of sqrtProcCount iterations
    for (int i = 0; i < sqrtProcCount; i++) {
        // first operation c = c + a * b
        c = c + (a * b);

        // second operation for each row of A shift left 1 time
        // and for each column of B shift up 1 time
        int left, right, up, down;
        // identify neigbours of process
        MPI_Cart_shift(MPI_COMM_CART, 1, 1, &left, &right);
        MPI_Cart_shift(MPI_COMM_CART, 0, 1, &up, &down);
        // std::cout << "Shift complete on proc = " << procRank << " " <<
        //    left << " " << right << " " << up << " " << down << std::endl;

        // make shift
        MPI_Status status;
        MPI_Sendrecv_replace(a.data(), blockSideLen * blockSideLen, MPI_DOUBLE,
            left, 1, right, 1, MPI_COMM_CART, &status);
        MPI_Sendrecv_replace(b.data(), blockSideLen * blockSideLen, MPI_DOUBLE,
            up, 1, down, 1, MPI_COMM_CART, &status);
        /*
        MPI_Send(a.data(), blockSideLen * blockSideLen, MPI_DOUBLE, left, 1, MPI_COMM_CART);
        MPI_Recv(a.data(), blockSideLen * blockSideLen, MPI_DOUBLE, right, MPI_ANY_TAG, MPI_COMM_CART, &status);
        MPI_Send(b.data(), blockSideLen * blockSideLen, MPI_DOUBLE, up, 1, MPI_COMM_CART);
        MPI_Recv(b.data(), blockSideLen * blockSideLen, MPI_DOUBLE, down, MPI_ANY_TAG, MPI_COMM_CART, &status);
        */
    }

    // gathering data from processes
    if (procRank == 0) {
        ans.prepareSpace(matSizes[0], matSizes[0]);

        // copy root's calculated data
        for (int i = 0; i < blockSideLen; i++)
            for (int j = 0; j < blockSideLen; j++)
                ans[i][j] = c[i][j];

        // copy !root's calculated data
        MPI_Status status;
        int recProcCoords[2];
        for (int i = 1; i < procCount; i++) {
            MPI_Recv(c.data(), blockSideLen * blockSideLen, MPI_DOUBLE, MPI_ANY_SOURCE,
                MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Cart_coords(MPI_COMM_CART, status.MPI_TAG, 2, recProcCoords);

            for (int j = 0; j < blockSideLen; j++) {
                for (int k = 0; k < blockSideLen; k++) {
                    ans[recProcCoords[0] * blockSideLen + j]
                        [recProcCoords[1] * blockSideLen + k] = c[j][k];
                }
            }
        }
    } else {
        MPI_Send(c.data(), blockSideLen * blockSideLen, MPI_DOUBLE, 0, procRank, MPI_COMM_WORLD);
    }
    // std::cout << "Data gathered on rank = " << procRank << std::endl;
    return ans;
}
