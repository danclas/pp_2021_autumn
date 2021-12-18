// Copyright 2021 Krivosheev Miron

#include <gtest/gtest.h>
#include <iostream>
#include <ctime>
#include <random>
#include "./symbol_frequency.h"
#include <gtest-mpi-listener.hpp>

std::string makeRandomString(int size = 10) {
  std::string S(size, '0');
  std::mt19937 gen(time(0));
  std::uniform_int_distribution<> up(65, 90);
  std::uniform_int_distribution<> low(97, 122);
  for (int i = 0; i < size; ++i) {
    if (up(gen) % 3 == 0) {
      S[i] = static_cast<char>(up(gen));
    } else if (up(gen) % 3 == 1) {
      S[i] = static_cast<char>(low(gen));
    } else {
      S[i] = ' ';
    }
  }
  return S;
}

TEST(Non_Parallel_Operation, non_parallel_test_func) {
  std::string s = "The recvcount and recvtype parameters are ignored."
    "The scattered vector is still considered to contain n segments,"
    "where n is the group size; the segment that corresponds to the root"
    "process is not moved";
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  char symbol = 'c';
  double freq = getFrequencyNonParallel(symbol, s);
  if (my_rank == 0)
    ASSERT_NEAR(freq, 0.045, 0.0000001);
}

TEST(Parallel_Operation, parallel_test_func) {
  std::string s = "The recvcount and recvtype parameters are ignored."
    "The scattered vector is still considered to contain n segments,"
    "where n is the group size; the segment that corresponds to the root"
    "process is not moved";
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  char symbol = 'c';
  double freq_expect = getFrequencyNonParallel(symbol, s);
  double freq_par = getFrequencyParallel(symbol, s);
  if (my_rank == 0)
    ASSERT_NEAR(freq_expect, freq_par, 0.0000001);
}

TEST(Parallel_Operation, parallel_test_func_uppercase) {
  std::string s = "THE RECVCOUNT AND RECVTYPE PARAMETERS ARE IGNORED."
  "THE SCATTERED VECTOR IS STILL CONSIDERED TO CONTAIN N SEGMENTSL, WHERE"
  "N IS THE GROUP SIZE; THE SEGMENT THAT CORRESPONDS TO THE ROOT PROCESS"
  "IS NOT MOVED";
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  char symbol = 'c';
  double freq_expect = getFrequencyNonParallel(symbol, s);
  double freq_par = getFrequencyParallel(symbol, s);
  if (my_rank == 0)
    ASSERT_NEAR(freq_expect, freq_par, 0.0000001);
}

TEST(Parallel_Operation, parallel_test_func_lowercase) {
  std::string s = "the recvcount and recvtype parameters are ignored."
    "the scattered vector is still considered to contain n segments,"
    "where n is the group size; the segment that corresponds to the root"
    "process is not moved";
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  char symbol = 'c';
  double freq_expect = getFrequencyNonParallel(symbol, s);
  double freq_par = getFrequencyParallel(symbol, s);
  if (my_rank == 0)
    ASSERT_NEAR(freq_expect, freq_par, 0.0000001);
}

TEST(Parallel_Operation, parallel_test_func_random) {
  std::string s = makeRandomString(30);
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  char symbol = 'c';
  double freq_expect = getFrequencyNonParallel(symbol, s);
  double freq_par = getFrequencyParallel(symbol, s);
  if (my_rank == 0)
    ASSERT_NEAR(freq_expect, freq_par, 0.0000001);
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
