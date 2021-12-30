// Copyright 2021 Poplavsky Anton
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "../../../modules/task_1/poplavsky_a_monte_carlo/monte_carlo.h"

#define POINTS 10000000
#define PI 3.14159265358979323846264338327950288419

double function_x(double x) { return x; }

double function_x_x(double x) { return x * x; }

double function_x_x_x(double x) { return x * x * x; }

TEST(Monte_Carlo, Function_x) {
  int ProcRank;
  double low = 0.0;
  double up = 100.7;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  double hits = MonteCarlo(low, up, POINTS, function_x);

  if (ProcRank == 0) {
    ASSERT_NEAR(pow(up - low, 2) / 2, hits, abs(0.01 * hits));
  }
}

TEST(Monte_Carlo, Negative_function_x_x) {
  int ProcRank;
  double low = 0.0;
  double up = 95.8;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  double hits = MonteCarlo(low, up, POINTS, function_x_x);

  if (ProcRank == 0) {
    ASSERT_NEAR(pow(up - low, 3) / 3, hits, abs(0.01 * hits));
  }
}

TEST(Monte_Carlo, Function_x_x) {
  int ProcRank;
  double low = 0.0;
  double up = 94.2;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  double hits = MonteCarlo(low, up, POINTS, function_x_x_x);

  if (ProcRank == 0) {
    ASSERT_NEAR(pow((up - low), 4) / 4, hits, abs(0.01 * hits));
  }
}

TEST(Monte_Carlo, Function_sin) {
  int ProcRank;
  double low = 0.0;
  double up = PI / 2;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  double hits = MonteCarlo(low, up, POINTS, sin);

  if (ProcRank == 0) {
    ASSERT_NEAR(1, hits, abs(0.01 * hits));
  }
}

TEST(Monte_Carlo, Function_cos) {
  int ProcRank;
  double low = 0.0;
  double up = PI / 2;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  double hits = MonteCarlo(low, up, POINTS, cos);

  if (ProcRank == 0) {
    ASSERT_NEAR(1, hits, abs(0.01 * hits));
  }
}

TEST(Monte_Carlo, Negative_points) {
  int ProcRank;
  double low = 0.0;
  double up = PI / 2;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcRank == 0) {
    ASSERT_ANY_THROW(MonteCarlo(low, up, -POINTS, sin));
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
