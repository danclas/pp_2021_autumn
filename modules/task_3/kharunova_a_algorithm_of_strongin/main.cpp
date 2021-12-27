// Copyright 2021 Kharunova Alina
#include <gtest/gtest.h>

#include "./algorithm_of_strongin.h"

#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, parallel_sum_in_10_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time;
  if (rank == 0) {
    start = MPI_Wtime();
    linAlgorithm(1, 10, 0.01);
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
    start = MPI_Wtime();
  }
  paralAlgorithm(1, 10, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_100_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time;
  if (rank == 0) {
    start = MPI_Wtime();
    linAlgorithm(1, 100, 0.01);
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
    start = MPI_Wtime();
  }
  paralAlgorithm(1, 100, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_1000_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time;
  if (rank == 0) {
    start = MPI_Wtime();
    linAlgorithm(1, 1000, 0.01);
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
    start = MPI_Wtime();
  }
  paralAlgorithm(1, 1000, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_500_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time;
  if (rank == 0) {
    start = MPI_Wtime();
    linAlgorithm(1, 500, 0.01);
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
    start = MPI_Wtime();
  }
  paralAlgorithm(1, 500, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_474_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time;
  if (rank == 0) {
    start = MPI_Wtime();
    linAlgorithm(1, 474, 0.01);
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
    start = MPI_Wtime();
  }
  paralAlgorithm(1, 474, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
  }
}

TEST(Parallel_Operations_MPI, parallel_sum_in_50_elements_matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end, time;
  if (rank == 0) {
    start = MPI_Wtime();
    linAlgorithm(1, 50, 0.01);
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
    start = MPI_Wtime();
  }
  paralAlgorithm(1, 50, 0.01);
  if (rank == 0) {
    end = MPI_Wtime();
    time = end - start;
    std::cout << time << std::endl;
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
