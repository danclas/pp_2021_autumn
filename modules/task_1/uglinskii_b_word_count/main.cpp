// Copyright 2021 Uglinskii Bogdan
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./word_count.h"

TEST(Sequential_Word_Count_MPI, Sirus_speech_seq) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcRank == 0) {
    std::string str =
        "Feel the thrill of THE VOID! Rain of Stars! Glorious Flames! "
        "Obliteration. You will long for NONEXISTENCE";
    int count = CountWordsSubstr(str);
    ASSERT_EQ(count, 17);
  }
}

TEST(Sequential_Word_Count_MPI, One_word_seq) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcRank == 0) {
    std::string str = "DIE!";
    int count = CountWordsSubstr(str);
    ASSERT_EQ(count, 1);
  }
}

TEST(Parallel_Word_Count_MPI, Sirus_speech_parallel) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::string str = "";
  if (ProcRank == 0) {
    str =
        "Feel the thrill of THE VOID! Rain of Stars! Glorious Flames! "
        "Obliteration. You will long for NONEXISTENCE";
  }

  int count = ParallelWordCount(str);
  if (ProcRank == 0) {
    ASSERT_EQ(count, 17);
  }
}
TEST(Parallel_Word_Count_MPI, One_word_parallel) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::string str = "";
  if (ProcRank == 0) {
    str = "DIE!";
  }

  int count = ParallelWordCount(str);

  if (ProcRank == 0) {
    ASSERT_EQ(count, 1);
  }
}

TEST(Parallel_Word_Count_MPI, Generated_small_String_parallel) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::string str = "";

  if (ProcRank == 0) {
    str = GenStr(7, 3);
  }

  int par_count = ParallelWordCount(str);

  if (ProcRank == 0) {
    int seq_count = CountWordsSubstr(str);
    ASSERT_EQ(seq_count, par_count);
  }
}

TEST(Parallel_Word_Count_MPI, Generated_big_String_parallel) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  std::string str = "";
  if (ProcRank == 0) {
    str = GenStr(10, 11);
  }

  int par_count = ParallelWordCount(str);

  if (ProcRank == 0) {
    int seq_count = CountWordsSubstr(str);
    ASSERT_EQ(seq_count, par_count);
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
