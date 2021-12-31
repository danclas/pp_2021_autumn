// Copyright 2021 Kostin Maksim
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./ribbon_horizontal_scheme.h"

TEST(Sequential_Operations, can_multiply_matrices_right) {
  std::vector<int> A = {4, 5, 6};
  std::vector<int> B = {1, 2, 3};
  std::vector<int> test_C = {4, 8, 12, 5, 10, 15, 6, 12, 18};
  std::vector<int> C(9);
  C = SequentialMatrixMultiplication(A, 3, 1, B, 1, 3);

  EXPECT_EQ(C, test_C);
}

TEST(Sequential_Operations,
  sequential_cant_multiply_matrices_if_col_a_and_row_b_not_equal) {
  std::vector<int> A(20);
  std::vector<int> B(20);
  std::vector<int> C;

  ASSERT_ANY_THROW(C = SequentialMatrixMultiplication(A, 4, 5, B, 4, 5));
}

TEST(Parallel_Operations,
  parallel_cant_multiply_matrices_if_col_a_and_row_b_not_equal) {
  std::vector<int> A(20);
  std::vector<int> B(20);
  std::vector<int> C;

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    ASSERT_ANY_THROW(C = ParallelMatrixMultiplication(A, 4, 5, B, 4, 5));
  }
}

TEST(Parallel_Operations, can_multiply_square_matrices) {
  std::vector<int> A(9), B(9);
  A = getRandomMatrix(3, 3);
  B = getRandomMatrix(3, 3);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> C;
  C = ParallelMatrixMultiplication(A, 3, 3, B, 3, 3);

  if (rank == 0) {
    std::vector<int> Cs = SequentialMatrixMultiplication(A, 3, 3, B, 3, 3);
    EXPECT_EQ(C, Cs);
  }
}

TEST(Parallel_Operations, sequential_and_parallel_are_equal) {
  std::vector<int> A = {4, 5, 6};
  std::vector<int> B = {1, 2, 3};
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> C_parallel = ParallelMatrixMultiplication(A, 3, 1, B, 1, 3);

  if (rank == 0) {
    std::vector<int> C_sequential(9);
    C_sequential = SequentialMatrixMultiplication(A, 3, 1, B, 1, 3);

    EXPECT_EQ(C_sequential, C_parallel);
  }
}

TEST(Parallel_Operations, can_multiply_300x100_on_100x400) {
  std::vector<int> A = getRandomMatrix(300, 100);
  std::vector<int> B = getRandomMatrix(100, 400);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> C_parallel =
      ParallelMatrixMultiplication(A, 300, 100, B, 100, 400);

  if (rank == 0) {
    std::vector<int> C_sequential(300 * 400);
    C_sequential = SequentialMatrixMultiplication(A, 300, 100, B, 100, 400);

    EXPECT_EQ(C_sequential, C_parallel);
  }
}

TEST(Parallel_Operations, can_multiply_33x12_on_12x35) {
  std::vector<int> A = getRandomMatrix(33, 12);
  std::vector<int> B = getRandomMatrix(12, 35);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> C_parallel = ParallelMatrixMultiplication(A, 33, 12,
    B, 12, 35);

  if (rank == 0) {
    std::vector<int> C_sequential(33 * 35);
    C_sequential = SequentialMatrixMultiplication(A, 33, 12, B, 12, 35);

    EXPECT_EQ(C_sequential, C_parallel);
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
