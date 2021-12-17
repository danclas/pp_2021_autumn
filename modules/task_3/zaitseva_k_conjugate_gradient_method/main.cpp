// Copyright 2021 Zaitseva Ksenia
#include <gtest/gtest.h>
#include <mpi.h>

#include <cmath>
#include <gtest-mpi-listener.hpp>
#include <iostream>

#include "conjugateGradientMethod.h"

TEST(Parallel_MPI, Test_n3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double A[9] = {4, -1, 2, -1, 6, -2, 2, -2, 5};
  double b[3] = {-1, 9, -10};
  int n = 3;
  std::vector<double> res_seq;
  std::vector<double> res_par = conjugateGradientMethodParallel(A, b, n);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) {
    res_seq = conjugateGradientMethodSerial(A, b, n);
    for (int i = 0; i < n; i++)
      EXPECT_NEAR(res_seq[i], res_par[i],
                  std::numeric_limits<double>::epsilon() * 16 * 1000);
  }
}

TEST(Parallel_MPI, Test_n4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double A[9] = {0, 2, -0.5, 0, 2, 2, -2, 1.5, -0.5};
  double b[3] = {0, -3, 0.5};
  int n = 3;
  std::vector<double> res_seq;
  std::vector<double> res_par = conjugateGradientMethodParallel(A, b, n);
  if (rank == 0) {
    res_seq = conjugateGradientMethodSerial(A, b, n);
    for (int i = 0; i < n; i++)
      EXPECT_NEAR(res_seq[i], res_par[i],
                  std::numeric_limits<double>::epsilon() * 16 * 1000);
  }
}

TEST(Parallel_MPI, Test_n5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double A[9] = {1, 2, 0, 2, 5, -1, 0, -1, 3};
  double b[3] = {1, 5, 0};
  int n = 3;
  std::vector<double> res_seq;
  std::vector<double> res_par = conjugateGradientMethodParallel(A, b, n);
  if (rank == 0) {
    res_seq = conjugateGradientMethodSerial(A, b, n);
    for (int i = 0; i < n; i++)
      EXPECT_NEAR(res_seq[i], res_par[i],
                  std::numeric_limits<double>::epsilon() * 16 * 1000);
  }
}

TEST(Parallel_MPI, Test_n9) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double A[4] = {1, 2, 0, 5};
  double b[2] = {1, 9};
  int n = 2;
  std::vector<double> res_seq;
  std::vector<double> res_par = conjugateGradientMethodParallel(A, b, n);
  if (rank == 0) {
    res_seq = conjugateGradientMethodSerial(A, b, n);
    for (int i = 0; i < n; i++)
      EXPECT_NEAR(res_seq[i], res_par[i],
                  std::numeric_limits<double>::epsilon() * 16 * 1000);
  }
}

TEST(Parallel_MPI, Test_n10) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double A[4] = {-11, 6, 6, -6};
  double b[2] = {1, 0};
  int n = 2;
  std::vector<double> res_seq;
  std::vector<double> res_par = conjugateGradientMethodParallel(A, b, n);
  if (rank == 0) {
    res_seq = conjugateGradientMethodSerial(A, b, n);
    for (int i = 0; i < n; i++)
      EXPECT_NEAR(res_seq[i], res_par[i],
                  std::numeric_limits<double>::epsilon() * 16 * 1000);
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
