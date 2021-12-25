// Copyright 2021 Pyatckin Nikolai
#include <gtest/gtest.h>
#include <mpi.h>
#include <math.h>
#include <utility>
#include <vector>
#include "../../../modules/task_3/pyatckin_n_integrals_rectangle/integrals_rectangle.h"
#include <gtest-mpi-listener.hpp>

TEST(Integration, 2_dimension_integral) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 2;
    std::vector<std::pair<double, double>> scope(n);
    if (rank == 0)
        scope = { {4, 10}, {1, 56} };
    std::function<double(const std::vector<double>&)>
        f = [](const std::vector<double>& vec) {
        return vec[0] * vec[0] - 2 * vec[1]; };  // x * x - 2 * y
    double result = ParallelIntegral(f, scope);

    if (rank == 0) {
        double error = 0.0001;
        ASSERT_NEAR(result, SeqIntegral(f, scope), error);
    }
}

TEST(Integration, 3_dimension_integrl) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 3;
    std::vector<std::pair<double, double>> scope(n);
    if (rank == 0)
        scope = { { 0, 1 }, { -13, 5 }, { 3, 7 } };
    std::function<double(const std::vector<double>&)>
        f = [](const std::vector<double>& vec) {
        return vec[0] + vec[1] * vec[1] +
            vec[2] * vec[2] * vec[2]; };  // x + y*y + z*z*z
    double result = ParallelIntegral(f, scope);

    if (rank == 0) {
        double error = 0.0001;
        ASSERT_NEAR(result, SeqIntegral(f, scope), error);
    }
}

TEST(Integration, 3_dimension_integral_use_log) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 3;
    std::vector<std::pair<double, double>> scope(n);
    if (rank == 0)
        scope = { { 0, 1 }, { -13, 5 }, { 3, 7 } };
    std::function<double(const std::vector<double>&)>
        f = [](const std::vector<double>& vec) {
        return log10(2 * vec[0] * vec[0])
            + sqrt(vec[2]) + 5 * vec[1]; };  // log10(2*x*x) + sqrt(z) + 5*y

    double result = ParallelIntegral(f, scope);

    if (rank == 0) {
        double error = 0.0001;
        ASSERT_NEAR(result, SeqIntegral(f, scope), error);
    }
}

TEST(Integration, 3_dimension_integral_use_sin_and_cos) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 3;
    std::vector<std::pair<double, double>> scope(n);
    if (rank == 0)
        scope = { { -9, 1 }, { -100, 100 }, { -2, 2 } };
    std::function<double(const std::vector<double>&)>
        f = [](const std::vector<double>& vec) {
        return (exp(vec[0]) - sqrt(10) * 5 * sin(vec[1])
            + cos(-2 * vec[2] * vec[2])); };
    // exp(x) - sqrt(10) * 5 * sin(y) + cos(-2 * z * z)

    double result = ParallelIntegral(f, scope);

    if (rank == 0) {
        double error = 0.0001;
        ASSERT_NEAR(result, SeqIntegral(f, scope), error);
    }
}

TEST(Integration, 4_dimension_integral) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 4;
    std::vector<std::pair<double, double>> scope(n);
    if (rank == 0)
        scope = { { -5, 2 }, { -4.5, 5 }, { -7, 2 }, { 5, 7 } };

    std::function<double(const std::vector<double>&)>
        f = [](const std::vector<double>& vec) {
        return vec[0] + vec[1] + vec[2] + vec[3]; };  // x + y + z + t

    double result = ParallelIntegral(f, scope);
    if (rank == 0) {
        double error = 0.0001;
        ASSERT_NEAR(result, SeqIntegral(f, scope), error);
    }
}

TEST(Integration, 4_dimension_integral_exp_and_sin) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 4;
    std::vector<std::pair<double, double>> scope(n);
    if (rank == 0)
        scope = { { -4.5, 0 }, { 0, 5 }, { -58, 12 }, { 5, 73 } };

    std::function<double(const std::vector<double>&)>
        f = [](const std::vector<double>& vec) {
        return cos(5 * vec[0]) + exp(vec[1]) + 2.9 *
            sin(vec[2]) - vec[3] * vec[3]; };
    // cos(5*x) + exp(y) + 2.9*sin(z) - t*t

    double result = ParallelIntegral(f, scope);
    if (rank == 0) {
        double error = 0.0001;
        ASSERT_NEAR(result, SeqIntegral(f, scope), error);
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
