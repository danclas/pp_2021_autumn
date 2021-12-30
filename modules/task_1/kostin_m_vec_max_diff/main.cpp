// Copyright 2021 Kostin Maksim
#include <gtest/gtest.h>
#include <vector>
#include <gtest-mpi-listener.hpp>
#include "./vec_max_diff.h"


TEST(Sequential_Operations, can_get_diff_with_positive_elements) {
  std::vector<int> vec = {1, 2, 3, 5, 6};

  int max = getSequentialMaxDiff(vec);
  ASSERT_EQ(max, 2);
}

TEST(Sequential_Operations, can_get_diff_with_negative_elements) {
  std::vector<int> vec = {-1, -2, -3, -6, -7};

  int max = getSequentialMaxDiff(vec);
  ASSERT_EQ(max, 3);
}

TEST(Sequential_Operations_MPI,
  can_get_diff_between_positive_and_negative_elements) {
  std::vector<int> vec = {1, -2, 4, 6, 7};

  int max = getSequentialMaxDiff(vec);
  ASSERT_EQ(max, 6);
}

TEST(Parallel_operations_MPI, sequential_and_parallel_are_equal) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;

  if (rank == 0) {
    vec = getRandomVector(30);
  }

  int p_max = getParallelMaxDiff(vec);

  if (rank == 0) {
    EXPECT_EQ(getSequentialMaxDiff(vec), p_max);
  }
}

TEST(Parallel_operations_MPI, sequential_and_parallel_are_equal_on_big_vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;

  if (rank == 0) {
    vec = getRandomVector(111);
  }

  int p_max = getParallelMaxDiff(vec);

  if (rank == 0) {
    EXPECT_EQ(getSequentialMaxDiff(vec), p_max);
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
