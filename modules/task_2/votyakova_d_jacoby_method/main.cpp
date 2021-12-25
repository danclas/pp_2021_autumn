// Copyright 2021 Votyakova Daria

#include <gtest/gtest.h>

#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./jacoby_method.h"

TEST(JACOBI_METHOD_MPI, TEST_Equation_1) {
  fflush(stdout);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> A, b;

  if (rank == 0) {
    A = {4., 1., 1., 1., 6., -1., 1., 2., 5};
    b = {9., 10., 20.};

    std::vector<double> x = getJacobiSequential(A, b, 3);
    double error = getError(A, x, b);
    fflush(stdout);
    ASSERT_LE(error, eps);
  }

  std::vector<double> x = getJacobiParallel(A, b, 3);

  if (rank == 0) {
    double error = getError(A, x, b);
    ASSERT_LE(error, eps);
  }
  fflush(stdout);
}

TEST(JACOBI_METHOD_MPI, TEST_Equation_2) {
  fflush(stdout);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> A, b;

  if (rank == 0) {
    A = {32., 2., 1., 3., 1., 1.,  8., 3., 1., 3., 1., 2., 16.,
         3.,  1., 1., 2., 3., 56., 1., 1., 2., 1., 3., 32.};
    b = {43., 14., -3., 169., -19.};

    std::vector<double> x = getJacobiSequential(A, b, 5);
    double error = getError(A, x, b);
    fflush(stdout);
    ASSERT_LE(error, eps);
  }

  std::vector<double> x = getJacobiParallel(A, b, 5);

  if (rank == 0) {
    double error = getError(A, x, b);
    ASSERT_LE(error, eps);
  }
  fflush(stdout);
}

TEST(JACOBI_METHOD_MPI, TEST_Random_Equation_3) {
  fflush(stdout);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> A, b;
  int size = 100;

  if (rank == 0) {
    A = getDiagonallyDominantMatrix(size);
    b = getB(size);

    std::vector<double> x = getJacobiSequential(A, b, size);
    double error = getError(A, x, b);
    ASSERT_LE(error, eps);
  }

  std::vector<double> x = getJacobiParallel(A, b, size);

  if (rank == 0) {
    double error = getError(A, x, b);
    ASSERT_LE(error, eps);
  }
  fflush(stdout);
}

TEST(JACOBI_METHOD_MPI, TEST_Random_Equation_4) {
  fflush(stdout);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> A, b;
  int size = 500;

  if (rank == 0) {
    A = getDiagonallyDominantMatrix(size);
    b = getB(size);

    std::vector<double> x = getJacobiSequential(A, b, size);
    double error = getError(A, x, b);
    ASSERT_LE(error, eps);
  }

  std::vector<double> x = getJacobiParallel(A, b, size);

  if (rank == 0) {
    double error = getError(A, x, b);
    ASSERT_LE(error, eps);
  }
  fflush(stdout);
}

TEST(JACOBI_METHOD_MPI, TEST_Random_Equation_5) {
  fflush(stdout);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> A, b;
  // double start, end, stime, ptime;
  int size = 1000;

  if (rank == 0) {
    A = getDiagonallyDominantMatrix(size);
    b = getB(size);

    // start = MPI_Wtime();
    std::vector<double> x = getJacobiSequential(A, b, size);
    // end = MPI_Wtime();

    double error = getError(A, x, b);
    // stime = end - start;
    // printf("Sequential error: %f\n", error);
    // printf("Sequential time: %f\n", stime);
    ASSERT_LE(error, eps);
    // start = MPI_Wtime();
  }

  std::vector<double> x = getJacobiParallel(A, b, size);

  if (rank == 0) {
    // end = MPI_Wtime();
    double error = getError(A, x, b);
    // ptime = end - start;
    // printf("Parallel error: %f\n", error);
    // printf("Parallel time: %f\n", ptime);
    // printf("Speedup: %f\n", stime / ptime);
    ASSERT_LE(error, eps);
  }
  fflush(stdout);
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
