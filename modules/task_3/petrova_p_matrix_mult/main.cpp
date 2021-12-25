// Copyright 2021 Petrova Polina

#include <gtest/gtest.h>
#include <iostream>
#include"./matrix_mult.h"
#include<gtest-mpi-listener.hpp>

TEST(mpi_Fox, can_create_random_vector_0) {
    EXPECT_NO_THROW(std::vector<double> a = fillRandomVector(100));
}
TEST(mpi_Fox, can_create_random_matrix_0) {
    EXPECT_NO_THROW(std::vector< std::vector<double> > a = fillRandomMatrix(100));
}
TEST(mpi_Fox, test_0) {
    int q = 5;
    int p = 5;
    int lda = q * p;
    double* A = new double[lda * lda];
    double* B = new double[lda * lda];
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
        // ASSERT_NEAR(eps, 0, 0.0001);
    }

    delete[] A;
    delete[] B;
    delete[] Cpar;
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
