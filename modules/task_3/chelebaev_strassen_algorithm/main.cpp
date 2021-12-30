// Copyright 2021 Chelebaev Artem
#include <gtest/gtest.h>
#include <vector>
#include "../chelebaev_strassen_algorithm/strassen_algorithm.h"
#include <gtest-mpi-listener.hpp>

TEST(Multiply, Matrix16) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    Matrix a, b;
    double stime, ptime;
    if (my_rank == 0) {
        int h = 16, w = 16;
        double max_num = 10.0;
        a = createRandomMatrix(h, w, max_num);
        b = createRandomMatrix(h, w, max_num);
    }
    clock_t startp = clock();
    Matrix res = parallelMulti(a, b);
    ptime = (clock() - startp) / static_cast<double>(CLOCKS_PER_SEC);
    if (my_rank == 0) {
        double delta = 0.1;
        clock_t starts = clock();
        Matrix seq = sequentialMulti(a, b);
        stime = (clock() - starts) / static_cast<double>(CLOCKS_PER_SEC);
        std::cout << "Sequential time: " << stime << std::endl;
        std::cout << "Parallel time: " << ptime << std::endl;
        ASSERT_TRUE(areEqual(seq, res, delta));
    }
}

TEST(Multiply, Matrix32) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    Matrix a, b;
    double stime, ptime;
    if (my_rank == 0) {
        int n = 32, m = 32;
        double max_num = 100.0;
        a = createRandomMatrix(n, m, max_num);
        b = createRandomMatrix(n, m, max_num);
    }
    clock_t startp = clock();
    Matrix res = parallelMulti(a, b);
    ptime = (clock() - startp) / static_cast<double>(CLOCKS_PER_SEC);
    if (my_rank == 0) {
        double delta = 0.1;
        clock_t starts = clock();
        Matrix seq = sequentialMulti(a, b);
        stime = (clock() - starts) / static_cast<double>(CLOCKS_PER_SEC);
        std::cout << "Sequential time: " << stime << std::endl;
        std::cout << "Parallel time: " << ptime << std::endl;
        ASSERT_TRUE(areEqual(seq, res, delta));
    }
}

TEST(Multiply, Matrix64) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    Matrix a, b;
    double stime, ptime;
    if (my_rank == 0) {
        int n = 64, m = 64;
        double max_num = 100.0;
        a = createRandomMatrix(n, m, max_num);
        b = createRandomMatrix(n, m, max_num);
    }
    clock_t startp = clock();
    Matrix res = parallelMulti(a, b);
    ptime = (clock() - startp) / static_cast<double>(CLOCKS_PER_SEC);
    if (my_rank == 0) {
        double delta = 0.1;
        clock_t starts = clock();
        Matrix seq = sequentialMulti(a, b);
        stime = (clock() - starts) / static_cast<double>(CLOCKS_PER_SEC);
        std::cout << "Sequential time: " << stime << std::endl;
        std::cout << "Parallel time: " << ptime << std::endl;
        ASSERT_TRUE(areEqual(seq, res, delta));
    }
}

TEST(Multiply, Matrix128) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    Matrix a, b;
    double stime, ptime;
    if (my_rank == 0) {
        int n = 128, m = 128;
        double max_num = 100.0;
        a = createRandomMatrix(n, m, max_num);
        b = createRandomMatrix(n, m, max_num);
    }
    clock_t startp = clock();
    Matrix res = parallelMulti(a, b);
    ptime = (clock() - startp) / static_cast<double>(CLOCKS_PER_SEC);
    if (my_rank == 0) {
        double delta = 0.1;
        clock_t starts = clock();
        Matrix seq = sequentialMulti(a, b);
        stime = (clock() - starts) / static_cast<double>(CLOCKS_PER_SEC);
        std::cout << "Sequential time: " << stime << std::endl;
        std::cout << "Parallel time: " << ptime << std::endl;
        ASSERT_TRUE(areEqual(seq, res, delta));
    }
}

TEST(Multiply, Matrix256) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    Matrix a, b;
    double stime, ptime;
    if (my_rank == 0) {
        int n = 256, m = 256;
        double max_num = 100.0;
        a = createRandomMatrix(n, m, max_num);
        b = createRandomMatrix(n, m, max_num);
    }
    clock_t startp = clock();
    Matrix res = parallelMulti(a, b);
    ptime = (clock() - startp) / static_cast<double>(CLOCKS_PER_SEC);
    if (my_rank == 0) {
        double delta = 0.1;
        clock_t starts = clock();
        Matrix seq = sequentialMulti(a, b);
        stime = (clock() - starts) / static_cast<double>(CLOCKS_PER_SEC);
        std::cout << "Sequential time: " << stime << std::endl;
        std::cout << "Parallel time: " << ptime << std::endl;
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
