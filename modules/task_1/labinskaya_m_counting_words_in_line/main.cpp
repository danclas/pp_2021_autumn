// Copyright 2021 Labinskaya Marina
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./counting_words_in_line.h"

TEST(TEST_SEQ, sequental_counting_words_with_empty_str) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        std::string str = "";

        int correct_res = 0;
        int res = sequential_counting_words(str);
        ASSERT_EQ(correct_res, res);
    }
}

TEST(TEST_SEQ, sequental_counting_words_with_empty_str_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        std::string str = "    ";

        int correct_res = 0;
        int res = sequential_counting_words(str);
        ASSERT_EQ(correct_res, res);
    }
}

TEST(TEST_SEQ, sequental_counting_words_without_dash) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        std::string str = "London is the capital of Great Britain";
        int correct_res = 7;
        int res = sequential_counting_words(str);

        ASSERT_EQ(correct_res, res);
    }
}

TEST(TEST_SEQ, sequental_counting_words_with_dash) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        std::string str = "Good weather at the resort - a successful component of a vacation";

        int correct_res = 11;
        int res = sequential_counting_words(str);
        ASSERT_EQ(correct_res, res);
    }
}

TEST(TEST_PARALLEL, parallel_counting_words_with_empty_str) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::string str;
    if (ProcRank == 0) {
        str = "";
    }

    int res = parallel_counting_words(str);
    if (ProcRank == 0) {
        int correct_res = 0;
        ASSERT_EQ(correct_res, res);
    }
}

TEST(TEST_PARALLEL, parallel_counting_words_with_empty_str_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::string str;
    if (ProcRank == 0) {
        str = "    ";
    }

    int res = parallel_counting_words(str);
    if (ProcRank == 0) {
        int correct_res = 0;
        ASSERT_EQ(correct_res, res);
    }
}

TEST(TEST_PARALLEL, parallel_counting_words_without_dash) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::string str;
    if (ProcRank == 0) {
        str = "London is the capital of Great Britain";
    }

    int res = parallel_counting_words(str);
    if (ProcRank == 0) {
        int correct_res = sequential_counting_words(str);
        ASSERT_EQ(correct_res, res);
    }
}

TEST(TEST_PARALLEL, parallel_counting_words_with_dash) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::string str;
    if (ProcRank == 0) {
        str = "Good weather at the resort - a successful component of a vacation";
    }

    int res = parallel_counting_words(str);
    if (ProcRank == 0) {
        int correct_res = sequential_counting_words(str);
        ASSERT_EQ(correct_res, res);
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
