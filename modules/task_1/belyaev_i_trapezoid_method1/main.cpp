// Copyright 2021 Belyaev Ilya
#include <gtest/gtest.h>
#include <cmath>
#include <functional>
#include "./trapezoid_method.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_MPI, x_squared) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int a = 0, b = 20, n = 200;
    const std::function<double(double)> f = [](double x) { return x * x; };

    double global_sum = getParallel(a, b, n, f);

    if (rank == 0) {
        double reference_sum = getIntegral(a, b, n, f);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}
TEST(Parallel_MPI, x_cube) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a = 0, b = 20, n = 200;
    const std::function<double(double)> f = [](double x) { return x * x * x; };

    double global_sum = getParallel(a, b, n, f);

    if (rank == 0) {
        double reference_sum = getIntegral(a, b, n, f);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}
TEST(Parallel_MPI, x_cos) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a = 0, b = 20, n = 200;
    const std::function<double(double)> f = [](double x) { return cos(x); };

    double global_sum = getParallel(a, b, n, f);

    if (rank == 0) {
        double reference_sum = getIntegral(a, b, n, f);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}
TEST(Parallel_MPI, x_polynom) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a = 0, b = 10, n = 100;
    const std::function<double(double)> f =
        [](double x) {return x * x - 5 * x + 4;};

    double global_sum = getParallel(a, b, n, f);

    if (rank == 0) {
        double reference_sum = getIntegral(a, b, n, f);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}
TEST(Parallel_MPI, x_tan) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a = 0, b = 20, n = 200;
    const std::function<double(double)> f = [](double x) { return tan(x); };

    double global_sum = getParallel(a, b, n, f);

    if (rank == 0) {
        double reference_sum = getIntegral(a, b, n, f);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
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
