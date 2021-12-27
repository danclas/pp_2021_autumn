// Copyright 2021 Votyakova Daria
#include <gtest/gtest.h>

#include <cmath>
#include <gtest-mpi-listener.hpp>

#include "./trapezoidal.h"

using std::cout;
using std::endl;

const double error = 0.01;

const function<double(vector<double>)> f1 = [](vector<double> vec) {
  double x = vec[0];
  return x * x + 10 * x - 5;
};

const function<double(vector<double>)> f2 = [](vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  return x - sin(y) + x * x;
};

const function<double(vector<double>)> f3 = [](vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  return y * y * y - 10 * cos(x);
};

const function<double(vector<double>)> f4 = [](vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  double z = vec[2];
  return x * x * x + sin(y) * 2 - 10 * z;
};

const function<double(vector<double>)> f5 = [](vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  double z = vec[2];
  return (x + y + z) * 10 * log10(z);
};

TEST(Trapezoidal_method_MPI, TEST_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  ABs abs = {{0, 2}};
  int segments = 100;

  double parallel_integral = ParallelTrapezoidal(segments, abs, f1);

  if (rank == 0) {
    double sequential_integral = SequentialTrapezoidal(segments, abs, f1);

    ASSERT_NEAR(sequential_integral, parallel_integral, error);
  }
}

TEST(Trapezoidal_method_MPI, TEST_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  ABs abs = {{0, 2}, {-1, 5}};
  int segments = 50;

  double parallel_integral = ParallelTrapezoidal(segments, abs, f2);

  if (rank == 0) {
    double sequential_integral = SequentialTrapezoidal(segments, abs, f2);

    ASSERT_NEAR(sequential_integral, parallel_integral, error);
  }
}

TEST(Trapezoidal_method_MPI, TEST_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  ABs abs = {{0, 2}, {1, 3}};
  int segments = 100;

  double parallel_integral = ParallelTrapezoidal(segments, abs, f3);

  if (rank == 0) {
    double sequential_integral = SequentialTrapezoidal(segments, abs, f3);

    ASSERT_NEAR(sequential_integral, parallel_integral, error);
  }
}

TEST(Trapezoidal_method_MPI, TEST_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  ABs abs = {{0, 2}, {1, 3}, {-2, 3}};
  int segments = 10;

  double parallel_integral = ParallelTrapezoidal(segments, abs, f4);

  if (rank == 0) {
    double sequential_integral = SequentialTrapezoidal(segments, abs, f4);

    ASSERT_NEAR(sequential_integral, parallel_integral, error);
  }
}

TEST(Trapezoidal_method_MPI, TEST_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  ABs abs = {{0, 2}, {6, 7}, {5, 10}};
  int segments = 10;

  double start, end;
  if (rank == 0) start = MPI_Wtime();
  double parallel_integral = ParallelTrapezoidal(segments, abs, f5);
  if (rank == 0) end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    cout << "P time: " << ptime << endl;

    start = MPI_Wtime();
    double sequential_integral = SequentialTrapezoidal(segments, abs, f5);
    end = MPI_Wtime();

    double stime = end - start;
    cout << "S time: " << stime << endl;
    cout << "Speedup: " << stime / ptime << endl;

    ASSERT_NEAR(sequential_integral, parallel_integral, error);
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
