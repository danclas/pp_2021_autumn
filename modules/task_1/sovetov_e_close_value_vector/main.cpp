// Copyright 2021 Sovetov Egor
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "../../../modules/task_1/sovetov_e_close_value_vector/close_value_vector.h"
#include <gtest-mpi-listener.hpp>



TEST(Test_close_value_vector, Test_close_value_one) {
  int bee;
  MPI_Comm_rank(MPI_COMM_WORLD, &bee);
  std::vector<int> wektor = {0, 6, 8, 16, 20, 30};
  if (bee == 0) {
    int minDfference = getMinDifferenceOneProc(wektor);
    ASSERT_EQ(minDfference, 2);
  }
}

TEST(Test_close_value_vector, Test_close_value_one_oneElem) {
  int bee;
  MPI_Comm_rank(MPI_COMM_WORLD, &bee);
  std::vector<int> wektor = {3};
  if (bee == 0) {
    int minDfference = getMinDifferenceOneProc(wektor);
    ASSERT_EQ(minDfference, 0);
  }
}

TEST(Test_close_value_vector, Test_close_value_oneElem) {
  int bee;
  MPI_Comm_rank(MPI_COMM_WORLD, &bee);
  std::vector<int> wektor;
  wektor = getRandomVector(1);
  int minDfference = getMinDifference(wektor);
  if (bee == 0) {
    ASSERT_EQ(minDfference, getMinDifferenceOneProc(wektor));
  }
}

TEST(Test_close_value_vector, Test_close_value_10Elem) {
  int bee;
  MPI_Comm_rank(MPI_COMM_WORLD, &bee);
  std::vector<int> wektor;
  wektor = getRandomVector(10);
  int minDfference = getMinDifference(wektor);
  if (bee == 0) {
    ASSERT_EQ(minDfference, getMinDifferenceOneProc(wektor));
  }
}

TEST(Test_close_value_vector, Test_close_value_50Elem) {
  int bee;
  MPI_Comm_rank(MPI_COMM_WORLD, &bee);
  std::vector<int> wektor;
  wektor = getRandomVector(50);
  int minDfference = getMinDifference(wektor);
  if (bee == 0) {
    ASSERT_EQ(minDfference, getMinDifferenceOneProc(wektor));
  }
}

TEST(Test_close_value_vector, Test_close_value_100Elem) {
  int bee;
  MPI_Comm_rank(MPI_COMM_WORLD, &bee);
  std::vector<int> wektor;
  wektor = getRandomVector(100);
  int minDfference = getMinDifference(wektor);
  if (bee == 0) {
    ASSERT_EQ(minDfference, getMinDifferenceOneProc(wektor));
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
