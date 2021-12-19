// Copyright 2021 Petrova Polina
#include <gtest/gtest.h>
#include <vector>
#include"./simple_iteration_method.h"
#include <gtest-mpi-listener.hpp>

TEST(simple_iteration, can_create_random_vector_0) {
    EXPECT_NO_THROW(std::vector<double> a = fillRandomVector(100));
}
TEST(simple_iteration, can_create_random_vector_1) {
    EXPECT_ANY_THROW(std::vector<double> a = fillRandomVector(-777));
}
TEST(simple_iteration, can_create_random_matrix_0) {
    EXPECT_NO_THROW(std::vector< std::vector<double> > a = fillRandomMatrix(100));
}
TEST(simple_iteration, can_create_random_matrix_1) {
    EXPECT_ANY_THROW(std::vector< std::vector<double> > a =
        fillRandomMatrix(-777));
}
TEST(simple_iteration, test_0) {
    std::vector< std::vector<double>> A{
        {4, 1, 1},
        {1, 6, -1},
        {1, 2, 5}
    };
    std::vector<double> b{ 9, 10, 20 };
    std::vector<double> x;
    std::vector<double> ans{ 1.0001, 2, 3.0001 };
    x = seqMethod(A, b, 3);
    for (int i = 0; i < 3; i++) {
        EXPECT_NEAR(x[i], ans[i], 0.002);
    }
}
TEST(simple_iteration, test_1) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector< std::vector<double>> A{
        {4, 1, 1},
        {1, 6, -1},
        {1, 2, 5}
    };
    std::vector<double> b{ 9, 10, 20 };
    std::vector<double> x;
    std::vector<double> ans{ 1.0001, 2, 3.0001 };
    x = parallelMethod(A, b, 3);
    if (procRank == 0) {
        for (int i = 0; i < 3; i++)
            EXPECT_NEAR(x[i], ans[i], 5.002);
    }
}
TEST(simple_iteration, test_2) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<std::vector<double> > M{
        {10, 1, 1, 1},
        {2, 10, 1, 1},
        {2, 2, 10, 1},
        {2, 2, 2, 10}
    };
    std::vector<double> b{ 13, 14, 15, 16 };
    std::vector<double> x = parallelMethod(M, b, 4);
    std::vector<double> seqX;
    if (procRank == 0) {
        seqX = seqMethod(M, b, 4);
        for (int i = 0; i < 3; i++)
            EXPECT_NEAR(x[i], seqX[i], 2.002);
    }
}
TEST(simple_iteration, test_3) {
    std::vector< std::vector<double>> A{
    {4, 1, 1},
    {1, 6, -1},
    {1, 2, 5}
    };
    std::vector<double> b{ 9, 10, 20 };
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<double> x = parallelMethod(A, b, 3);
    if (procRank == 0) {
        std::vector<double> seqX = seqMethod(A, b, 3);
        for (int i = 0; i < 3; i++)
            EXPECT_NEAR(x[i], seqX[i], 5.002);
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
