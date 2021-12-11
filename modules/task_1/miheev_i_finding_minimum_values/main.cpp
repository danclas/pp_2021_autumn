// Copyright 2021 Miheev Ivan
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./finding_minimum_values.h"

TEST(Sequential_Finding_Minimum_By_Matrix_Columns, Rows_More_Columns) {
  int currentProcess;
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

  if (currentProcess == 0) {
    std::vector<int> matrix = {2, 4, 0, -2, 1, 3, -1, 0, -10, 2, 4, 5};

    std::vector<int> globalMatrixMininimum =
        sequentialFindMin(matrix, 4, 3);
    std::vector<int> referenceMatrixMinimum = {-2, 0, -10};
    ASSERT_EQ(globalMatrixMininimum, referenceMatrixMinimum);
  }
}

TEST(Sequential_Finding_Minimum_By_Matrix_Columns, Rows_Less_Columns) {
  int currentProcess;
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

  if (currentProcess == 0) {
    std::vector<int> matrix = {-2, 4, 3, 1, 1, 3, -1, 0, 1, 2, -4, 2};

    std::vector<int> globalMatrixMininimum =
        sequentialFindMin(matrix, 3, 4);
    std::vector<int> referenceMatrixMinimum = {-2, 2, -4, 0};
    ASSERT_EQ(globalMatrixMininimum, referenceMatrixMinimum);
  }
}

TEST(Parallel_Finding_Minimum_By_Matrix_Columns, Rows_Equal_Columns) {
  int currentProcess;
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

  std::vector<int> matrix;
  const std::vector<int>::size_type matrixRows = 100;
  const std::vector<int>::size_type matrixColumns = 100;

  if (currentProcess == 0) {
    getMatrix(&matrix, matrixRows * matrixColumns);
  }

  std::vector<int> globalMatrixMininimum =
      parallelFindMin(matrix, matrixRows, matrixColumns);

  if (currentProcess == 0) {
    std::vector<int> referenceMatrixMinimum =
        sequentialFindMin(matrix, matrixRows, matrixColumns);
    ASSERT_EQ(globalMatrixMininimum, referenceMatrixMinimum);
  }
}

TEST(Parallel_Finding_Minimum_By_Matrix_Columns, Rows_More_Columns) {
  int currentProcess;
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

  std::vector<int> matrix;
  const std::vector<int>::size_type matrixRows = 200;
  const std::vector<int>::size_type matrixColumns = 100;

  if (currentProcess == 0) {
    getMatrix(&matrix, matrixRows * matrixColumns);
  }

  std::vector<int> globalMatrixMinimum =
      parallelFindMin(matrix, matrixRows, matrixColumns);

  if (currentProcess == 0) {
    std::vector<int> referenceMatrixMinimum =
        sequentialFindMin(matrix, matrixRows, matrixColumns);
    ASSERT_EQ(globalMatrixMinimum, referenceMatrixMinimum);
  }
}

TEST(Parallel_Finding_Minimum_By_Matrix_Columns, Rows_Less_Columns) {
  int currentProcess;
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

  std::vector<int> matrix;
  const std::vector<int>::size_type matrixRows = 100;
  const std::vector<int>::size_type matrixColumns = 200;

  if (currentProcess == 0) {
    getMatrix(&matrix, matrixRows * matrixColumns);
  }

  std::vector<int> globalMatrixMinimum =
      parallelFindMin(matrix, matrixRows, matrixColumns);

  if (currentProcess == 0) {
    std::vector<int> referenceMatrixMinimum =
        sequentialFindMin(matrix, matrixRows, matrixColumns);
    ASSERT_EQ(globalMatrixMinimum, referenceMatrixMinimum);
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
