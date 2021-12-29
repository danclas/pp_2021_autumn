// Copyright 2021 ghanga moukela brexe juniors
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "random"
#include "./operator_sobel.h"
#include <gtest-mpi-listener.hpp>

TEST(test_sobel, test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 3;
    int cols = 3;
    std::vector<int> test(rows * cols);
    test[0] = 10;
    test[1] = 12;
    test[2] = 45;
    test[3] = 123;
    test[4] = 67;
    test[5] = 240;
    test[6] = 11;
    test[7] = 75;
    test[8] = 49;
    std::vector<int> paral(rows * cols);
    paral = parallel_sobel(test, rows, cols);
    if (rank == 0) {
        std::vector<int> seq(rows * cols);
        seq = calc_sobel(test, rows, cols);
        ASSERT_EQ(seq, paral);
    }
}

TEST(test_sobel, test2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 3;
    int cols = 4;
    std::vector<int> test(rows * cols);
    test[0] = 153;
    test[1] = 20;
    test[2] = 148;
    test[3] = 36;
    test[4] = 58;
    test[5] = 192;
    test[6] = 231;
    test[7] = 200;
    test[8] = 17;
    test[9] = 55;
    test[10] = 149;
    test[11] = 13;
    std::vector<int> paral(rows * cols);
    paral = parallel_sobel(test, rows, cols);
    if (rank == 0) {
        std::vector<int> seq(rows * cols);
        seq = calc_sobel(test, rows, cols);
        ASSERT_EQ(seq, paral);
    }
}
TEST(test_sobel, test3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 5;
    int cols = 4;
    std::vector<int> test(rows * cols);
    test[0] = 57;
    test[1] = 22;
    test[2] = 5;
    test[3] = 69;
    test[4] = 173;
    test[5] = 206;
    test[6] = 95;
    test[7] = 11;
    test[8] = 40;
    test[9] = 72;
    test[10] = 155;
    test[11] = 131;
    test[12] = 80;
    test[13] = 14;
    test[14] = 215;
    test[15] = 1;
    test[16] = 64;
    test[17] = 168;
    test[18] = 33;
    test[19] = 105;
    std::vector<int> paral(rows * cols);
    paral = parallel_sobel(test, rows, cols);
    if (rank == 0) {
        std::vector<int> seq(rows * cols);
        seq = calc_sobel(test, rows, cols);
        ASSERT_EQ(seq, paral);
    }
}

TEST(test_sobel, test4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 4;
    int cols = 4;
    std::vector<int> test(rows * cols);
    test[0] = 15;
    test[1] = 74;
    test[2] = 123;
    test[3] = 98;
    test[4] = 2;
    test[5] = 10;
    test[6] = 36;
    test[7] = 89;
    test[8] = 55;
    test[9] = 47;
    test[10] = 25;
    test[11] = 81;
    test[12] = 16;
    test[13] = 158;
    test[14] = 200;
    test[15] = 247;
    std::vector<int> paral(rows * cols);
    paral = parallel_sobel(test, rows, cols);
    if (rank == 0) {
        std::vector<int> seq(rows * cols);
        seq = calc_sobel(test, rows, cols);
        ASSERT_EQ(seq, paral);
    }
}
TEST(test_sobel, test5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 5;
    int cols = 5;
    std::vector<int> test(rows * cols);
    test[0] = 2;
    test[1] = 85;
    test[2] = 3;
    test[3] = 240;
    test[4] = 89;
    test[5] = 15;
    test[6] = 209;
    test[7] = 34;
    test[8] = 100;
    test[9] = 200;
    test[10] = 4;
    test[11] = 76;
    test[12] = 133;
    test[13] = 90;
    test[14] = 5;
    test[15] = 160;
    test[16] = 51;
    test[17] = 3;
    test[18] = 16;
    test[19] = 49;
    test[20] = 178;
    test[21] = 99;
    test[22] = 107;
    test[23] = 214;
    test[24] = 85;
    std::vector<int> paral(rows * cols);
    paral = parallel_sobel(test, rows, cols);
    if (rank == 0) {
        std::vector<int> seq(rows * cols);
        seq = calc_sobel(test, rows, cols);
        ASSERT_EQ(seq, paral);
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
