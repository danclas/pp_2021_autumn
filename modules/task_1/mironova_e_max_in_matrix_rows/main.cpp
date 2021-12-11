// Copyright 2021 Mironova Ekaterina
#include <gtest/gtest.h>
#include <vector>
#include "./max_in_matrix_rows.h"
#include <gtest-mpi-listener.hpp>

TEST(MPI_TASK_1, cant_find_maxes_in_empty_matrix) {
    size_t columns = 0;
    std::vector<int> matrix;
    ASSERT_ANY_THROW(findMaxesInMatrixRows(matrix, columns));
}

TEST(MPI_TASK_1, work_correct_with_matrix_4_on_5) {
    size_t rows = 4, columns = 5;
    std::vector<int> matrix;
    std::vector<int> result1, result2;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix = fillRandomMatrix(rows, columns);
        result1 = findMaxesInMatrixRows(matrix, columns);
    }
    result2 = parallelFindMaxesInMatrixRows(matrix, rows, columns);
    if (procRank == 0) {
        ASSERT_EQ(result1, result2);
    }
}

TEST(MPI_TASK_1, work_correct_with_matrix_3_on_3) {
    size_t rows = 3, columns = 3;
    std::vector<int> matrix;
    std::vector<int> result1, result2;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix = fillRandomMatrix(rows, columns);
        result1 = findMaxesInMatrixRows(matrix, columns);
    }
    result2 = parallelFindMaxesInMatrixRows(matrix, rows, columns);
    if (procRank == 0) {
        ASSERT_EQ(result1, result2);
    }
}

TEST(MPI_TASK_1, work_correct_with_matrix_6_on_3) {
    size_t rows = 6, columns = 3;
    std::vector<int> matrix;
    std::vector<int> result1, result2;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix = fillRandomMatrix(rows, columns);
        result1 = findMaxesInMatrixRows(matrix, columns);
    }
    result2 = parallelFindMaxesInMatrixRows(matrix, rows, columns);
    if (procRank == 0) {
        ASSERT_EQ(result1, result2);
    }
}

TEST(MPI_TASK_1, work_correct_with_matrix_1_on_7) {
    size_t rows = 1, columns = 7;
    std::vector<int> matrix;
    std::vector<int> result1, result2;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix = fillRandomMatrix(rows, columns);
        result1 = findMaxesInMatrixRows(matrix, columns);
    }
    result2 = parallelFindMaxesInMatrixRows(matrix, rows, columns);
    if (procRank == 0) {
        ASSERT_EQ(result1, result2);
    }
}

TEST(MPI_TASK_1, work_correct_with_matrix_8_on_1) {
    size_t rows = 8, columns = 1;
    std::vector<int> matrix;
    std::vector<int> result1, result2;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix = fillRandomMatrix(rows, columns);
        result1 = findMaxesInMatrixRows(matrix, columns);
    }
    result2 = parallelFindMaxesInMatrixRows(matrix, rows, columns);
    if (procRank == 0) {
        ASSERT_EQ(result1, result2);
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
