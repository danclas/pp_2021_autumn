//  Copyright 2021 Elanskiy Akexandr
#include "./max_values_in_rows.h"

TEST(MPI_TEST, size_1x10) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 1;
    int columns = 10;
    std::vector<int> matrix;
    std::vector<int> res_seq;
    std::vector<int> res_par;
    matrix = mat_gen(rows, columns);
    res_par = max_in_rows_par(matrix, rows, columns);
    if (procRank == 0) {
        res_seq = max_in_rows_seq(matrix, columns);
        ASSERT_EQ(res_seq, res_par);
    }
}

TEST(MPI_TEST, size_13x37) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 13;
    int columns = 37;
    std::vector<int> matrix;
    std::vector<int> res_seq;
    std::vector<int> res_par;
    matrix = mat_gen(rows, columns);
    res_par = max_in_rows_par(matrix, rows, columns);
    if (procRank == 0) {
        res_seq = max_in_rows_seq(matrix, columns);
        ASSERT_EQ(res_seq, res_par);
    }
}

TEST(MPI_TEST, size_20x20) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 20;
    int columns = 20;
    std::vector<int> matrix;
    std::vector<int> res_seq;
    std::vector<int> res_par;
    matrix = mat_gen(rows, columns);
    res_par = max_in_rows_par(matrix, rows, columns);
    if (procRank == 0) {
        res_seq = max_in_rows_seq(matrix, columns);
        ASSERT_EQ(res_seq, res_par);
    }
}

TEST(MPI_TEST, size_20x77) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 20;
    int columns = 77;
    std::vector<int> matrix;
    std::vector<int> res_seq;
    std::vector<int> res_par;
    matrix = mat_gen(rows, columns);
    res_par = max_in_rows_par(matrix, rows, columns);
    if (procRank == 0) {
        res_seq = max_in_rows_seq(matrix, columns);
        ASSERT_EQ(res_seq, res_par);
    }
}

TEST(MPI_TEST, size_37x5) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 37;
    int columns = 5;
    std::vector<int> matrix;
    std::vector<int> res_seq;
    std::vector<int> res_par;
    matrix = mat_gen(rows, columns);
    res_par = max_in_rows_par(matrix, rows, columns);
    if (procRank == 0) {
        res_seq = max_in_rows_seq(matrix, columns);
        ASSERT_EQ(res_seq, res_par);
    }
}

TEST(MPI_TEST, size_26x11) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 26;
    int columns = 11;
    std::vector<int> matrix;
    std::vector<int> res_seq;
    std::vector<int> res_par;
    matrix = mat_gen(rows, columns);
    res_par = max_in_rows_par(matrix, rows, columns);
    if (procRank == 0) {
        res_seq = max_in_rows_seq(matrix, columns);
        ASSERT_EQ(res_seq, res_par);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    std::vector<int> result1;
    std::vector<int> result2;
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());
    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
