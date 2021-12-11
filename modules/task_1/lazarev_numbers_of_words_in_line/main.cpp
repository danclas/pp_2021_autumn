// Copyright 2021 Lazarev Alexey

#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "../../../modules/task_1/lazarev_numbers_of_words_in_line/word_count.h"

TEST(WordCount, without_space) {
  std::string st = "sdhsd sdh dsd dh sd";
  ASSERT_EQ(GetCountString(st), 5);
}

TEST(WordCount, with_a_space) {
  std::string st = "sdhsd sdh sdsdh sd ";
  ASSERT_EQ(GetCountString(st), 4);
}

TEST(WordCount, separation) {
  std::string st = "sdhsd!sdh#sdsdh)sd@";
  ASSERT_EQ(GetCountString(st), 1);
}

TEST(WordCount, empty) {
  std::string st = "";
  ASSERT_EQ(GetCountString(st), 0);
}

TEST(WordCount, space) {
  std::string st = " ";
  ASSERT_EQ(GetCountString(st), 0);
}

TEST(WordCount, parallel) {
  int ProcRank, ProcNum;
  int res = 0;
  std::string st;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  st = "sdhdf dfjdf drjd fffd dffg jg ";
  res = Count_pp(st);
  if (ProcRank == 0) {
    ASSERT_EQ(res, 6);
  }
}

TEST(WordCount, parallelCount) {
  int ProcRank, ProcNum;
  std::string st;
  int i = 0;
  int count = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  st = "sdggh gfjfg drr jfg ghj df";
  count = Count_pp(st);
  if (ProcRank == 0) {
    i = GetCountString(st);
    ASSERT_EQ(i, count);
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
