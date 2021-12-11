// Copyright 2021 Feoktistov Andrey
#include <gtest/gtest.h>
#include <vector>
#include "../feoktistov_a_vector_avg/vector_avg.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_NEGATIVE_SIZE) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = -1;

  if (rank == 0) global_vec = generateRandomPositiveVector(count_size_vector);

  float Secaverage = calcParallelAverage(global_vec, count_size_vector);

  if (rank == 0) {
    float Paraverage = calcSequentialAverage(global_vec);
    ASSERT_EQ(Secaverage, Paraverage);
  }
}

TEST(Parallel_Operations_MPI, Test_0_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 0;

  if (rank == 0) global_vec = generateRandomPositiveVector(count_size_vector);

  float Secaverage = calcParallelAverage(global_vec, count_size_vector);

  if (rank == 0) {
    float Paraverage = calcSequentialAverage(global_vec);
    ASSERT_EQ(Secaverage, Paraverage);
  }
}

TEST(Parallel_Operations_MPI, Test_10_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 10;

  if (rank == 0) global_vec = generateRandomPositiveVector(count_size_vector);

  float Secaverage = calcParallelAverage(global_vec, count_size_vector);

  if (rank == 0) {
    float Paraverage = calcSequentialAverage(global_vec);
    ASSERT_EQ(Secaverage, Paraverage);
  }
}

TEST(Parallel_Operations_MPI, Test_15_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 15;

  if (rank == 0) global_vec = generateRandomPositiveVector(count_size_vector);

  float Secaverage = calcParallelAverage(global_vec, count_size_vector);

  if (rank == 0) {
    float Paraverage = calcSequentialAverage(global_vec);
    ASSERT_EQ(Secaverage, Paraverage);
  }
}

TEST(Parallel_Operations_MPI, Test_150_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 150;

  if (rank == 0) global_vec = generateRandomPositiveVector(count_size_vector);

  float Secaverage = calcParallelAverage(global_vec, count_size_vector);

  if (rank == 0) {
    float Paraverage = calcSequentialAverage(global_vec);
    ASSERT_EQ(Secaverage, Paraverage);
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
