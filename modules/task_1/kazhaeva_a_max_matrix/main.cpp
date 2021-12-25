// Copyright 2021 Kazhaeva Anastasia
#include <gtest/gtest.h>
#include <vector>
#include "./max_matrix.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Max_MPI_1, Size_5x5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int a[5 * 5] = { -1, 2, 3, 4, 5,
                   -1, 7, 8, 9, 1,
                   -1, 3, 4, 5, 6,
                   -1, 0, 9, 1, 2,
                   1, 4, 5, 6, 7 };
  const int A_col = 5;
  const int A_row = 5;
  int* b;

  b = Max_Matrix(a, A_row, A_col);
  if (rank == 0) {
    int res[5] = { 1, 7, 9, 9, 7, };
    for (int i = 0; i < A_col; i++) {
      ASSERT_EQ(b[i], res[i]);
    }
  }
}

TEST(Parallel_Max_MPI_2, Size_5x5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int a[5 * 5] = { 1, 2, 3, 4, 5,
                 100, 7, 8, 3, 1,
                   2, 8, 4, 9, 6,
                   9, 0, 9, 50, 2,
                   3, 4, 5, 6, 7 };
  const int A_col = 5;
  const int A_row = 5;
  int* b;
  b = Max_Matrix(a, A_row, A_col);
  if (rank == 0) {
    int res[5] = { 100, 8, 9, 50, 7, };
    for (int i = 0; i < A_col; i++) {
      ASSERT_EQ(b[i], res[i]);
    }
  }
}

TEST(Parallel_Max_MPI_3, Size_5x4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int a[5 * 4] = { 1, 3, 3, 4,
                   6, 7, 8, 3,
                   2, 8, 4, 5,
                   9, 0, 9, 1,
                   3, 4, 5, 6, };
  const int A_col = 4;
  const int A_row = 5;
  int* b;

  b = Max_Matrix(a, A_row, A_col);
  int res[5] = { 9, 8, 9, 6, };
  if (rank == 0) {
    for (int i = 0; i < A_col; i++) {
      ASSERT_EQ(b[i], res[i]);
    }
  }
}
TEST(Parallel_Max_MPI_4, Size_100x100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int Matrix[100 * 100];
  int* resmatr1 = nullptr;
  if (rank == 0) {
    Rand(Matrix, 100, 100);
    resmatr1 = CalcMaxNumber(Matrix, 100, 100);
  }
  int* resmatr2 = nullptr;
  resmatr2 = Max_Matrix(Matrix, 100, 100);
  if (rank == 0) {
    for (int i = 0; i < 100; i++) {
      ASSERT_EQ(resmatr2[i], resmatr1[i]);
    }
  }
}
TEST(Parallel_Max_MPI_5, Size_20x50) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int Matrix[20 * 50];
  int* resmatr1 = nullptr;
  if (rank == 0) {
    Rand(Matrix, 20, 50);
    resmatr1 = CalcMaxNumber(Matrix, 20, 50);
  }
  int* resmatr2 = nullptr;
  resmatr2 = Max_Matrix(Matrix, 20, 50);
  if (rank == 0) {
    for (int i = 0; i < 50; i++) {
      ASSERT_EQ(resmatr2[i], resmatr1[i]);
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
