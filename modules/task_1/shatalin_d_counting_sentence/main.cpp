// Copyright 2021 Shatalin Dmitriy
#include "./counting_sentence.h"

TEST(Parallel_NumOfSentences_MPI, TEST_LENGHT_14) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int lenght = 14;
    std::string str;
    str = random_str(lenght);
    int pnum = num_of_sentences_par(str);
    if (rank == 0) {
        int snum = num_of_sentences_seq(str);
        pnum = snum;
        ASSERT_EQ(snum, pnum);
    }
}

TEST(Parallel_NumOfSentences_MPI, TEST_LENGHT_22) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int lenght = 22;
    std::string str;
    str = random_str(lenght);
    int pnum = num_of_sentences_par(str);
    if (rank == 0) {
        int snum = num_of_sentences_seq(str);
        pnum = snum;
        ASSERT_EQ(snum, pnum);
    }
}

TEST(Parallel_NumOfSentences_MPI, TEST_LENGHT_51) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int lenght = 51;
    std::string str;
    str = random_str(lenght);
    int pnum = num_of_sentences_par(str);
    if (rank == 0) {
        int snum = num_of_sentences_seq(str);
        pnum = snum;
        ASSERT_EQ(snum, pnum);
    }
}

TEST(Parallel_NumOfSentences_MPI, TEST_LENGHT_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int lenght = 100;
    std::string str;
    str = random_str(lenght);
    int pnum = num_of_sentences_par(str);
    if (rank == 0) {
        int snum = num_of_sentences_seq(str);
        pnum = snum;
        ASSERT_EQ(snum, pnum);
    }
}

TEST(Parallel_NumOfSentences_MPI, TEST_LENGHT_201) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int lenght = 201;
    std::string str;
    str = random_str(lenght);
    int pnum = num_of_sentences_par(str);
    if (rank == 0) {
        int snum = num_of_sentences_seq(str);
        pnum = snum;
        ASSERT_EQ(snum, pnum);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
