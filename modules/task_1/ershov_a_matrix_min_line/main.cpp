// Copyright 2021 Ershov Aleksey
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_min_line_mpi.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Parallel_Algorithm) {
  int rank;
  const size_t n = 18;
  const size_t h = 6;
  const size_t w = 3;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> realResult;
  std::vector<double> result;
  std::vector<double> data;
  if (rank == 0) {
    for (size_t i = 0; i < n; ++i) {
      data.push_back(i);
    }
    realResult = {0, 3, 6, 9, 12, 15};
  }
  result = getParallelMatrixMinLine(data, h, w);
  if (rank == 0) {
    for (size_t i = 0; i < h; ++i) {
      ASSERT_EQ(result[i], realResult[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Sequential_Algorithm) {
  int rank;
  const size_t n = 18;
  const size_t h = 6;
  const size_t w = 3;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> realResult;
  std::vector<double> result;
  std::vector<double> data;
  if (rank == 0) {
    for (size_t i = 0; i < n; ++i) {
      data.push_back(i);
    }
    realResult = {0, 3, 6, 9, 12, 15};
  }
  if (rank == 0) {
    result = getSequentialMatrixMinLine(data, h, w);
    for (size_t i = 0; i < h; ++i) {
      ASSERT_EQ(result[i], realResult[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Min_Algorithm) {
  int rank;
  const size_t h = 60;
  const size_t w = 60;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> parallelResult;
  std::vector<double> sequentialResult;
  std::vector<double> data;
  if (rank == 0) {
    data = getRandomVector(h, w);
  }
  parallelResult = getParallelMatrixMinLine(data, h, w);
  if (rank == 0) {
    sequentialResult = getSequentialMatrixMinLine(data, h, w);
    for (size_t i = 0; i < h; ++i) {
      ASSERT_EQ(sequentialResult[i], parallelResult[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Big_Row_Matrix) {
  int rank;
  const size_t h = 1000;
  const size_t w = 50;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> parallelResult;
  std::vector<double> sequentialResult;
  std::vector<double> data;
  if (rank == 0) {
    data = getRandomVector(h, w);
  }
  parallelResult = getParallelMatrixMinLine(data, h, w);
  if (rank == 0) {
    sequentialResult = getSequentialMatrixMinLine(data, h, w);
    for (size_t i = 0; i < h; ++i) {
      ASSERT_EQ(sequentialResult[i], parallelResult[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Big_Collums_Matrix) {
  int rank;
  const size_t h = 50;
  const size_t w = 1000;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> parallelResult;
  std::vector<double> sequentialResult;
  std::vector<double> data;
  if (rank == 0) {
    data = getRandomVector(h, w);
  }
  parallelResult = getParallelMatrixMinLine(data, h, w);
  if (rank == 0) {
    sequentialResult = getSequentialMatrixMinLine(data, h, w);
    for (size_t i = 0; i < h; ++i) {
      ASSERT_EQ(sequentialResult[i], parallelResult[i]);
    }
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
