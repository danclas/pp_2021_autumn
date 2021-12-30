  // Copyright 2021 Kitaev Pavel

#include <gtest/gtest.h>
#include <mpi.h>
#include "./kitaev_algorithm_moores.h"
#include <gtest-mpi-listener.hpp>

TEST(Moores_Algorithm, Get_Random_Graph_10) {
  int size_graph = 10;
  ASSERT_NO_THROW(GetRandomGraph(size_graph));
}

TEST(Moores_Algorithm, Get_Random_Graph_100) {
  int size_graph = 100;
  ASSERT_NO_THROW(GetRandomGraph(size_graph));
}

TEST(Moores_Algorithm, Sequential_Moores_Algorithm_10) {
  int ProcRank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    int size_graph = 10;
    std::vector<int> graph = GetRandomGraph(size_graph);
    ASSERT_NO_THROW(SequentialMoores(graph, size_graph));
  }
}

TEST(Moores_Algorithm, Sequential_Moores_Algorithm_100) {
  int ProcRank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    int size_graph = 100;
    std::vector<int> graph = GetRandomGraph(size_graph);
    ASSERT_NO_THROW(SequentialMoores(graph, size_graph));
  }
}

TEST(Moores_Algorithm, Sequential_and_Parallel_Moores_Algorithm_10) {
  int ProcRank;
  int size_graph = 10;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  std::vector<int> graph;
  if (ProcRank == 0) {
    graph = GetRandomGraph(size_graph);
  }

  std::vector<int> par_res(size_graph);
  par_res = ParallelMoores(graph, size_graph);

  if (ProcRank == 0) {
    std::vector<int> seq_res(size_graph);
    seq_res = SequentialMoores(graph, size_graph);
    ASSERT_EQ(par_res, seq_res);
  }
}

TEST(Moores_Algorithm, Sequential_and_Parallel_Moores_Algorithm_50) {
  int ProcRank;
  int size_graph = 50;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  std::vector<int> graph;
  if (ProcRank == 0) {
    graph = GetRandomGraph(size_graph);
  }

  std::vector<int> par_res(size_graph);
  par_res = ParallelMoores(graph, size_graph);

  if (ProcRank == 0) {
    std::vector<int> seq_res(size_graph);
    seq_res = SequentialMoores(graph, size_graph);
    ASSERT_EQ(par_res, seq_res);
  }
}
TEST(Moores_Algorithm, Sequential_and_Parallel_Moores_Algorithm_100_Time) {
  int ProcRank;
  int size_graph = 100;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  double t1, t2;
  std::vector<int> graph;
  if (ProcRank == 0) {
    graph = GetRandomGraph(size_graph);
  }

  std::vector<int> par_res(size_graph);
  MPI_Barrier(MPI_COMM_WORLD);
  t1 = MPI_Wtime();
  par_res = ParallelMoores(graph, size_graph);
  t2 = MPI_Wtime();

  if (ProcRank == 0) {
    std::cout << "Parallel time: " << t2 - t1 << std::endl;

    std::vector<int> seq_res(size_graph);
    t1 = MPI_Wtime();
    seq_res = SequentialMoores(graph, size_graph);
    t2 = MPI_Wtime();

    std::cout << "Sequential time: " << t2 - t1 << std::endl;
    ASSERT_EQ(par_res, seq_res);
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
