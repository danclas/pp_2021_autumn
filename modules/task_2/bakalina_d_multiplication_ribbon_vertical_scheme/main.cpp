// Copyright 2021 Bakalina Darya
#include <gtest/gtest.h>
#include "./multiplication_mtrx_by_vec.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, parallel_multiplication_in_matrix_with_size_10_x_10) {
    int ProcRank = 0;
    const int n = 15;
    const int m = 15;
    int** mtrx = 0;
    int* v = 0;
    int* ph_res = 0;
    bool flag = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        mtrx = new int* [m];

        for (int i = 0; i < m; i++) {
            mtrx[i] = new int[n];
            for (int j = 0; j < n; j++) {
                for (int j = 0; j < m; j++) {
                    mtrx[i][j] = 1;
                }
            }
        }
        v = new int[m];
        for (int i = 0; i < m; i++) {
            v[i] = 1;
        }
    }
    ph_res = paral_multiply(mtrx, v, n, m);
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            if (ph_res[i] == 15)
                flag = 1;
            else
                flag = 0;
        }
    }
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            delete[] mtrx[i];
        }
        delete[] mtrx;
        delete[] v;
        ASSERT_TRUE(flag);
    }
}

TEST(Parallel_Operations_MPI, parallel_multiplication_in_matrix_with_size_100_x_100) {
    int ProcRank = 0;
    const int n = 20;
    const int m = 20;
    int** mtrx = 0;
    int* v = 0;
    int* ph_res = 0;
    bool flag = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        mtrx = new int* [m];

        for (int i = 0; i < m; i++) {
            mtrx[i] = new int[n];
            for (int j = 0; j < n; j++) {
                for (int j = 0; j < m; j++) {
                    mtrx[i][j] = 1;
                }
            }
        }
        v = new int[m];
        for (int i = 0; i < m; i++) {
            v[i] = 1;
        }
    }
    ph_res = paral_multiply(mtrx, v, n, m);
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            if (ph_res[i] == 20)
                flag = 1;
            else
                flag = 0;
        }
    }
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            delete[] mtrx[i];
        }
        delete[] mtrx;
        delete[] v;
        ASSERT_TRUE(flag);
    }
}

TEST(Parallel_Operations_MPI, parallel_multiplication_in_matrix_with_size_1000_x_1000) {
    int ProcRank = 0;
    const int n = 25;
    const int m = 25;
    int** mtrx = 0;
    int* v = 0;
    int* ph_res = 0;
    bool flag = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        mtrx = new int* [m];

        for (int i = 0; i < m; i++) {
            mtrx[i] = new int[n];
            for (int j = 0; j < n; j++) {
                for (int j = 0; j < m; j++) {
                    mtrx[i][j] = 1;
                }
            }
        }
        v = new int[m];
        for (int i = 0; i < m; i++) {
            v[i] = 1;
        }
    }
    ph_res = paral_multiply(mtrx, v, n, m);
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            if (ph_res[i] == 25)
                flag = 1;
            else
                flag = 0;
        }
    }
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            delete[] mtrx[i];
        }
        delete[] mtrx;
        delete[] v;
        ASSERT_TRUE(flag);
    }
}

TEST(Parallel_Operations_MPI, parallel_multiplication_in_random_matrix_with_small_size) {
    int ProcRank = 0;
    const int n = 15;
    const int m = 15;
    int** mtrx = 0;
    int* v = 0;
    int* ph_res = 0, * lin_res = 0;
    bool flag = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        mtrx = create_random_matrix(n, m);
        v = create_random_vector(m);
        lin_res = lin_multiply(mtrx, v, n, m);
    }
    ph_res = paral_multiply(mtrx, v, n, m);
    if (ProcRank == 0) {
        flag = check_equality(ph_res, lin_res, n);
    }
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            delete[] mtrx[i];
        }
        delete[] mtrx;
        delete[] v;
        ASSERT_EQ(flag, 1);
    }
}

TEST(Parallel_Operations_MPI, parallel_multiplication_in_random_matrix_with_large_size) {
    int ProcRank = 0;
    const int n = 25;
    const int m = 25;
    int** mtrx = 0;
    int* v = 0;
    int* ph_res = 0, * lin_res = 0;
    bool flag = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        mtrx = create_random_matrix(n, m);
        v = create_random_vector(m);
        lin_res = lin_multiply(mtrx, v, n, m);
    }
    ph_res = paral_multiply(mtrx, v, n, m);
    if (ProcRank == 0) {
        flag = check_equality(ph_res, lin_res, n);
    }
    if (ProcRank == 0) {
        for (int i = 0; i < n; i++) {
            delete[] mtrx[i];
        }
        delete[] mtrx;
        delete[] v;
        ASSERT_EQ(flag, 1);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
