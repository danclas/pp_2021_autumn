// Copyright 2021 Utyugov Denis

#include <gtest/gtest.h>
#include <vector>
#include "./mures_algorithm.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Alg) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<edge> e;
  // if (rank == 0) {
  for (int i = 0; i < 7; i++) {
    add_edge(&e, i, i + 1, i + 1);
  }

  add_edge(&e, 0, 7, 6);
  add_edge(&e, 6, 7, 6);
  add_edge(&e, 7, 3, 5);
  add_edge(&e, 7, 5, -2);
  std::vector<std::vector<int>> a = mure(e, 8, 0);

  std::vector<int> b;
  for (int i = 3; i >= 0; i--) {
    b.push_back(i);
  }
  if (rank == 0) {
    ASSERT_EQ(b, a[4]);
  }
}

TEST(Parallel_Operations_MPI, EQ_Road) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<edge> e;
  // if (rank == 0) {
  for (int i = 0; i < 7; i++) {
    add_edge(&e, i, i + 1, i + 1);
  }

  add_edge(&e, 0, 7, 6);
  add_edge(&e, 6, 7, 6);
  add_edge(&e, 7, 3, 5);
  add_edge(&e, 7, 5, -2);
  std::vector<std::vector<int>> a = mure(e, 8, 0);

  std::vector<int> b;
  for (int i = 3; i >= 0; i--) {
    b.push_back(i);
  }
  if (rank == 0) {
    ASSERT_EQ(b, a[4]);
  }
}

TEST(Parallel_Operations_MPI, Try_Wheel_graph) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<edge> e;
  for (int i = 0; i < 9; i++) {
    add_edge(&e, i, i + 1, i + 1);
  }

  add_edge(&e, 0, 9, 10);
  add_edge(&e, 1, 9, 5);
  add_edge(&e, 2, 9, 5);
  add_edge(&e, 3, 9, 5);
  add_edge(&e, 4, 9, 5);
  add_edge(&e, 5, 9, 5);
  add_edge(&e, 6, 9, -100);
  add_edge(&e, 7, 9, 5);
  add_edge(&e, 8, 9, 5);

  std::vector<int> b;
  for (int i = 6; i >= 0; i--) {
    b.push_back(i);
  }
  // double start = MPI_Wtime();
  std::vector<std::vector<int>> a = mure(e, 10, 0);
  // double end = MPI_Wtime();
  if (rank == 0) {
    // std::cout << "TIME: " << end - start << std::endl;
    ASSERT_EQ(b, a[9]);
  }
}

TEST(Parallel_Operations_MPI, Try_another_start) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<edge> e;
  for (int i = 0; i < 9; i++) {
    add_edge(&e, i, i + 1, i + 1);
  }

  add_edge(&e, 0, 9, 10);
  add_edge(&e, 1, 9, 5);
  add_edge(&e, 2, 9, 5);
  add_edge(&e, 3, 9, 5);
  add_edge(&e, 4, 9, 5);
  add_edge(&e, 5, 9, 5);
  add_edge(&e, 6, 9, -100);
  add_edge(&e, 7, 9, 5);
  add_edge(&e, 8, 9, 5);

  std::vector<int> b;
  for (int i = 6; i >= 3; i--) {
    b.push_back(i);
  }
  std::vector<std::vector<int>> a = mure(e, 10, 3);
  if (rank == 0) {
    ASSERT_EQ(b, a[9]);
  }
}

TEST(Parallel_Operations_MPI, Try_graph_V_type) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<edge> e;
  // if (rank == 0) {
  for (int i = 0; i < 7; i++) {
    add_edge(&e, i, i + 1, i + 1);
  }

  if (rank == 0) {
    ASSERT_NO_THROW(mure(e, 8, 0));
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
