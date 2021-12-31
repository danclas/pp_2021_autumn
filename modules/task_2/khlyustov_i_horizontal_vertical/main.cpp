// Copyright 2021 Khlyustov Ilya
#include <gtest/gtest.h>
#include "./horizontal_vertical.h"
#include <gtest-mpi-listener.hpp>

TEST(Horizontal_Vertical_MPI, Generate_Matrix) {
    ASSERT_NO_THROW(getRandomMatrix(10, 15));
}

TEST(Horizontal_Vertical_MPI, Sequential_works_correctly_with_more_rows) {
    Matrix matrix1 = { 1, 2,
                       3, 4,
                       5, 6 };
    Matrix matrix2 = { 7, 8, 9,
                       0, 1, 2 };
    Matrix res = { 7, 10, 13,
                   21, 28, 35,
                   35, 46, 57 };
    ASSERT_EQ(res, getSequential(matrix1, matrix2, 3, 2, 3));
}

TEST(Horizontal_Vertical_MPI, Sequential_works_correctly_with_more_columns) {
    Matrix matrix1 = { 1, 2, 3,
                       1, 2, 3 };
    Matrix matrix2 = { 4, 4,
                       5, 5,
                       6, 6 };
    Matrix res = { 32, 32,
                   32, 32 };
    ASSERT_EQ(res, getSequential(matrix1, matrix2, 2, 3, 2));
}

TEST(Horizontal_Vertical_MPI, Parallel_works_correctly_with_more_columns) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    sizeType row_count = 20, col_count = 30;
    Matrix mtrx1(row_count * col_count), mtrx2(row_count * col_count);

    if (proc_rank == 0) {
        mtrx1 = getRandomMatrix(row_count, col_count);
        mtrx2 = getRandomMatrix(col_count, row_count);
    }

    Matrix par = getParallel(mtrx1, mtrx2, row_count, col_count);

    if (proc_rank == 0) {
        Matrix seq = getSequential(mtrx1, mtrx2, row_count, col_count, row_count);
        ASSERT_EQ(par, seq);
    }
}

TEST(Horizontal_Vertical_MPI, Parallel_works_correctly_with_more_rows) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    Matrix::size_type row_count = 30, column_count = 20;
    Matrix matrix1(row_count * column_count), matrix2(row_count * column_count);

    if (proc_rank == 0) {
        matrix1 = getRandomMatrix(row_count, column_count);
        matrix2 = getRandomMatrix(column_count, row_count);
    }

    Matrix res = getParallel(matrix1, matrix2, row_count, column_count);

    if (proc_rank == 0) {
        Matrix expected_res = getSequential(matrix1, matrix2, row_count, column_count, row_count);

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
