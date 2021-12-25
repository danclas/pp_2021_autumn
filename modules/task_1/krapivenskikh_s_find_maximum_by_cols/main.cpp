// Copyright 2021 Krapivenskikh Stepan
#include <gtest/gtest.h>
#include <vector>
#include "./find_maximum_by_cols.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Function_MPI, Test_Find_Min_By_Cols_100x100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix;
    const int matrixRows = 100;
    const int matrixCols = 100;

    if (rank == 0) {
        global_matrix = getRandomMatrix(matrixRows * matrixCols);
    }

    std::vector<int> global_max = getParallelFunction(global_matrix, matrixRows, matrixCols);

    if (rank == 0) {
        std::vector<int> reference_max = getSequentialFunction(global_matrix, matrixRows, matrixCols);
        ASSERT_EQ(global_max, reference_max);
    }
}

TEST(Parallel_Function_MPI, Test_Find_Min_By_Cols_150_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix;
    const int matrixRows = 150;
    const int matrixCols = 100;

    if (rank == 0) {
        global_matrix = getRandomMatrix(matrixRows * matrixCols);
    }

    std::vector<int> global_max = getParallelFunction(global_matrix, matrixRows, matrixCols);

    if (rank == 0) {
        std::vector<int> reference_max = getSequentialFunction(global_matrix, matrixRows, matrixCols);
        ASSERT_EQ(global_max, reference_max);
    }
}

TEST(Parallel_Function_MPI, Test_Find_Min_By_Cols_100x150) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix;
    const int matrixRows = 100;
    const int matrixCols = 150;

    if (rank == 0) {
        global_matrix = getRandomMatrix(matrixRows * matrixCols);
    }

    std::vector<int> global_max = getParallelFunction(global_matrix, matrixRows, matrixCols);

    if (rank == 0) {
        std::vector<int> reference_max = getSequentialFunction(global_matrix, matrixRows, matrixCols);
        ASSERT_EQ(global_max, reference_max);
    }
}

TEST(Parallel_Function_MPI, Test_Find_Min_By_Cols_150x150) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix;
    const int matrixRows = 150;
    const int matrixCols = 150;

    if (rank == 0) {
        global_matrix = getRandomMatrix(matrixRows * matrixCols);
    }

    std::vector<int> global_max = getParallelFunction(global_matrix, matrixRows, matrixCols);

    if (rank == 0) {
        std::vector<int> reference_max = getSequentialFunction(global_matrix, matrixRows, matrixCols);
        ASSERT_EQ(global_max, reference_max);
    }
}

TEST(Parallel_Function_MPI, Test_Find_Min_By_Cols_200x150) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_matrix;
    const int matrixRows = 200;
    const int matrixCols = 150;

    if (rank == 0) {
        global_matrix = getRandomMatrix(matrixRows * matrixCols);
    }

    std::vector<int> global_max = getParallelFunction(global_matrix, matrixRows, matrixCols);

    if (rank == 0) {
        std::vector<int> reference_max = getSequentialFunction(global_matrix, matrixRows, matrixCols);
        ASSERT_EQ(global_max, reference_max);
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
