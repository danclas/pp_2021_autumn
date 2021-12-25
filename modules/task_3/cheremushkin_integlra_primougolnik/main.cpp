// Copyright 2021 Gordey Maria
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_3/cheremushkin_integlra_primougolnik/integral.h"

TEST(INTEGRAL_MPI, TEST_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::pair<double, double>> limits(2);
    limits = { {0, 2}, {0, 1} };
    double reference_result;

    if (rank == 0) {
        reference_result = linelintegral(limits);
    }
    double parallel_result = Parallelintegral(limits);
    if (rank == 0) {
        ASSERT_NEAR(reference_result, parallel_result, eps);
    }
}

TEST(INTEGRAL_MPI, TEST_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::pair<double, double>> limits(2);
    limits = { {0, 1}, {0, 1.5} };
    double reference_result;
    if (rank == 0) {
        reference_result = linelintegral(limits);
    }

    double parallel_result = Parallelintegral(limits);

    if (rank == 0) {
        ASSERT_NEAR(reference_result, parallel_result, eps);
    }
}

TEST(INTEGRAL_MPI, TEST_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::pair<double, double>> limits(3);
    limits = { {-2, 3}, {0, 1}, {0, 2.4} };
    double reference_result;

    if (rank == 0) {
        reference_result = linelintegral(limits);
    }

    double parallel_result = Parallelintegral(limits);

    if (rank == 0) {
        ASSERT_NEAR(reference_result, parallel_result, eps);
    }
}

TEST(INTEGRAL_MPI, TEST_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::pair<double, double>> limits(3);
    limits = { {0, 1}, {-3, 2.5}, {0, exp(2)} };
    double reference_result;

    if (rank == 0) {
        reference_result = linelintegral(limits);
    }

    double parallel_result = Parallelintegral(limits);

    if (rank == 0) {
        ASSERT_NEAR(reference_result, parallel_result, eps);
    }
}

TEST(INTEGRAL_MPI, TEST_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    std::vector<std::pair<double, double>> limits(3);
    limits = { {0, 2}, {-1, 2}, {1, exp(1)} };
    double reference_result;

    if (rank == 0) {
        reference_result = linelintegral(limits);
    }

    double parallel_result = Parallelintegral(limits);

    if (rank == 0) {
        ASSERT_NEAR(reference_result, parallel_result, eps);
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
