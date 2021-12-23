// Copyright 2021 Petrova Polina

#include <gtest/gtest.h>
#include"./matrix_mult.h"
#include<gtest-mpi-listener.hpp>
#include <iostream>


TEST(mpi_Fox, test_0) {
    int q = 5;
    int p = 5;
    int lda = q * p;
    double* A = new double[lda * lda];
    double* B = new double[lda * lda];
    double* Cseq = new double[lda * lda];
    double* Cpar = new double[lda * lda];

    double* pa = A;
    double* pb = B;
    memset(pa, 0, sizeof(double) * lda * lda);
    memset(pb, 0, sizeof(double) * lda * lda);
    for (int i = 0; i < 5; i++) {
        pa[i * lda + i] = i + 1;
        pb[i * lda + i] = i + 1;
    }
    ASSERT_NO_THROW(Fox_mpi(5, A, lda, B, lda, Cpar, lda));
}
TEST(mpi_Fox, test_1) {
    int q = 5;
    int p = 5;
    int lda = q * p;
    double* A = new double[lda * lda];
    double* B = new double[lda * lda];
    double* Cseq = new double[lda * lda];
    double* Cpar = new double[lda * lda];

    double* pa = A;
    double* pb = B;
    memset(pa, 0, sizeof(double) * lda * lda);
    memset(pb, 0, sizeof(double) * lda * lda);
    for (int i = 0; i < 5; i++) {
        pa[i * lda + i] = i + 1;
        pb[i * lda + i] = i + 1;
    }
    ASSERT_NO_THROW(Fox_sequence(5, q, A, lda, B, lda, Cpar, lda));
}
TEST(mpi_Fox, test_2) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    int q = 50;
    int p = 50;
    int lda = q * p;
    double* A = new double[lda * lda];
    double* B = new double[lda * lda];
    double* Cseq = new double[lda * lda];
    double* Cpar = new double[lda * lda];

    double* pa = A;
    double* pb = B;
    memset(pa, 0, sizeof(double) * lda * lda);
    memset(pb, 0, sizeof(double) * lda * lda);
    Fox_mpi(50, A, lda, B, lda, Cpar, lda);

    if (procRank == 0) {
        Fox_sequence(50, q, A, lda, B, lda, Cseq, lda);
        _daxpy(lda * lda, -1, Cpar, 1, Cseq, 1);
        double e = _dnrm2(lda * lda, Cseq, 1);
        ASSERT_DOUBLE_EQ(e, 0);
    }

    delete[] A;
    delete[] B;
    delete[] Cpar;
    delete[] Cseq;
}
TEST(mpi_Fox, test_3) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    int q = 77;
    int p = 77;
    int lda = q * p;
    double* A = new double[lda * lda];
    double* B = new double[lda * lda];
    double* Cseq = new double[lda * lda];
    double* Cpar = new double[lda * lda];

    double* pa = A;
    double* pb = B;
    memset(pa, 0, sizeof(double) * lda * lda);
    memset(pb, 0, sizeof(double) * lda * lda);
    Fox_mpi(77, A, lda, B, lda, Cpar, lda);

    if (procRank == 0) {
        Fox_sequence(77, q, A, lda, B, lda, Cseq, lda);
        _daxpy(lda * lda, -1, Cpar, 1, Cseq, 1);
        double e = _dnrm2(lda * lda, Cseq, 1);
        ASSERT_DOUBLE_EQ(e, 0);
    }

    delete[] A;
    delete[] B;
    delete[] Cpar;
    delete[] Cseq;
}
TEST(mpi_Fox, test_4) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    int q = 100;
    int p = 100;
    int lda = q * p;
    double* A = new double[lda * lda];
    double* B = new double[lda * lda];
    double* Cseq = new double[lda * lda];
    double* Cpar = new double[lda * lda];

    double* pa = A;
    double* pb = B;
    memset(pa, 0, sizeof(double) * lda * lda);
    memset(pb, 0, sizeof(double) * lda * lda);
    Fox_mpi(100, A, lda, B, lda, Cpar, lda);

    if (procRank == 0) {
        Fox_sequence(100, q, A, lda, B, lda, Cseq, lda);
        _daxpy(lda * lda, -1, Cpar, 1, Cseq, 1);
        double e = _dnrm2(lda * lda, Cseq, 1);
        ASSERT_DOUBLE_EQ(e, 0);
    }

    delete[] A;
    delete[] B;
    delete[] Cpar;
    delete[] Cseq;
}
TEST(mpi_Fox, test_5) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    // double t1, t2, st1, st2;
    int q = 100;
    int p = 100;
    int lda = q * p;
    double* A = new double[lda * lda];
    double* B = new double[lda * lda];
    double* Cseq = new double[lda * lda];
    double* Cpar = new double[lda * lda];

    double* pa = A;
    double* pb = B;
    memset(pa, 0, sizeof(double) * lda * lda);
    memset(pb, 0, sizeof(double) * lda * lda);
    // t1 = MPI_Wtime();
    Fox_mpi(100, A, lda, B, lda, Cpar, lda);
    // t2 = MPI_Wtime();

    if (procRank == 0) {
        // st1 = MPI_Wtime();
        Fox_sequence(100, q, A, lda, B, lda, Cseq, lda);
        // st2 = MPI_Wtime();
        _daxpy(lda * lda, -1, Cpar, 1, Cseq, 1);
        double e = _dnrm2(lda * lda, Cseq, 1);
        // std::cout << "seq time: " << st2 - st1 << std::endl;
        // std::cout << "parallel time: " << t2 - t1 << std::endl;
        ASSERT_DOUBLE_EQ(e, 0);
    }

    delete[] A;
    delete[] B;
    delete[] Cpar;
    delete[] Cseq;
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
