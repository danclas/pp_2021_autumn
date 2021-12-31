// Copyright 2021 Zotov Maksim
#include <gtest/gtest.h>
#include "./integration_Monte_Carlo_method.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_MPI, Test_func_1) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar, resSeq;
    double a = 0, b = 2;
    int n = 100000;
    resPar = integralParallel(a, b, n, f1);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, n, f1);
        EXPECT_NEAR(resSeq, resPar, 0.5);
    }
}


TEST(Parallel_MPI, Test_funct_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar, resSeq;
    double a = 2, b = 12;
    int n = 100000;
    resPar = integralParallel(a, b, n, f2);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, n, f2);
        EXPECT_NEAR(resSeq, resPar, 0.5);
    }
}

TEST(Parallel_MPI, Test_funct_3) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar, resSeq;
    double a = 0, b = 3;
    int n = 100000;
    resPar = integralParallel(a, b, n, f3);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, n, f3);
        EXPECT_NEAR(resSeq, resPar, 0.5);
    }
}

TEST(Parallel_MPI, Test_funct_4) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar, resSeq;
    double a = 0, b = 20;
    int n = 100000;
    resPar = integralParallel(a, b, n, f4);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, n, f4);
        EXPECT_NEAR(resSeq, resPar, 0.5);
    }
}

TEST(Parallel_MPI, Test_funct_5) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double resPar, resSeq;
    double a = 0, b = 6;
    int n = 100000;
    resPar = integralParallel(a, b, n, f5);
    if (ProcRank == 0) {
        resSeq = integralMonteCarlo(a, b, n, f5);
        EXPECT_NEAR(resSeq, resPar, 0.5);
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

    return 0;
}
