// Copyright 2021 Pinezhanin Evgeny
#include <gtest/gtest.h>
#include <cmath>
#include <functional>
#include "./trapezoidal_rule.h"
#include <gtest-mpi-listener.hpp>

TEST(Trapezoidal_rule_test, x_square_a_less_b) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> f1 = [](double x) -> double { return x * x; };
    double a = 5.0, b = 10.0;
    int n = 100;

    double global_sum = getIntegralTrapezoidalRuleParallel(f1, a, b, n);

    if (rank == 0) {
        double reference_sum = getIntegralTrapezoidalRuleSequential(f1, a, b, n);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}

TEST(Trapezoidal_rule_test, x_square_a_more_b) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> f1 = [](double x) -> double { return x * x; };
    double a = 10.0, b = 5.0;
    int n = 100;

    double global_sum = getIntegralTrapezoidalRuleParallel(f1, a, b, n);

    if (rank == 0) {
        double reference_sum = getIntegralTrapezoidalRuleSequential(f1, a, b, n);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}

TEST(Trapezoidal_rule_test, x_cube) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> f2 = [](double x) -> double { return x * x * x; };
    double a = 0.0, b = 6.0;
    int n = 100;

    double global_sum = getIntegralTrapezoidalRuleParallel(f2, a, b, n);

    if (rank == 0) {
        double reference_sum = getIntegralTrapezoidalRuleSequential(f2, a, b, n);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}

TEST(Trapezoidal_rule_test, polynomial) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> f3 =
        [](double x) -> double { return 5.0 * x * x * x * x + 10.0 * x * x * x + 3.0 * x * x; };
    double a = 0.0, b = 5.0;
    int n = 1000;

    double global_sum = getIntegralTrapezoidalRuleParallel(f3, a, b, n);

    if (rank == 0) {
        double reference_sum = getIntegralTrapezoidalRuleSequential(f3, a, b, n);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}

TEST(Trapezoidal_rule_test, null) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> f4 = [](double x) -> double { return 0.0; };
    double a = -5.0, b = 5.0;
    int n = 1000;

    double global_sum = getIntegralTrapezoidalRuleParallel(f4, a, b, n);

    if (rank == 0) {
        double reference_sum = getIntegralTrapezoidalRuleSequential(f4, a, b, n);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}

TEST(Trapezoidal_rule_test, x_sinx) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> f5 = [](double x) -> double { return x * sin(x); };
    double a = -5.0, b = 5.0;
    int n = 1000;

    double global_sum = getIntegralTrapezoidalRuleParallel(f5, a, b, n);

    if (rank == 0) {
        double reference_sum = getIntegralTrapezoidalRuleSequential(f5, a, b, n);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}

TEST(Trapezoidal_rule_test, line) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> f6 = [](double x) -> double { return x; };
    double a = 2.0, b = 7.0;
    int n = 1000;

    double global_sum = getIntegralTrapezoidalRuleParallel(f6, a, b, n);

    if (rank == 0) {
        double reference_sum = getIntegralTrapezoidalRuleSequential(f6, a, b, n);
        ASSERT_DOUBLE_EQ(reference_sum, global_sum);
    }
}

TEST(Trapezoidal_rule_test, exp) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::function<double(double)> f7 = [](double x) -> double { return exp(x); };
    double a = -6.0, b = 6.0;
    int n = 1000;

    double global_sum = getIntegralTrapezoidalRuleParallel(f7, a, b, n);

    if (rank == 0) {
        double reference_sum = getIntegralTrapezoidalRuleSequential(f7, a, b, n);
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
