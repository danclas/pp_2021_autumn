// Copyright 2021 Lukashuk Diana
#include <gtest/gtest.h>

#include <iostream>

#include "./min_matrix.h"
#include <gtest-mpi-listener.hpp>

TEST(Matrix_class, Test_get) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    static int val[] = {2,  4,  19, 0,  -5, -4, 2,  2,  5,  -100, 20, 10, 12,
                        14, 50, 10, 90, 32, 65, 11, 50, 23, 85,   75, 10};
    Matrix a(-1, 0);
    bool d = 1;
    if (a.get(0) != val[0]) d = 0;
    if (a.get(10) != val[10]) d = 0;
    if (a.get(24) != val[24]) d = 0;
    ASSERT_EQ(d, true);
  }
}
TEST(Matrix_class, Test_min_in_line) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    Matrix a(-1, 0);
    bool d = 1;
    if (a.Min_in_line(0) != -5) d = 0;
    if (a.Min_in_line(1) != -100) d = 0;
    if (a.Min_in_line(2) != 10) d = 0;
    if (a.Min_in_line(5) != 0) d = 0;
    ASSERT_EQ(d, true);
  }
}
TEST(Matrix_class, Test_operator_equals) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    Matrix a(20, 20), b(-1, 0);
    bool d = 1;
    a = b;
    for (int i = 0; i < 25; i++) {
      if (a.get(i) != b.get(i)) d = 0;
    }
    ASSERT_EQ(d, true);
  }
}

TEST(Matrix_class_with_mpi, Test_min_in_matrix_base) {
  Matrix a(-1, 0);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int res = a.Min_in_matrix_mpi();
  if (rank == 0) {
    ASSERT_EQ(res, -100);
  }
}

TEST(Matrix_class_with_mpi, Test_min_in_matrix_10x10) {
  Matrix a(10, 10);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int res = a.Min_in_matrix_mpi();
  if (rank == 0) {
    ASSERT_EQ(res, a.Min_in_matrix_one());
  }
}

TEST(Matrix_class_with_mpi, Test_min_in_matrix_7x6) {
  Matrix a(7, 6);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int res = a.Min_in_matrix_mpi();
  if (rank == 0) {
    ASSERT_EQ(res, a.Min_in_matrix_one());
  }
}
TEST(Matrix_class_with_mpi, Test_min_in_matrix_50x50) {
  Matrix a(50, 50);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int res = a.Min_in_matrix_mpi();
  if (rank == 0) {
    ASSERT_EQ(res, a.Min_in_matrix_one());
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
