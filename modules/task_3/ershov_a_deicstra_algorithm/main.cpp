// Copyright 2021 Ershov Aleksey
#include <gtest/gtest.h>

#include <vector>

#include "./deicstra_mpi.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Deicstra_MPI, Test_Deicstra) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t count = 6;
  std::vector<int> result;
  std::vector<int> trueResult;
  std::vector<std::vector<int>> graf;

  if (rank == 0) {
    graf = {{0, 7, 9, 0, 0, 14},  {7, 0, 10, 15, 0, 0}, {9, 10, 0, 11, 0, 2},
            {0, 15, 11, 0, 6, 0}, {0, 0, 0, 6, 0, 9},   {14, 0, 2, 0, 9, 0}};

    result = getDeicstra(graf, count, 0);
    trueResult = {0, 7, 9, 20, 20, 11};

    for (size_t i = 0; i < count; ++i) {
      ASSERT_EQ(result[i], trueResult[i]);
    }
  }
}

TEST(Parallel_Deicstra_MPI, Test_Sequential_Deicstra) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t count = 6;
  std::vector<int> result;
  std::vector<int> trueResult;
  std::vector<std::vector<int>> graf;

  if (rank == 0) {
    graf = {{0, 7, 9, 0, 0, 14},  {7, 0, 10, 15, 0, 0}, {9, 10, 0, 11, 0, 2},
            {0, 15, 11, 0, 6, 0}, {0, 0, 0, 6, 0, 9},   {14, 0, 2, 0, 9, 0}};

    result = getSequentialDeicstra(graf, count);

    trueResult = {0,  7,  9,  20, 20, 11, 7,  0,  10, 15, 21, 12,
                  9,  10, 0,  11, 11, 2,  20, 15, 11, 0,  6,  13,
                  20, 21, 11, 6,  0,  9,  11, 12, 2,  13, 9,  0};

    for (size_t i = 0; i < count * count; ++i) {
      ASSERT_EQ(result[i], trueResult[i]);
    }
  }
}

TEST(Parallel_Deicstra_MPI, Test_Parallel_Deicstra) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t count = 6;
  std::vector<int> result;
  std::vector<int> trueResult;
  std::vector<std::vector<int>> graf;
  graf = {{0, 7, 9, 0, 0, 14},  {7, 0, 10, 15, 0, 0}, {9, 10, 0, 11, 0, 2},
          {0, 15, 11, 0, 6, 0}, {0, 0, 0, 6, 0, 9},   {14, 0, 2, 0, 9, 0}};

  result = getParallelDeicstra(graf, count);

  if (rank == 0) {
    trueResult = {0,  7,  9,  20, 20, 11, 7,  0,  10, 15, 21, 12,
                  9,  10, 0,  11, 11, 2,  20, 15, 11, 0,  6,  13,
                  20, 21, 11, 6,  0,  9,  11, 12, 2,  13, 9,  0};

    for (size_t i = 0; i < count * count; ++i) {
      ASSERT_EQ(result[i], trueResult[i]);
    }
  }
}

TEST(Parallel_Deicstra_MPI, Test_Random_Graf_Parallel_Deicstra) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t count = 9;
  std::vector<int> result;
  std::vector<int> trueResult;
  std::vector<std::vector<int>> graf;
  graf = getRandomVector(count);

  result = getParallelDeicstra(graf, count);

  if (rank == 0) {
    trueResult = getSequentialDeicstra(graf, count);
    for (size_t i = 0; i < count * count; ++i) {
      ASSERT_EQ(result[i], trueResult[i]);
    }
  }
}

TEST(Parallel_Deicstra_MPI, Test_Random_Normal_Graf_Parallel_Deicstra) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const size_t count = 100;
  std::vector<int> result;
  std::vector<int> trueResult;
  std::vector<std::vector<int>> graf;
  graf = getRandomVector(count);

  result = getParallelDeicstra(graf, count);

  if (rank == 0) {
    trueResult = getSequentialDeicstra(graf, count);
    for (size_t i = 0; i < count * count; ++i) {
      ASSERT_EQ(result[i], trueResult[i]);
    }
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
