// Copyright 2021 Alimov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./ne_poradok_vector.h"
#include <gtest-mpi-listener.hpp>

TEST(MPI_Allreduce_1, normal) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n = 5;
  int a[n] = {
      1, 2, 3, 4, 5,
  };
  int b;
  b = fun(a, n);
  if (rank == 0) {
    ASSERT_EQ(b, 0);
  }
}
TEST(MPI_Allreduce_2, wrong_last) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n = 5;
  int a[n] = {1, 2, 3, 4, 3};
  int b;
  b = fun(a, n);
  if (rank == 0) {
    ASSERT_EQ(b, 1);
  }
}

TEST(MPI_Allreduce_3, wrong_midle) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n = 5;
  int a[n] = {
      2, 3, 1, 4, 5,
  };
  int b;
  b = fun(a, n);
  if (rank == 0) {
    ASSERT_EQ(b, 1);
  }
}

TEST(MPI_Allreduce_4, random) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n = 100;
  int* a = new int[n];
  if (rank == 0) {
    Rand(a, n);
  }
  int b;
  int res = getNumNonMonotonyElems(a, n);
  b = fun(a, n);
  if (rank == 0) {
    ASSERT_EQ(b, res);
  }
}

TEST(MPI_Allreduce_5, random_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int n = 1000;
  int* a = new int[n];
  if (rank == 0) {
    Rand(a, n);
  }
  int b;
  int res = getNumNonMonotonyElems(a, n);
  b = fun(a, n);
  if (rank == 0) {
    ASSERT_EQ(b, res);
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
