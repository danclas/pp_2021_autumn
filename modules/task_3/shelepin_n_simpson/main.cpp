// Copyright 2021 Shelepin Nikita
#include <gtest/gtest.h>

#include <cmath>
#include <gtest-mpi-listener.hpp>

#include "./simpson.h"

TEST(SIMPSON_METHOD, TEST_FUNCTION_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    printf("  f(x,y) = x * x - 2 * y\n");
    fflush(stdout);
  }

  const std::function<double(std::vector<double>)> f =
      [](std::vector<double> vec) {
        double x = vec[0];
        double y = vec[1];
        return x * x - 2 * y;
      };

  std::vector<std::pair<double, double>> limits({{4, 10}, {1, 2}});
  std::vector<int> n({100, 100});

  double start = MPI_Wtime();
  double result = getParallelSimpson(f, limits, n);
  double end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;

    start = MPI_Wtime();
    double reference_result = getSequentialSimpson(f, limits, n);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "  Sequential result: " << reference_result << std::endl;
    std::cout << "  Sequential time: " << stime << std::endl;
    std::cout << "  Parallel result: " << result << std::endl;
    std::cout << "  Parallel time: " << ptime << std::endl;
    std::cout << "  Speedup: " << stime / ptime << std::endl;

    double error = 0.0001;
    ASSERT_NEAR(result, reference_result, error);
  }
}

TEST(SIMPSON_METHOD, TEST_FUNCTION_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    printf("  f(x,y,z) = log10(2 * x * x) + sqrt(z) + 5 * y\n");
    fflush(stdout);
  }

  const std::function<double(std::vector<double>)> f =
      [](std::vector<double> vec) {
        double x = vec[0];
        double y = vec[1];
        double z = vec[2];
        return log10(2 * x * x) + sqrt(z) + 5 * y;
      };

  std::vector<std::pair<double, double>> limits({{4, 10}, {1, 2}, {2, 5}});
  std::vector<int> n({10, 10, 10});

  double result = getParallelSimpson(f, limits, n);

  if (rank == 0) {
    double reference_result = getSequentialSimpson(f, limits, n);
    double error = 0.0001;
    ASSERT_NEAR(result, reference_result, error);
  }
}

TEST(SIMPSON_METHOD, TEST_FUNCTION_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    printf("  f(x,y,z,t) = x + y + z + t\n");
    fflush(stdout);
  }

  const std::function<double(std::vector<double>)> f =
      [](std::vector<double> vec) {
        double x = vec[0];
        double y = vec[1];
        double z = vec[2];
        return x * y * z;
      };

  std::vector<std::pair<double, double>> limits({{4, 10}, {1, 2}, {4, 5}});
  std::vector<int> n({10, 5, 5});

  double result = getParallelSimpson(f, limits, n);

  if (rank == 0) {
    double reference_result = getSequentialSimpson(f, limits, n);
    double error = 0.0001;
    ASSERT_NEAR(result, reference_result, error);
  }
}

TEST(SIMPSON_METHOD, TEST_FUNCTION_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    printf("  f(x,y,z) = exp(x) - sqrt(10) * 5 * sin(y) + cos(-2 * z * z)\n");
    fflush(stdout);
  }

  const std::function<double(std::vector<double>)> f =
      [](std::vector<double> vec) {
        double x = vec[0];
        double y = vec[1];
        double z = vec[2];
        return exp(x) - sqrt(10) * 5 * sin(y) + cos(-2 * z * z);
      };

  std::vector<std::pair<double, double>> limits({{4, 10}, {1, 2}, {0, 5}});
  std::vector<int> n({10, 5, 5});

  double result = getParallelSimpson(f, limits, n);

  if (rank == 0) {
    double reference_result = getSequentialSimpson(f, limits, n);
    double error = 0.0001;
    ASSERT_NEAR(result, reference_result, error);
  }
}

TEST(SIMPSON_METHOD, TEST_FUNCTION_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    printf("  f(x,y,z,t) = cos(5 * x) + exp(y) + 2.9 * sin(z) - t * t\n");
    fflush(stdout);
  }

  const std::function<double(std::vector<double>)> f =
      [](std::vector<double> vec) {
        double x = vec[0];
        double y = vec[1];
        double z = vec[2];
        double t = vec[3];
        return cos(5 * x) + exp(y) + 2.9 * sin(z) - t * t;
      };

  std::vector<std::pair<double, double>> limits(
      {{4, 10}, {1, 2}, {1, 5}, {6, 10}});
  std::vector<int> n({5, 5, 3, 2});

  double start = MPI_Wtime();
  double result = getParallelSimpson(f, limits, n);
  double end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;

    start = MPI_Wtime();
    double reference_result = getSequentialSimpson(f, limits, n);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "  Sequential result: " << reference_result << std::endl;
    std::cout << "  Sequential time: " << stime << std::endl;
    std::cout << "  Parallel result: " << result << std::endl;
    std::cout << "  Parallel time: " << ptime << std::endl;
    std::cout << "  Speedup: " << stime / ptime << std::endl;

    double error = 0.0001;
    ASSERT_NEAR(result, reference_result, error);
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
