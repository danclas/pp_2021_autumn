// Copyright 2021 Cheremushkin Kirill
#include <gtest/gtest.h>
#include <vector>
#include "./count_words.h"
#include <gtest-mpi-listener.hpp>
#include "string"
TEST(Single_tests, Counting_words_linearly_in_the_first_line) {
    int ProcRank;  // Counting words linearly in the first line
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        std::string text;
        text = create_sting(0);
        int expected_count = 7;
        int actual_count = counting(text);
        ASSERT_EQ(expected_count, actual_count);
    }
}

TEST(Single_tests, Counting_words_linearly_in_the_second_line) {
    int ProcRank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        std::string text;
        text = create_sting(1);
        int expected_count = 3;
        int actual_count = counting(text);
        ASSERT_EQ(expected_count, actual_count);
    }
}

TEST(Parallel_Tests, Word_counting_in_parallel_mode_first_line) {
    int ProcRank;  // word counting in parallel mode first line
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::string text;
    if (ProcRank == 0)
        text = create_sting(0);
    int actual_count = parallel_counting(text);
    if (ProcRank == 0) {
        int expected_count = counting(text);
        ASSERT_EQ(expected_count, actual_count);
    }
}

TEST(Parallel_Tests, Word_counting_in_parallel_mode_second_line) {
    int ProcRank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::string text;
    if (ProcRank == 0) {
        text = create_sting(1);
    }
    int actual_count = parallel_counting(text);
    if (ProcRank == 0) {
        int expected_count = counting(text);
        ASSERT_EQ(expected_count, actual_count);
    }
}

TEST(Parallel_Tests, Word_counting_in_parallel_mode_second_plus_first_line) {
    int ProcRank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::string text;
    if (ProcRank == 0) {
        text = create_sting(0) + " " + create_sting(1);
    }
    int actual_count = parallel_counting(text);
    if (ProcRank == 0) {
        int expected_count = counting(text);
        ASSERT_EQ(expected_count, actual_count);
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
