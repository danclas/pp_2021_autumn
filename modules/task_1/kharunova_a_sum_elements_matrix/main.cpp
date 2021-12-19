// Copyright 2021 Kharunova Alina
#include <gtest/gtest.h>

#include "../../../modules/task_1/kharunova_a_sum_elements_matrix/sum_elements_matrix.h"

#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, parallel_sum_in_100_elements_matrix) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  const int n = 10;
  const int m = 10;
  int* mtrx = nullptr;
  int pSum = 0, lSum = 0;

  if (ProcRank == 0) {
    mtrx = new int[n * m];
    for (int i = 0; i < n * m; i++) {
      mtrx[i] = i;
    }
  }

  pSum = paralSum(mtrx, n, m);
  if (ProcRank == 0) {
    lSum = linSum(mtrx, n * m);
    ASSERT_EQ(pSum, lSum);
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_vector) {
  int ProcRank = 0;
  const int n = 1;
  const int m = 100;
  int* mtrx = 0;
  int pSum = 0, lSum = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    mtrx = new int[n * m];
    for (int i = 0; i < n * m; i++) {
      mtrx[i] = i;
    }
  }
  pSum = paralSum(mtrx, n, m);
  if (ProcRank == 0) {
    lSum = linSum(mtrx, n * m);
    delete[] mtrx;
    ASSERT_EQ(pSum, lSum);
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_1000000_elements_matrix) {
  int ProcRank = 0;
  const int n = 1000;
  const int m = 1000;
  int* mtrx = 0;
  int pSum = 0, lSum = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    mtrx = new int[n * m];
    for (int i = 0; i < n * m; i++) {
      mtrx[i] = i;
    }
  }
  pSum = paralSum(mtrx, n, m);
  if (ProcRank == 0) {
    lSum = linSum(mtrx, n * m);
    delete[] mtrx;
    ASSERT_EQ(pSum, lSum);
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_random_vector) {
  int ProcRank = 0;
  const int n = 1000;
  const int m = 1;
  int* mtrx = 0;
  int pSum = 0, lSum = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    mtrx = createRandomMatrix(n * m);
  }
  pSum = paralSum(mtrx, n, m);
  if (ProcRank == 0) {
    lSum = linSum(mtrx, n * m);
    delete[] mtrx;
    ASSERT_EQ(pSum, lSum);
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_random_1000000_elements_matrix) {
  int ProcRank = 0;
  const int n = 1000;
  const int m = 1000;
  int* mtrx = 0;
  int pSum = 0, lSum = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    mtrx = createRandomMatrix(n * m);
  }
  pSum = paralSum(mtrx, n, m);
  if (ProcRank == 0) {
    lSum = linSum(mtrx, n * m);
    delete[] mtrx;
    ASSERT_EQ(pSum, lSum);
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

