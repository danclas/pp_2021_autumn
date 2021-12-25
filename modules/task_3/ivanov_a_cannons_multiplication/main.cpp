// Copyright 2021 Ivanov Arkadiy
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./cannons_multiplication.h"
#include <gtest-mpi-listener.hpp>

#define USE_EFFECIENCY_TESTS 0

TEST(matrix_test, operator_plus_test) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        const int n = 5;
        matrix<double> a(n, n), b(n, n);
        a.fillMatrix(generateRisingValue, 0, 0, 0);
        b.fillMatrix(generateFallingValue, 0, 0, 0);
        matrix<double> c = a + b;
        bool ok = true;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (c[i][j] != 0)
                    ok = false;
        ASSERT_TRUE(ok);
    }
}

TEST(cannons_mult_test, mult_test_1) {
    const int n = 10;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> ans = cannonsMultiplication(&a, &b);
    if (procRank == 0) {
        ASSERT_TRUE(ans == a * b);
    }
}

TEST(cannons_mult_test, mult_test_2) {
    const int n = 4;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> ans = cannonsMultiplication(&a, &b);
    if (procRank == 0) {
        ASSERT_TRUE(ans == a * b);
    }
}

TEST(cannons_mult_test, mult_test_3) {
    const int n = 40;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> ans = cannonsMultiplication(&a, &b);
    if (procRank == 0) {
        ASSERT_TRUE(ans == a * b);
    }
}

TEST(cannons_mult_test, mult_test_4) {
    const int n = 20;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> ans = cannonsMultiplication(&a, &b);
    if (procRank == 0) {
        ASSERT_TRUE(ans == a * b);
    }
}

TEST(cannons_mult_test, mult_test_5) {
    const int n = 50;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> ans = cannonsMultiplication(&a, &b);
    if (procRank == 0) {
        ASSERT_TRUE(ans == a * b);
    }
}

#if USE_EFFECIENCY_TESTS == 1

TEST(cannons_efficiency_test, test_1) {
    const int n = 500;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    double parBef = MPI_Wtime();
    matrix<double> ans = cannonsMultiplication(&a, &b);
    double parAft = MPI_Wtime();
    if (procRank == 0) {
        std::cout << "N = " << n << std::endl;
        std::cout << "Parallel multiplication took " << (parAft - parBef) << " seconds\n";
        double seqBef = MPI_Wtime();
        matrix<double> compare = a * b;
        double seqAft = MPI_Wtime();
        std::cout << "Sequential multiplication took " << (seqAft - seqBef) << " seconds\n";
        std::cout << "Tseq / Tpar = " << (seqAft - seqBef) / (parAft - parBef) << std::endl;
        ASSERT_TRUE(ans == compare);
    }
}

TEST(cannons_efficiency_test, test_2) {
    const int n = 1000;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(n, n);
        b.prepareSpace(n, n);
        a.fillMatrix(generateRngValue, 0, 15, 0);
        b.fillMatrix(generateRngValue, 0, 15, 0);
    }
    double parBef = MPI_Wtime();
    matrix<double> ans = cannonsMultiplication(&a, &b);
    double parAft = MPI_Wtime();
    if (procRank == 0) {
        std::cout << "N = " << n << std::endl;
        std::cout << "Parallel multiplication took " << (parAft - parBef) << " seconds\n";
        double seqBef = MPI_Wtime();
        matrix<double> compare = a * b;
        double seqAft = MPI_Wtime();
        std::cout << "Sequential multiplication took " << (seqAft - seqBef) << " seconds\n";
        std::cout << "Tseq / Tpar = " << (seqAft - seqBef) / (parAft - parBef) << std::endl;
        ASSERT_TRUE(ans == compare);
    }
}

#endif

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);  // check for MPI_SUCCESS?

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);  // just comment
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
