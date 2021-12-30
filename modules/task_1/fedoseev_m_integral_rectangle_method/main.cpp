// Copyright 2021 Fedoseyev Mikhail
#include <gtest/gtest.h>
#include <mpi.h>

#include <cmath>
#include <iostream>
#include <gtest-mpi-listener.hpp>

#include "./integral_rectangle_method.h"

TEST(RECTANGLE_METHOD_MPI, TEST_SIN) {
  int RK;
  MPI_Comm_rank(MPI_COMM_WORLD, &RK);
  double x = 0.0, y = 5.5;
  int z = 1000;
  double R_seq, R_par;
  R_par = IntegralParallel(x, y, z, 0);
  if (RK == 0) {
    R_seq = IntegralSequential(x, y, z, 0);
    EXPECT_NEAR(R_seq, R_par,
                std::numeric_limits<double>::epsilon() *
                    std::max(R_par, R_seq) * 16 * 1000);
  }
}

TEST(RECTANGLE_METHOD_MPI, TEST_COS) {
  int RK;
  MPI_Comm_rank(MPI_COMM_WORLD, &RK);
  double x = 0.0, y = 5.5;
  int z = 1000;
  double R_seq, R_par;
  R_par = IntegralParallel(x, y, z, 4);
  if (RK == 0) {
    R_seq = IntegralSequential(x, y, z, 4);
    EXPECT_NEAR(R_seq, R_par,
                std::numeric_limits<double>::epsilon() * 16 * 1000);
  }
}

TEST(RECTANGLE_METHOD_MPI, TEST_1) {
  int RK;
  MPI_Comm_rank(MPI_COMM_WORLD, &RK);
  double x = 0.0, y = 5.5;
  int z = 1000;
  double R_seq, R_par;
  R_par = IntegralParallel(x, y, z, 1);
  if (RK == 0) {
    R_seq = IntegralSequential(x, y, z, 1);
    EXPECT_NEAR(R_seq, R_par,
                std::numeric_limits<double>::epsilon() *
                    std::max(R_par, R_seq) * 16 * 1000);
  }
}

TEST(RECTANGLE_METHOD_MPI, TEST_2) {
  int RK;
  MPI_Comm_rank(MPI_COMM_WORLD, &RK);
  double x = 0.0, y = 5.5;
  int z = 1000;
  double R_seq, R_par;
  R_par = IntegralParallel(x, y, z, 2);
  if (RK == 0) {
    R_seq = IntegralSequential(x, y, z, 2);
    EXPECT_NEAR(R_seq, R_par,
                std::numeric_limits<double>::epsilon() *
                    std::max(R_par, R_seq) * 16 * 1000);
  }
}

TEST(RECTANGLE_METHOD_MPI, TEST_3) {
  int RK;
  MPI_Comm_rank(MPI_COMM_WORLD, &RK);
  double x = 0.0, y = 5.5;
  int z = 1000;
  double R_seq, R_par;
  R_par = IntegralParallel(x, y, z, 3);
  if (RK == 0) {
    R_seq = IntegralSequential(x, y, z, 3);
    EXPECT_NEAR(R_seq, R_par,
                std::numeric_limits<double>::epsilon() *
                    std::max(R_par, R_seq) * 16 * 1000);
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
