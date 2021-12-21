// Copyright 2021 Trevogin Kirill
#include "../../../modules/task_3/trevogin_k_canon/canon.h"
// can generate in range [-maxValue, -minValue] && [minValue, maxValue] IN: minValue, maxValue > 0
int generateRngValue(int minValue, int maxValue, int* feedback)
{
    (void)feedback; // to supress potential warnings
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>
        dist(static_cast<unsigned int>(minValue), static_cast<unsigned int>(maxValue));
    return static_cast<int>(dist(rng)) * ((static_cast<int>(dist(rng)) & 0x01) ? -1 : 1);
}

// generates: {minValue + feedback, minValue + feedback + 1, minValue + feedback + 2, ...}
int generateRisingValue(int minValue, int empty, int* feedback)
{
    (void)empty;
    return minValue + ((*feedback)++);
}

// generates: {maxValue - feedback, maxValue - feedback - 1, maxValue - feedback - 2, ...}
int generateFallingValue(int empty, int maxValue, int* feedback)
{
    (void)empty;
    return maxValue - ((*feedback)++);
}

// generates v1, v2, v1, v2, v1, v2, v1, v2, ... with rule feedback % 2 == 0 -> v1 else v2
int generateJumpingValue(int v1, int v2, int* feedback)
{
    return ((*feedback)++ & 0x01) ? v2 : v1;
}

bool isMultAcceptable(int* mat)
{
    // rule 1: A[NxN] && B[NxN] && N!=0
    if (mat[0] != mat[1] || mat[0] != mat[2] || mat[0] != mat[3])
        return false;
    if (mat[1] != mat[2] || mat[1] != mat[3])
        return false;
    if (mat[2] != mat[3])
        return false;
    if (mat[0] == 0)
        return false;
    return true;
}

int nearestSqrt(int procCount)
{
    // find nearest sqrt to procCount
    int sqr = 1;
    while (sqr * sqr <= procCount)
        sqr++;
    sqr--;
    return sqr;
}

matrix<double> cannonsMultiplication(matrix<double>* A, matrix<double>* B)
{
    matrix<double> mat_l;
    // get MPI info
    int rank = 0, size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // if size == 1
    if (size == 1)
        return (*A) * (*B);

    // sending matrix sizes
    int matSizes[4] = { 0, 0, 0, 0 };
    if (rank == 0) {
        matSizes[0] = A->getNumRows();
        matSizes[1] = A->getNumColums();
        matSizes[2] = B->getNumRows();
        matSizes[3] = B->getNumColums();
    }
    MPI_Bcast(reinterpret_cast<void*>(matSizes), 4, MPI_INT, 0, MPI_COMM_WORLD);

    // check if parallel multiplication is acceptable A[nxn] B[nxn] n>0
    if (!isMultAcceptable(matSizes))
        return mat_l;

    int sqrt_size = nearestSqrt(size);
    // rule that n % sqr must be == 0
    // so blocks could have the same size
    while (matSizes[0] % sqrt_size != 0)
        sqrt_size--;
    size = sqrt_size * sqrt_size;
    if (rank >= size)
        return mat_l;
    if (size == 1)
        return (*A) * (*B);

    // creating communicator with Cartesian(Decartova) topology
    MPI_Comm MPI_COMM_CART;
    int dims[2];
    dims[0] = dims[1] = sqrt_size;
    int periods[2] = { 1, 1 };
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &MPI_COMM_CART);

    // preparing space for submatrices of matrices A, B and C
    if (matSizes[0] % sqrt_size != 0)
        return mat_l;
    int block_side_len = matSizes[0] / sqrt_size;
    matrix<double> a(block_side_len, block_side_len);
    matrix<double> b(block_side_len, block_side_len);
    matrix<double> c(block_side_len, block_side_len);

    // root process sends blocks for other processes
    if (rank == 0) {
        int base_proc[2];
        int OfMatrA[2];
        int OfMatrB[2];
        for (int i = 1; i < size; i++) {
            MPI_Cart_coords(MPI_COMM_CART, i, 2, base_proc);
            // left shift of blocks of matrix A
            OfMatrA[0] = base_proc[0];
            OfMatrA[1] = (base_proc[0] + base_proc[1]) % sqrt_size;
            // up shift of block of matrix B
            OfMatrB[0] = (base_proc[1] + base_proc[0]) % sqrt_size;
            OfMatrB[1] = base_proc[1];
            // copying blocks to send
            for (int j = 0; j < block_side_len; j++) {
                for (int k = 0; k < block_side_len; k++) {
                    a[j][k] = (*A)[OfMatrA[0] * block_side_len + j][OfMatrA[1] * block_side_len + k];
                    b[j][k] = (*B)[OfMatrB[0] * block_side_len + j][OfMatrB[1] * block_side_len + k];
                }
            }
            // sending blocks
            MPI_Send(a.data(), block_side_len * block_side_len, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            MPI_Send(b.data(), block_side_len * block_side_len, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        }
        // setting blocks for root proc
        for (int i = 0; i < block_side_len; i++) {
            for (int j = 0; j < block_side_len; j++) {
                a[i][j] = (*A)[i][j];
                b[i][j] = (*B)[i][j];
            }
        }
    } else {
        MPI_Status status;
        MPI_Recv(a.data(), block_side_len * block_side_len,
            MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(b.data(), block_side_len * block_side_len,
            MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    // main computation cycle of sqrtProcCount iterations
    for (int i = 0; i < sqrt_size; i++) {
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
        MPI_Sendrecv_replace(a.data(), block_side_len * block_side_len, MPI_DOUBLE,
            left, 1, right, 1, MPI_COMM_CART, &status);
        MPI_Sendrecv_replace(b.data(), block_side_len * block_side_len, MPI_DOUBLE,
            up, 1, down, 1, MPI_COMM_CART, &status);
       
    }

    // gathering data from processes
    if (rank == 0) {
        mat_l.prepareSpace(matSizes[0], matSizes[0]);

        // copy root's calculated data
        for (int i = 0; i < block_side_len; i++)
            for (int j = 0; j < block_side_len; j++)
                mat_l[i][j] = c[i][j];

        // copy !root's calculated data
        MPI_Status status;
        int recProcCoords[2];
        for (int i = 1; i < size; i++) {
            MPI_Recv(c.data(), block_side_len * block_side_len, MPI_DOUBLE, MPI_ANY_SOURCE,
                MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Cart_coords(MPI_COMM_CART, status.MPI_TAG, 2, recProcCoords);

            for (int j = 0; j < block_side_len; j++) {
                for (int k = 0; k < block_side_len; k++) {
                    mat_l[recProcCoords[0] * block_side_len + j]
                       [recProcCoords[1] * block_side_len + k]
                        = c[j][k];
                }
            }
        }
    } else {
        MPI_Send(c.data(), block_side_len * block_side_len, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
    }
    return mat_l;
}