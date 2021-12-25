// Copyright 2021 Sablin Alexander

#include <gtest/gtest.h>
#include <cmath>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_3/sablin_a_trapezoid_method/trapezoid_method.h"


double function_1(double x, double y) {
  return sin(x) * cos(y);
}

double function_2(double x, double y) {
  return x * y;
}

double function_3(double x, double y) {
  return pow(x, y);
}

double function_4(double x, double y) {
  return x * x + y * y;
}

double function_5(double x, double y) {
  return 1;
}

TEST(Trapezoid_method_test, Test_function_1_n_10000) {
    double (*f)(double, double) = function_1;
    const int n = 10000;
    double a_x = 0.0;
    double b_x = 3.0;
    double a_y = 0.0;
    double b_y = 3.0;
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double start, finish, parallel_time, sequential_time, parallel_res, sequential_res;
    if (ProcRank == 0) {
        start = MPI_Wtime();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    parallel_res = trapezoidMethodParallel(f, a_x, b_x, a_y, b_y, n);
    MPI_Barrier(MPI_COMM_WORLD);
    if (ProcRank == 0) {
        finish = MPI_Wtime();
        parallel_time = finish - start;
        start = MPI_Wtime();
        sequential_res = trapezoidMethodSequential(f, a_x, b_x, a_y, b_y, n);
        finish = MPI_Wtime();
        sequential_time = finish - start;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "--- func : sin(x) * cos(y) " << std::endl;
        std::cout << "--- n : 10000 " << std::endl;
        std::cout << "--- Sequential time = " << sequential_time << std::endl;
        std::cout << "--- Parallel time = " << parallel_time << std::endl;
        std::cout << "--- Acceleration = " << sequential_time / parallel_time << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        ASSERT_EQ(std::abs(parallel_res - sequential_res) < 0.015, true);
    }
}

TEST(Trapezoid_method_test, Test_function_2_n_10000) {
    double (*f)(double, double) = function_2;
    const int n = 10000;
    double a_x = 0.0;
    double b_x = 3.0;
    double a_y = 0.0;
    double b_y = 3.0;
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double start, finish, parallel_time, sequential_time, parallel_res, sequential_res;
    if (ProcRank == 0) {
        start = MPI_Wtime();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    parallel_res = trapezoidMethodParallel(f, a_x, b_x, a_y, b_y, n);
    MPI_Barrier(MPI_COMM_WORLD);
    if (ProcRank == 0) {
        finish = MPI_Wtime();
        parallel_time = finish - start;
        start = MPI_Wtime();
        sequential_res = trapezoidMethodSequential(f, a_x, b_x, a_y, b_y, n);
        finish = MPI_Wtime();
        sequential_time = finish - start;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "--- func : x * y " << std::endl;
        std::cout << "--- n : 10000 " << std::endl;
        std::cout << "--- Sequential time = " << sequential_time << std::endl;
        std::cout << "--- Parallel time = " << parallel_time << std::endl;
        std::cout << "--- Acceleration = " << sequential_time / parallel_time << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        ASSERT_EQ(std::abs(parallel_res - sequential_res) < 0.015, true);
    }
}

TEST(Trapezoid_method_test, Test_function_2_n_20000) {
    double (*f)(double, double) = function_2;
    const int n = 20000;
    double a_x = 0.0;
    double b_x = 3.0;
    double a_y = 0.0;
    double b_y = 3.0;
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double start, finish, parallel_time, sequential_time, parallel_res, sequential_res;
    if (ProcRank == 0) {
        start = MPI_Wtime();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    parallel_res = trapezoidMethodParallel(f, a_x, b_x, a_y, b_y, n);
    MPI_Barrier(MPI_COMM_WORLD);
    if (ProcRank == 0) {
        finish = MPI_Wtime();
        parallel_time = finish - start;
        start = MPI_Wtime();
        sequential_res = trapezoidMethodSequential(f, a_x, b_x, a_y, b_y, n);
        finish = MPI_Wtime();
        sequential_time = finish - start;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "--- func : x * y " << std::endl;
        std::cout << "--- n : 20000 " << std::endl;
        std::cout << "--- Sequential time = " << sequential_time << std::endl;
        std::cout << "--- Parallel time = " << parallel_time << std::endl;
        std::cout << "--- Acceleration = " << sequential_time / parallel_time << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        ASSERT_EQ(std::abs(parallel_res - sequential_res) < 0.015, true);
    }
}

TEST(Trapezoid_method_test, Test_function_3_n_10000) {
    double (*f)(double, double) = function_3;
    const int n = 10000;
    double a_x = 0.0;
    double b_x = 3.0;
    double a_y = 0.0;
    double b_y = 3.0;
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double start, finish, parallel_time, sequential_time, parallel_res, sequential_res;
    if (ProcRank == 0) {
        start = MPI_Wtime();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    parallel_res = trapezoidMethodParallel(f, a_x, b_x, a_y, b_y, n);
    MPI_Barrier(MPI_COMM_WORLD);
    if (ProcRank == 0) {
        finish = MPI_Wtime();
        parallel_time = finish - start;
        start = MPI_Wtime();
        sequential_res = trapezoidMethodSequential(f, a_x, b_x, a_y, b_y, n);
        finish = MPI_Wtime();
        sequential_time = finish - start;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "--- func : pow(x, y) " << std::endl;
        std::cout << "--- n : 10000 " << std::endl;
        std::cout << "--- Sequential time = " << sequential_time << std::endl;
        std::cout << "--- Parallel time = " << parallel_time << std::endl;
        std::cout << "--- Acceleration = " << sequential_time / parallel_time << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        ASSERT_EQ(std::abs(parallel_res - sequential_res) < 0.015, true);
    }
}

TEST(Trapezoid_method_test, Test_function_4_n_10000) {
    double (*f)(double, double) = function_4;
    const int n = 10000;
    double a_x = 0.0;
    double b_x = 3.0;
    double a_y = 0.0;
    double b_y = 3.0;
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double start, finish, parallel_time, sequential_time, parallel_res, sequential_res;
    if (ProcRank == 0) {
        start = MPI_Wtime();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    parallel_res = trapezoidMethodParallel(f, a_x, b_x, a_y, b_y, n);
    MPI_Barrier(MPI_COMM_WORLD);
    if (ProcRank == 0) {
        finish = MPI_Wtime();
        parallel_time = finish - start;
        start = MPI_Wtime();
        sequential_res = trapezoidMethodSequential(f, a_x, b_x, a_y, b_y, n);
        finish = MPI_Wtime();
        sequential_time = finish - start;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "--- func : x * x + y * y " << std::endl;
        std::cout << "--- n : 10000 " << std::endl;
        std::cout << "--- Sequential time = " << sequential_time << std::endl;
        std::cout << "--- Parallel time = " << parallel_time << std::endl;
        std::cout << "--- Acceleration = " << sequential_time / parallel_time << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        ASSERT_EQ(std::abs(parallel_res - sequential_res) < 0.015, true);
    }
}

TEST(Trapezoid_method_test, Test_function_5_n_10000) {
    double (*f)(double, double) = function_5;
    const int n = 10000;
    double a_x = 0.0;
    double b_x = 3.0;
    double a_y = 0.0;
    double b_y = 3.0;
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double start, finish, parallel_time, sequential_time, parallel_res, sequential_res;
    if (ProcRank == 0) {
        start = MPI_Wtime();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    parallel_res = trapezoidMethodParallel(f, a_x, b_x, a_y, b_y, n);
    MPI_Barrier(MPI_COMM_WORLD);
    if (ProcRank == 0) {
        finish = MPI_Wtime();
        parallel_time = finish - start;
        start = MPI_Wtime();
        sequential_res = trapezoidMethodSequential(f, a_x, b_x, a_y, b_y, n);
        finish = MPI_Wtime();
        sequential_time = finish - start;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "--- func : 1 " << std::endl;
        std::cout << "--- n : 10000 " << std::endl;
        std::cout << "--- Sequential time = " << sequential_time << std::endl;
        std::cout << "--- Parallel time = " << parallel_time << std::endl;
        std::cout << "--- Acceleration = " << sequential_time / parallel_time << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        ASSERT_EQ(std::abs(parallel_res - sequential_res) < 0.015, true);
    }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

  return RUN_ALL_TESTS();
}
