// Copyright 2021 Popp Maksim
#include <gtest/gtest.h>
#include "./max_value_of_matrix.h"
#include <gtest-mpi-listener.hpp>

TEST(MAX_IN_PARALLEL_MPI, Size_2x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* testMatrix = nullptr;
    const int k = 2, l = 3;

    if (rank == 0) {
        testMatrix = generateMatrixRandom(k, l);
    }

    int max = findingMaxInParallel(testMatrix, k, l);

    if (rank == 0) {
        int maxRef = searchMaxSequentially(testMatrix, k, l);
        ASSERT_EQ(maxRef, max);
    }
}

TEST(MAX_IN_PARALLEL_MPI, Size_5x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* testMatrix = nullptr;
    const int k = 5, l = 5;

    if (rank == 0) {
        testMatrix = generateMatrixRandom(k, l);
    }

    int max = findingMaxInParallel(testMatrix, k, l);

    if (rank == 0) {
        int maxRef = searchMaxSequentially(testMatrix, k, l);
        ASSERT_EQ(maxRef, max);
    }
}

TEST(MAX_IN_PARALLEL_MPI, Size_20x30) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* testMatrix = nullptr;
    const int k = 20, l = 30;

    if (rank == 0) {
        testMatrix = generateMatrixRandom(k, l);
    }

    int max = findingMaxInParallel(testMatrix, k, l);

    if (rank == 0) {
        int maxRef = searchMaxSequentially(testMatrix, k, l);
        ASSERT_EQ(maxRef, max);
    }
}


TEST(MAX_IN_PARALLEL_MPI, Size_10x15) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* testMatrix = nullptr;
    const int k = 10, l = 15;

    if (rank == 0) {
        testMatrix = generateMatrixRandom(k, l);
    }

    int max = findingMaxInParallel(testMatrix, k, l);

    if (rank == 0) {
        int maxRef = searchMaxSequentially(testMatrix, k, l);
        ASSERT_EQ(maxRef, max);
    }
}





TEST(MAX_IN_PARALLEL_MPI, Size_50x50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* testMatrix = nullptr;
    const int k = 50, l = 50;

    if (rank == 0) {
        testMatrix = generateMatrixRandom(k, l);
    }

    int max = findingMaxInParallel(testMatrix, k, l);

    if (rank == 0) {
        int maxRef = searchMaxSequentially(testMatrix, k, l);
        ASSERT_EQ(maxRef, max);
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
