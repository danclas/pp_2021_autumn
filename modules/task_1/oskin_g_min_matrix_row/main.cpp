// Copyright 2021 Oskin Georgii
#include <gtest/gtest.h>
#include <vector>
#include "./min_matrix_row.h"
#include <gtest-mpi-listener.hpp>
TEST(Parallel_Operations_MPI, getlocalmin_test) {
  int n = 5;
  std::vector<int> vector = {1, 7, 9, 8, 6};
  int res = getLocalMinimum(vector, n);
  ASSERT_EQ(1, res);
}

TEST(Parallel_Operations_MPI, getlocalmin_test_more) {
  int n = 5;
  std::vector<int> vector = {2, 2, 5, 8, 6};
  int res = getLocalMinimum(vector, n);
  ASSERT_EQ(2, res);
}

TEST(Parallel_Operations_MPI, getlocalmin_test_single) {
  int n = 5;
  std::vector<int> vector = {2};
  int res = getLocalMinimum(vector, n);
  ASSERT_EQ(2, res);
}

TEST(Parallel_Operations_MPI, findmin_nonparall) {
  int m = 20, n = 20;
  std::vector<int> true_res(m);
  std::vector<std::vector<int>> g_matrix;
  g_matrix = getRandMatrix(m, n);
  findMinNonParallel(g_matrix, m, n, true_res);
}

TEST(Parallel_Operations_MPI, findmin_parall) {
  int m = 20, n = 20;
  std::vector<std::vector<int>> g_matrix(m, std::vector<int>(n));
  g_matrix = getRandMatrix(m, n);
  std::vector<int> res(m);
  for (int i = 0; i < n; i++) {
    res[i] = 0;
  }
  std::vector<int> true_res(m);
  findMinNonParallel(g_matrix, m, n, true_res);
  getParallelOperations(g_matrix, m, n, res);
  for (int i = 0; i < m; i++) {
    ASSERT_EQ(true_res[i], res[i]);
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
