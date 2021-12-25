// Copyright 2021 Preobrazhenskaya Yuliya
#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "./multi_dimension_monte_carlo.h"
#include <gtest-mpi-listener.hpp>

double func_1(std::vector<double> v) {
    return 7 * v[0] * v[0] + 6 * v[1] * v[1] * v[1];
}

double func_2(std::vector<double> v) {
    return atan(v[0]) + 7 * sin(v[1]) + 3 * v[2] * v[2];
}

double func_3(std::vector<double> v) {
    return v[0] + 8 * v[1] + v[2] * v[2] * v[2] - cos(v[3]);
}

double func_4(std::vector<double> v) {
    return 5 * v[0] * v[0] * v[0] - 2 * v[1] + sin(v[2] * v[3] * v[4]);
}

TEST(Multi_Dimension_Monte_Carlo_MPI, dimension_is_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double>lower = { 0.0, 1.0 };
    std::vector<double>upper = { 1.0, 2.0 };
    int number_of_points = 100000;
    double start = MPI_Wtime();
    double parallel_result = getParallelOperations(lower, upper,
        number_of_points, func_1);
    double end = MPI_Wtime();
    if (rank == 0) {
        double parallel_runtime = end - start;
        double start = MPI_Wtime();
        double sequential_result = getSequentialOperations(lower, upper,
            number_of_points, func_1);
        double end = MPI_Wtime();
        double sequential_runtime = end - start;
        double error = abs(sequential_result - parallel_result);
        std::cout << "parallel_result = " << parallel_result << std::endl;
        std::cout << "sequential_result = " << sequential_result << std::endl;
        std::cout << "parallel_runtime = " << parallel_runtime << std::endl;
        std::cout << "sequential_runtime = " << sequential_runtime
            << std::endl;
        ASSERT_NEAR(parallel_result, sequential_result, error);
    }
}

TEST(Multi_Dimension_Monte_Carlo_MPI, dimension_is_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double>lower = { 0.5, 1.5, 3.0 };
    std::vector<double>upper = { 2.0, 3.0, 5.5 };
    int number_of_points = 100000;
    double start = MPI_Wtime();
    double parallel_result = getParallelOperations(lower, upper,
        number_of_points, func_2);
    double end = MPI_Wtime();
    if (rank == 0) {
        double parallel_runtime = end - start;
        double start = MPI_Wtime();
        double sequential_result = getSequentialOperations(lower, upper,
            number_of_points, func_2);
        double end = MPI_Wtime();
        double sequential_runtime = end - start;
        double error = abs(sequential_result - parallel_result);
        std::cout << "parallel_result = " << parallel_result << std::endl;
        std::cout << "sequential_result = " << sequential_result << std::endl;
        std::cout << "parallel_runtime = " << parallel_runtime << std::endl;
        std::cout << "sequential_runtime = " << sequential_runtime
            << std::endl;
        ASSERT_NEAR(parallel_result, sequential_result, error);
    }
}

TEST(Multi_Dimension_Monte_Carlo_MPI, dimension_is_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double>lower = { -2.0, 1.3, -4.0, 0.0 };
    std::vector<double>upper = { 0.0, 3.6, -1.0, 2.0 };
    int number_of_points = 100000;
    double start = MPI_Wtime();
    double parallel_result = getParallelOperations(lower, upper,
        number_of_points, func_3);
    double end = MPI_Wtime();
    if (rank == 0) {
        double parallel_runtime = end - start;
        double start = MPI_Wtime();
        double sequential_result = getSequentialOperations(lower, upper,
            number_of_points, func_3);
        double end = MPI_Wtime();
        double sequential_runtime = end - start;
        double error = abs(sequential_result - parallel_result);
        std::cout << "parallel_result = " << parallel_result << std::endl;
        std::cout << "sequential_result = " << sequential_result << std::endl;
        std::cout << "parallel_runtime = " << parallel_runtime << std::endl;
        std::cout << "sequential_runtime = " << sequential_runtime
            << std::endl;
        ASSERT_NEAR(parallel_result, sequential_result, error);
    }
}

TEST(Multi_Dimension_Monte_Carlo_MPI, dimension_is_4_with_more_points) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double>lower = { -2.0, 1.3, -4.0, 0.0 };
    std::vector<double>upper = { 0.0, 3.6, -1.0, 2.0 };
    int number_of_points = 1000000;
    double start = MPI_Wtime();
    double parallel_result = getParallelOperations(lower, upper,
        number_of_points, func_3);
    double end = MPI_Wtime();
    if (rank == 0) {
        double parallel_runtime = end - start;
        double start = MPI_Wtime();
        double sequential_result = getSequentialOperations(lower, upper,
            number_of_points, func_3);
        double end = MPI_Wtime();
        double sequential_runtime = end - start;
        double error = abs(sequential_result - parallel_result);
        std::cout << "parallel_result = " << parallel_result << std::endl;
        std::cout << "sequential_result = " << sequential_result << std::endl;
        std::cout << "parallel_runtime = " << parallel_runtime << std::endl;
        std::cout << "sequential_runtime = " << sequential_runtime
            << std::endl;
        ASSERT_NEAR(parallel_result, sequential_result, error);
    }
}

TEST(Multi_Dimension_Monte_Carlo_MPI, dimension_is_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double>lower = { 0.0, 1.5, 1.0, 3.0, 2.0 };
    std::vector<double>upper = { 4.0, 2.5, 5.0, 6.0, 3.0 };
    int number_of_points = 100000;
    double start = MPI_Wtime();
    double parallel_result = getParallelOperations(lower, upper,
        number_of_points, func_4);
    double end = MPI_Wtime();
    if (rank == 0) {
        double parallel_runtime = end - start;
        double start = MPI_Wtime();
        double sequential_result = getSequentialOperations(lower, upper,
            number_of_points, func_4);
        double end = MPI_Wtime();
        double sequential_runtime = end - start;
        double error = abs(sequential_result - parallel_result);
        std::cout << "parallel_result = " << parallel_result << std::endl;
        std::cout << "sequential_result = " << sequential_result << std::endl;
        std::cout << "parallel_runtime = " << parallel_runtime << std::endl;
        std::cout << "sequential_runtime = " << sequential_runtime
            << std::endl;
        ASSERT_NEAR(parallel_result, sequential_result, error);
    }
}

TEST(Multi_Dimension_Monte_Carlo_MPI, dimension_is_5_with_more_points) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double>lower = { 0.0, 1.5, 1.0, 3.0, 2.0 };
    std::vector<double>upper = { 4.0, 2.5, 5.0, 6.0, 3.0 };
    int number_of_points = 1000000;
    double start = MPI_Wtime();
    double parallel_result = getParallelOperations(lower, upper,
        number_of_points, func_4);
    double end = MPI_Wtime();
    if (rank == 0) {
        double parallel_runtime = end - start;
        double start = MPI_Wtime();
        double sequential_result = getSequentialOperations(lower, upper,
            number_of_points, func_4);
        double end = MPI_Wtime();
        double sequential_runtime = end - start;
        double error = abs(sequential_result - parallel_result);
        std::cout << "parallel_result = " << parallel_result << std::endl;
        std::cout << "sequential_result = " << sequential_result << std::endl;
        std::cout << "parallel_runtime = " << parallel_runtime << std::endl;
        std::cout << "sequential_runtime = " << sequential_runtime
            << std::endl;
        ASSERT_NEAR(parallel_result, sequential_result, error);
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
