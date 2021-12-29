// Copyright 2021 Chelebaev Artem
#include <gtest/gtest.h>
#include <vector>
#include "../chelebaev_strassen_algorithm/strassen_algorithm.h"
#include <gtest-mpi-listener.hpp>

TEST(Multiply, test0) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    Matrix a, b;
    if (my_rank == 0) {
        int n = 2, m = 2;
        double max_num = 10.0;
        a = createRandomMatrix(n, m, max_num);
        b = createRandomMatrix(n, m, max_num);
    }
    Matrix res = parallelMulti(a, b);
    if (my_rank == 0) {
        double delta = 0.1;
        Matrix seq = sequentialMulti(a, b);
        ASSERT_TRUE(areEqual(seq, res, delta));
    }
}

TEST(Multiply, test1) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    Matrix a, b;
    if (my_rank == 0) {
        int n = 4, m = 4;
        double max_num = 10.0;
        a = createRandomMatrix(n, m, max_num);
        b = createRandomMatrix(n, m, max_num);
    }
    Matrix res = parallelMulti(a, b);
    if (my_rank == 0) {
        double delta = 0.1;
        Matrix seq = sequentialMulti(a, b);
        ASSERT_TRUE(areEqual(seq, res, delta));
    }
}

TEST(Multiply, test2) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    Matrix a, b;
    if (my_rank == 0) {
        int n = 8, m = 8;
        double max_num = 10.0;
        a = createRandomMatrix(n, m, max_num);
        b = createRandomMatrix(n, m, max_num);
    }
    Matrix res = parallelMulti(a, b);
    if (my_rank == 0) {
        double delta = 0.1;
        Matrix seq = sequentialMulti(a, b);
        ASSERT_TRUE(areEqual(seq, res, delta));
    }
}

TEST(Multiply, test3) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    Matrix a, b;
    if (my_rank == 0) {
        int h = 16, w = 16;
        double max_num = 10.0;
        a = createRandomMatrix(h, w, max_num);
        b = createRandomMatrix(h, w, max_num);
    }
    Matrix res = parallelMulti(a, b);
    if (my_rank == 0) {
        double delta = 0.1;
        Matrix seq = sequentialMulti(a, b);
        ASSERT_TRUE(areEqual(seq, res, delta));
    }
}

TEST(Multiply, test4) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    Matrix a, b;
    if (my_rank == 0) {
        int n = 32, m = 32;
        double max_num = 100.0;
        a = createRandomMatrix(n, m, max_num);
        b = createRandomMatrix(n, m, max_num);
    }
    Matrix res = parallelMulti(a, b);
    if (my_rank == 0) {
        double delta = 0.1;
        Matrix seq = sequentialMulti(a, b);
        ASSERT_TRUE(areEqual(seq, res, delta));
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
