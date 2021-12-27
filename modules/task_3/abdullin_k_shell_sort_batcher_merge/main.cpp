// Copyright 2021 Abdullin Konstantin
#include <gtest/gtest.h>

#include <ctime>
#include <iostream>
#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./shell_sort_batcher_merge.h"

TEST(shell_sort_batcher_merge, correctness_1) {
  int rank, commSize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  if (rank == 0) {
    std::vector <double> result = RandomVector(32);
    result = Shell_sort(result);

    for (int i = 0; i < static_cast<int>(result.size()) - 1; i++)
      EXPECT_TRUE(result[i] <= result[i + 1]);
  }
}

TEST(shell_sort_batcher_merge, correctness_2) {
  int rank, commSize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  std::vector <double> result = RandomVector(32);
  result = Parallel_sort(result);

  if (rank == 0) {
    for (int i = 0; i < static_cast<int>(result.size()) - 1; i++)
      EXPECT_TRUE(result[i] <= result[i + 1]);
  }
}

TEST(shell_sort_batcher_merge, correctness_3) {
  int rank, commSize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  std::vector <double> result = RandomVector(128);
  result = Parallel_sort(result);

  if (rank == 0) {
    for (int i = 0; i < static_cast<int>(result.size()) - 1; i++)
      EXPECT_TRUE(result[i] <= result[i + 1]);
  }
}

TEST(shell_sort_batcher_merge, time_1) {
  int rank, commSize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  std::vector<double> par_result, seq_result;
  double start, end;
  par_result = RandomVector(128);
  seq_result = par_result;

  start = MPI_Wtime();
  par_result = Parallel_sort(par_result);
  end = MPI_Wtime();

  double par_time = end - start;

  start = MPI_Wtime();
  seq_result = Shell_sort(seq_result);
  end = MPI_Wtime();

  double seq_time = end - start;

  if (rank == 0) {
    for (int i = 0; i < static_cast<int>(par_result.size()); i++)
      EXPECT_DOUBLE_EQ(par_result[i], seq_result[i]);

    std::cout << "Parallel time: " << par_time << std::endl;
    std::cout << "Sequential time: " << seq_time << std::endl;
    std::cout << "Speedup: " << seq_time / par_time << std::endl;
  }
}

TEST(shell_sort_batcher_merge, time_2) {
  int rank, commSize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  std::vector<double> par_result, seq_result;
  double start, end;
  par_result = RandomVector(256);
  seq_result = par_result;

  start = MPI_Wtime();
  par_result = Parallel_sort(par_result);
  end = MPI_Wtime();

  double par_time = end - start;

  start = MPI_Wtime();
  seq_result = Shell_sort(seq_result);
  end = MPI_Wtime();

  double seq_time = end - start;

  if (rank == 0) {
    for (int i = 0; i < static_cast<int>(par_result.size()); i++)
      EXPECT_DOUBLE_EQ(par_result[i], seq_result[i]);

    std::cout << "Parallel time: " << par_time << std::endl;
    std::cout << "Sequential time: " << seq_time << std::endl;
    std::cout << "Speedup: " << seq_time / par_time << std::endl;
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  std::srand(std::time(nullptr));

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
