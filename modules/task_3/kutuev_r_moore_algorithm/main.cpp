// Copyright 2021 Kutuev Roman
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "../../../modules/task_3/kutuev_r_moore_algorithm/moore.h"

TEST(Moore_algorithm, Create_random) { ASSERT_NO_THROW(random(100)); }

TEST(Moore_algorithm, Moore_algorithm_ok) {
  int ProcRank;
  int vertex = 5;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    std::vector<int> graph(vertex * vertex);
    graph = random(vertex);
    ASSERT_NO_THROW(moore_algorithm(graph, vertex));
  }
}

TEST(Moore_algorithm, Moore_parallel_ok) {
  int ProcRank;
  int vertex = 100;
  std::vector<int> graph(vertex * vertex);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    graph = random(vertex);
  }
  MPI_Bcast(graph.data(), vertex * vertex, MPI_INT, 0, MPI_COMM_WORLD);
  ASSERT_NO_THROW(moore_parallel(graph, vertex));
}

TEST(Moore_algorithm, Graph_400) {
  int ProcRank;
  int vertex = 400;
  double t1, t2, t3, t4;
  std::vector<int> graph(vertex * vertex);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    graph = random(vertex);
  }
  MPI_Bcast(graph.data(), vertex * vertex, MPI_INT, 0, MPI_COMM_WORLD);
  t1 = MPI_Wtime();
  std::vector<int> global_marks = moore_parallel(graph, vertex);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    t2 = MPI_Wtime();
    std::vector<int> marks(vertex);
    t3 = MPI_Wtime();
    marks = moore_algorithm(graph, vertex);
    t4 = MPI_Wtime();
    printf("%lf\n", t2 - t1);
    fflush(stdout);
    printf("%lf\n", t4 - t3);
    fflush(stdout);
    printf("%lf\n", (t4 - t3) / (t2 - t1));
    fflush(stdout);
    for (int i = 0; i < vertex; i++) {
      ASSERT_EQ(global_marks[i], marks[i]);
    }
  }
}

TEST(Moore_algorithm, Graph_600) {
  int ProcRank;
  int vertex = 600;
  double t1, t2, t3, t4;
  std::vector<int> graph(vertex * vertex);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    graph = random(vertex);
  }
  MPI_Bcast(graph.data(), vertex * vertex, MPI_INT, 0, MPI_COMM_WORLD);
  t1 = MPI_Wtime();
  std::vector<int> global_marks = moore_parallel(graph, vertex);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    t2 = MPI_Wtime();
    std::vector<int> marks(vertex);
    t3 = MPI_Wtime();
    marks = moore_algorithm(graph, vertex);
    t4 = MPI_Wtime();
    printf("%lf\n", t2 - t1);
    fflush(stdout);
    printf("%lf\n", t4 - t3);
    fflush(stdout);
    printf("%lf\n", (t4 - t3) / (t2 - t1));
    fflush(stdout);
    for (int i = 0; i < vertex; i++) {
      ASSERT_EQ(global_marks[i], marks[i]);
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
