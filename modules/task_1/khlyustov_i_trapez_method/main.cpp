// Copyright 2021 Khlyustov Ilya
#include <gtest/gtest.h>
#include "./trapez_method_mpi.h"
#include <gtest-mpi-listener.hpp>

TEST(Trapez_Method_MPI, x_squared) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int a = 0, b = 10, n = 100;
    const std::function<double(double)> f = [](double x) { return x * x; };

    double par = getParallel(a, b, n, f);

    if (proc_rank == 0) {
        double seq = getSequential(a, b, n, f);
        ASSERT_NEAR(par, seq, 1e-9);
    }
}

TEST(Trapez_Method_MPI, x_cube) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int a = 0, b = 10, n = 100;
    const std::function<double(double)> f = [](double x) { return x * x * x; };

    double par = getParallel(a, b, n, f);

    if (proc_rank == 0) {
        double seq = getSequential(a, b, n, f);
        ASSERT_NEAR(par, seq, 1e-9);
    }
}

TEST(Trapez_Method_MPI, sqrt_x) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int a = 0, b = 10, n = 100;
    const std::function<double(double)> f = [](double x) { return sqrt(x); };

    double par = getParallel(a, b, n, f);

    if (proc_rank == 0) {
        double seq = getSequential(a, b, n, f);
        ASSERT_NEAR(par, seq, 1e-9);
    }
}

TEST(Trapez_Method_MPI, sin_x) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int a = 0, b = 10, n = 100;
    const std::function<double(double)> f = [](double x) { return sin(x); };

    double par = getParallel(a, b, n, f);

    if (proc_rank == 0) {
        double seq = getSequential(a, b, n, f);
        ASSERT_NEAR(par, seq, 1e-9);
    }
}

TEST(Trapez_Method_MPI, x_polynom) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int a = 0, b = 10, n = 100;
    const std::function<double(double)> f = [](double x) { return x * x - 2 * x + cos(x); };

    double par = getParallel(a, b, n, f);

    if (proc_rank == 0) {
        double seq = getSequential(a, b, n, f);
        ASSERT_NEAR(par, seq, 1e-9);
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
