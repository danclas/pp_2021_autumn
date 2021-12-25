// Copyright 2021 Gordey Maria
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./riemann_sum.h"

TEST(RIEMANN_SUM_MPI, TEST_Function_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double start, end, stime;

  std::vector<std::pair<double, double>> limits(2);
  limits = {{0, 2}, {0, 1}};
  int n = 200;
  double reference_result;

  const std::function<double(std::vector<double>)> f =
      [](std::vector<double> vec) {
        double x = vec[0];
        double y = vec[1];
        return x + y * y;
      };

  if (rank == 0) {
    start = MPI_Wtime();
    reference_result = getSequentialIntegrals(n, limits, f);
    end = MPI_Wtime();
    stime = end - start;
    std::cout << "Seqeuntial time: " << stime << std::endl;
    std::cout << "Sequential result: " << reference_result << std::fixed
              << std::endl;
    start = MPI_Wtime();
  }

  double parallel_result = getParallelIntegrals(n, limits, f);

  if (rank == 0) {
    end = MPI_Wtime();
    double ptime = end - start;
    std::cout << "Parallel time: " << ptime << std::endl;
    std::cout << "Parallel result: " << parallel_result << std::fixed
              << std::endl;
    std::cout << "Speedup is " << stime / ptime << std::endl;

    ASSERT_NEAR(reference_result, parallel_result, eps);
  }
}

TEST(RIEMANN_SUM_MPI, TEST_Function_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double start, end, stime;

  std::vector<std::pair<double, double>> limits(2);
  limits = {{0, 1}, {0, 1.5}};
  int n = 200;
  double reference_result;

  const std::function<double(std::vector<double>)> f =
      [](std::vector<double> vec) {
        double x = vec[0];
        double y = vec[1];
        return 4 - x * x - y * y;
      };

  if (rank == 0) {
    start = MPI_Wtime();
    reference_result = getSequentialIntegrals(n, limits, f);
    end = MPI_Wtime();
    stime = end - start;
    std::cout << "Seqeuntial time: " << stime << std::endl;
    std::cout << "Sequential result: " << reference_result << std::fixed
              << std::endl;
    start = MPI_Wtime();
  }

  double parallel_result = getParallelIntegrals(n, limits, f);

  if (rank == 0) {
    end = MPI_Wtime();
    double ptime = end - start;
    std::cout << "Parallel time: " << ptime << std::endl;
    std::cout << "Parallel result: " << parallel_result << std::fixed
              << std::endl;
    std::cout << "Speedup is " << stime / ptime << std::endl;

    ASSERT_NEAR(reference_result, parallel_result, eps);
  }
}

TEST(RIEMANN_SUM_MPI, TEST_Function_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double start, end, stime;

  std::vector<std::pair<double, double>> limits(3);
  limits = {{-2, 3}, {0, 1}, {0, 2.4}};
  int n = 100;
  double reference_result;

  const std::function<double(std::vector<double>)> f =
      [](std::vector<double> vec) {
        double x = vec[0];
        double y = vec[1];
        double z = vec[2];
        return sin(x) * y * z * z;
      };

  if (rank == 0) {
    start = MPI_Wtime();
    reference_result = getSequentialIntegrals(n, limits, f);
    end = MPI_Wtime();
    stime = end - start;
    std::cout << "Seqeuntial time: " << stime << std::endl;
    std::cout << "Sequential result: " << reference_result << std::fixed
              << std::endl;
    start = MPI_Wtime();
  }

  double parallel_result = getParallelIntegrals(n, limits, f);

  if (rank == 0) {
    end = MPI_Wtime();
    double ptime = end - start;
    std::cout << "Parallel time: " << ptime << std::endl;
    std::cout << "Parallel result: " << parallel_result << std::fixed
              << std::endl;
    std::cout << "Speedup is " << stime / ptime << std::endl;

    ASSERT_NEAR(reference_result, parallel_result, eps);
  }
}

TEST(RIEMANN_SUM_MPI, TEST_Function_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double start, end, stime;

  std::vector<std::pair<double, double>> limits(3);
  limits = {{0, 1}, {-3, 2.5}, {0, exp(2)}};
  int n = 100;
  double reference_result;

  const std::function<double(std::vector<double>)> f =
      [](std::vector<double> vec) {
        double x = vec[0];
        double y = vec[1];
        double z = vec[2];
        return x * x * y * cos(z);
      };

  if (rank == 0) {
    start = MPI_Wtime();
    reference_result = getSequentialIntegrals(n, limits, f);
    end = MPI_Wtime();
    stime = end - start;
    std::cout << "Seqeuntial time: " << stime << std::endl;
    std::cout << "Sequential result: " << reference_result << std::fixed
              << std::endl;
    start = MPI_Wtime();
  }

  double parallel_result = getParallelIntegrals(n, limits, f);

  if (rank == 0) {
    end = MPI_Wtime();
    double ptime = end - start;
    std::cout << "Parallel time: " << ptime << std::endl;
    std::cout << "Parallel result: " << parallel_result << std::fixed
              << std::endl;
    std::cout << "Speedup is " << stime / ptime << std::endl;

    ASSERT_NEAR(reference_result, parallel_result, eps);
  }
}

TEST(RIEMANN_SUM_MPI, TEST_Function_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double start, end, stime;

  std::vector<std::pair<double, double>> limits(3);
  limits = {{0, 2}, {-1, 2}, {1, exp(1)}};
  int n = 100;
  double reference_result;

  const std::function<double(std::vector<double>)> f =
      [](std::vector<double> vec) {
        double x = vec[0];
        double y = vec[1];
        double z = vec[2];
        return x * y * y * y / z;
      };

  if (rank == 0) {
    start = MPI_Wtime();
    reference_result = getSequentialIntegrals(n, limits, f);
    end = MPI_Wtime();
    stime = end - start;
    std::cout << "Seqeuntial time: " << stime << std::endl;
    std::cout << "Sequential result: " << reference_result << std::fixed
              << std::endl;
    start = MPI_Wtime();
  }

  double parallel_result = getParallelIntegrals(n, limits, f);

  if (rank == 0) {
    end = MPI_Wtime();
    double ptime = end - start;
    std::cout << "Parallel time: " << ptime << std::endl;
    std::cout << "Parallel result: " << parallel_result << std::fixed
              << std::endl;
    std::cout << "Speedup is " << stime / ptime << std::endl;

    ASSERT_NEAR(reference_result, parallel_result, eps);
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
