// Copyright 2021 Medvedeva Karina
#include <gtest/gtest.h>
#include <vector>
#include "./gauss_block_partition.h"
#include <gtest-mpi-listener.hpp>

TEST(Generation_Matrix, can_generate_square_matrix) {
    ASSERT_NO_THROW(getRandomMatrix(10, 10));
}

TEST(Generation_Matrix, can_generate_arbitrary_matrix) {
    ASSERT_NO_THROW(getRandomMatrix(10, 15));
}

TEST(Sequential_Operations_MPI, getSequentialOperations_can_work_with_square_matrix) {
    std::vector<int> matrix = getRandomMatrix(10, 10);

    ASSERT_NO_THROW(getSequentialOperations(matrix, 10, 10));
}

TEST(Sequential_Operations_MPI, getSequentialOperations_can_work_with_arbitrary_matrix) {
    std::vector<int> matrix = getRandomMatrix(10, 15);

    ASSERT_NO_THROW(getSequentialOperations(matrix, 10, 15));
}

TEST(Parallel_Operations_MPI, getParallelOperations_can_work_with_square_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>::size_type row_count = 20, column_count = 20;
    std::vector<int> matrix(row_count * column_count);

    if (rank == 0) {
        matrix = getRandomMatrix(row_count, column_count);
    }

    ASSERT_NO_THROW(getParallelOperations(matrix, 20));
}

TEST(Parallel_Operations_MPI, getParallelOperations_works_correctly) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>::size_type matrix_size = 18;
    std::vector<int> matrix(matrix_size * matrix_size);

    if (rank == 0) {
        matrix = getRandomMatrix(matrix_size, matrix_size);
    }

    getGaussKernel(2.0);
    std::vector<int> res = getParallelOperations(matrix, matrix_size);

    if (rank == 0) {
        std::vector<int> expected_res = getSequentialOperations(matrix, matrix_size, matrix_size);

        ASSERT_EQ(res, expected_res);
    }
}

TEST(DISABLED_Parallel_Operations_MPI, boost_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>::size_type matrix_size = 4096;
    std::vector<int> matrix(matrix_size * matrix_size);

    if (rank == 0) {
        matrix = getRandomMatrix(matrix_size, matrix_size);
    }

    getGaussKernel(2.0);

    auto start_getParallelOperations = MPI_Wtime();
    std::vector<int> res = getParallelOperations(matrix, matrix_size);
    auto finish_getParallelOperations = MPI_Wtime();

    if (rank == 0) {
        auto start_getSequentialOperations = MPI_Wtime();
        std::vector<int> expected_res = getSequentialOperations(matrix, matrix_size, matrix_size);
        auto finish_getSequentialOperations = MPI_Wtime();

        auto getSequentialOperations_running_time = finish_getSequentialOperations - start_getSequentialOperations;
        auto getParallelOperations_running_time = finish_getParallelOperations - start_getParallelOperations;
        auto acceleration = getSequentialOperations_running_time / getParallelOperations_running_time;

        printf("Running time of sequential algorithm=%lf\nRunning time of parallel algorithm=%lf\n",
            getSequentialOperations_running_time, getParallelOperations_running_time);
        printf("Acceleration=%lf\n", acceleration);

        ASSERT_EQ(res, expected_res);
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
