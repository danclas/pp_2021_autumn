// Copyright 2021 Dydykin Pavel
#include <gtest/gtest.h>
#include "../../../modules/task_2/dydykin_p_gauss_horizontal/gauss_horizontal.h"
#include <gtest-mpi-listener.hpp>


TEST(Test_Gauss_Horizontal, Test1_Gauss_NotParallel) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<double> matrix = {
            1, -2, 3, 2,
            3, 1, -1, 3,
            2, 5, 2, 9
    };

    const int rows = 3;
    const int cols = 4;

    if (ProcRank == 0) {
        std::vector <double> result = GaussNotParallel(matrix, rows, cols);
        std::vector <double> x = { 1.0, 1.0, 1.0 };
        for (int i = 0; i < rows; i++)
            ASSERT_NEAR(x[i], result[i], 0.000001);
    }
}

TEST(Test_Gauss_Horizontal, Test2_Gauss_Parallel) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<double> matrix = {
            2, 4, 1, 36,
            5, 2, 1, 47,
            2, 3, 4, 37
    };

    const int rows = 3;
    const int cols = 4;


    std::vector <double> result = GaussParallel(matrix, rows, cols);

    if (ProcRank == 0) {
        std::vector <double> x = { 7.0, 5.0, 2.0 };
        for (int i = 0; i < rows; i++)
            ASSERT_NEAR(x[i], result[i], 0.000001);
    }
}


TEST(Test_Gauss_Horizontal, Test3_GaussParallel_and_GaussNotParallel) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<double> matrix = {
            1, 2, 3, 1,
            2, -1, 2, 6,
            1, 1, 5, -1
    };

    const int rows = 3;
    const int cols = 4;

    std::vector <double> result = GaussParallel(matrix, rows, cols);

    if (ProcRank == 0) {
         std::vector <double> x = GaussNotParallel(matrix, rows, cols);
         for (int i = 0; i < rows; i++) {
             ASSERT_NEAR(x[i], result[i], 0.000001);
         }
    }
}
TEST(Test_Gauss_Horizontal, Test4_GaussParallel_and_GaussNotParallel) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<double> matrix = {
            1, 2, 5, -9,
            1, -1, 3, 2,
            3, -6, -3, 25
    };

    const int rows = 3;
    const int cols = 4;

    std::vector <double> result = GaussParallel(matrix, rows, cols);

    if (ProcRank == 0) {
        std::vector <double> x = GaussNotParallel(matrix, rows, cols);
        for (int i = 0; i < rows; i++)
            ASSERT_NEAR(x[i], result[i], 0.000001);
    }
}

TEST(Test_Gauss_Horizontal, Test5_GaussParallel_and_GaussNotParallel) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<double> matrix = {
            4, 8, 1, 13,
            2, 1, 1, 4,
            1, 1, 2, 4
    };

    const int rows = 3;
    const int cols = 4;

    std::vector <double> result = GaussParallel(matrix, rows, cols);

    if (ProcRank == 0) {
        std::vector <double> x = GaussNotParallel(matrix, rows, cols);
        for (int i = 0; i < rows; i++)
            ASSERT_NEAR(x[i], result[i], 0.000001);
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
