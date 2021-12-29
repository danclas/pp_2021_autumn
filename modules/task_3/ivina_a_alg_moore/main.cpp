// Copyright 2021 Ivina Anastasiya
#include <gtest/gtest.h>
#include <vector>
#include "./alg_moore.h"
#include <gtest-mpi-listener.hpp>

TEST(alg_moore, vertex_num_10) {
  int rank;
  int Vertex = 10;
  std::vector<std::vector<std::pair<int, int>>> graph(Vertex + 1);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int root = 1;
  int a = 0;
  int b = 0;
  std::vector<std::pair<int, int>> temp;
  if (rank == 0) {
    for (int i = 0; i < 15; i++) {
      temp.resize(i + 1);
      a = randomValue(1, 10);
      b = randomValue(1, 10);
      for (const auto &j : temp) {
        while ((j.first == a && j.second == b) ||
               (j.first == b && j.second == a) || (a == b)) {
          a = randomValue(1, 10);
          b = randomValue(1, 10);
        }
      }
      temp[i].first = a;
      temp[i].second = b;
      graph[a].push_back({b, randomValue(1, 10)});
    }
  }
  int to_send_size = 0;
  int to_send_vertex_num = 0;

  struct vertex {
    int id;
    int to;
    int w;
  };
  std::vector<vertex> to_send;
  if (rank == 0) {
    for (auto i = 0; i < graph.size(); ++i) {
      for (const auto &n : graph[i]) {
        to_send.push_back({i, n.first, n.second});
      }
    }
    to_send_size = to_send.size();
    to_send_vertex_num = graph.size();
  }
  MPI_Bcast(&to_send_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&to_send_vertex_num, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    graph.clear();
    to_send.clear();
    graph.resize(to_send_vertex_num);
  }

  for (int i = 0; i < to_send_size; ++i) {
    vertex ver{};
    if (rank == 0) {
      ver = to_send[i];
    }
    MPI_Bcast(&ver, 3, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
      graph[ver.id].push_back({ver.to, ver.w});
    }
  }
  to_send.clear();

  auto result = par_shortestPathFaster(graph, root, Vertex);

  if (rank == 0) {
    if (result.empty() != false) {
      const auto expected = result[0];
      for (const auto &evaluated : result) {
        EXPECT_EQ(expected, evaluated);
      }
    }
  }
}

TEST(alg_moore, vertex_num_20) {
  int rank;
  int Vertex = 20;
  std::vector<std::vector<std::pair<int, int>>> graph(Vertex + 1);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int root = 1;
  int a = 0;
  int b = 0;
  std::vector<std::pair<int, int>> temp;
  if (rank == 0) {
    for (int i = 0; i < 25; i++) {
      temp.resize(i + 1);
      a = randomValue(1, 20);
      b = randomValue(1, 20);
      for (const auto &j : temp) {
        while ((j.first == a && j.second == b) ||
               (j.first == b && j.second == a) || (a == b)) {
          a = randomValue(1, 20);
          b = randomValue(1, 20);
        }
      }
      temp[i].first = a;
      temp[i].second = b;
      graph[a].push_back({b, randomValue(1, 10)});
    }
  }
  int to_send_size = 0;
  int to_send_vertex_num = 0;

  struct vertex {
    int id;
    int to;
    int w;
  };
  std::vector<vertex> to_send;
  if (rank == 0) {
    for (auto i = 0; i < graph.size(); ++i) {
      for (const auto &n : graph[i]) {
        to_send.push_back({i, n.first, n.second});
      }
    }
    to_send_size = to_send.size();
    to_send_vertex_num = graph.size();
  }
  MPI_Bcast(&to_send_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&to_send_vertex_num, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    graph.clear();
    to_send.clear();
    graph.resize(to_send_vertex_num);
  }

  for (int i = 0; i < to_send_size; ++i) {
    vertex ver{};
    if (rank == 0) {
      ver = to_send[i];
    }
    MPI_Bcast(&ver, 3, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
      graph[ver.id].push_back({ver.to, ver.w});
    }
  }
  to_send.clear();

  auto result = par_shortestPathFaster(graph, root, Vertex);

  if (rank == 0) {
    if (result.empty() != false) {
      const auto expected = result[0];
      for (const auto &evaluated : result) {
        EXPECT_EQ(expected, evaluated);
      }
    }
  }
}

TEST(alg_moore, vertex_num_2) {
  int rank;
  int Vertex = 2;
  std::vector<std::vector<std::pair<int, int>>> graph(Vertex + 1);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int root = 1;
  if (rank == 0) {
    for (int i = 0; i < 1; i++) {
      int a = 1;
      int b = 2;
      graph[a].push_back({b, randomValue(1, 10)});
    }
  }
  int to_send_size = 0;
  int to_send_vertex_num = 0;

  struct vertex {
    int id;
    int to;
    int w;
  };
  std::vector<vertex> to_send;
  if (rank == 0) {
    for (auto i = 0; i < graph.size(); ++i) {
      for (const auto &n : graph[i]) {
        to_send.push_back({i, n.first, n.second});
      }
    }
    to_send_size = to_send.size();
    to_send_vertex_num = graph.size();
  }
  MPI_Bcast(&to_send_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&to_send_vertex_num, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    graph.clear();
    to_send.clear();
    graph.resize(to_send_vertex_num);
  }

  for (int i = 0; i < to_send_size; ++i) {
    vertex ver{};
    if (rank == 0) {
      ver = to_send[i];
    }
    MPI_Bcast(&ver, 3, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
      graph[ver.id].push_back({ver.to, ver.w});
    }
  }
  to_send.clear();

  auto result = par_shortestPathFaster(graph, root, Vertex);

  if (rank == 0) {
    if (result.empty() != false) {
      const auto expected = result[0];
      for (const auto &evaluated : result) {
        EXPECT_EQ(expected, evaluated);
      }
    }
  }
}

TEST(alg_moore, vertex_num_30) {
  int rank;
  int Vertex = 30;
  std::vector<std::vector<std::pair<int, int>>> graph(Vertex + 1);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int root = 1;
  int a = 0;
  int b = 0;
  std::vector<std::pair<int, int>> temp;
  if (rank == 0) {
    for (int i = 0; i < 35; i++) {
      temp.resize(i + 1);
      a = randomValue(1, 30);
      b = randomValue(1, 30);
      for (const auto &j : temp) {
        while ((j.first == a && j.second == b) ||
               (j.first == b && j.second == a) || (a == b)) {
          a = randomValue(1, 30);
          b = randomValue(1, 30);
        }
      }
      temp[i].first = a;
      temp[i].second = b;
      graph[a].push_back({b, randomValue(1, 10)});
    }
  }
  int to_send_size = 0;
  int to_send_vertex_num = 0;

  struct vertex {
    int id;
    int to;
    int w;
  };
  std::vector<vertex> to_send;
  if (rank == 0) {
    for (auto i = 0; i < graph.size(); ++i) {
      for (const auto &n : graph[i]) {
        to_send.push_back({i, n.first, n.second});
      }
    }
    to_send_size = to_send.size();
    to_send_vertex_num = graph.size();
  }
  MPI_Bcast(&to_send_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&to_send_vertex_num, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    graph.clear();
    to_send.clear();
    graph.resize(to_send_vertex_num);
  }

  for (int i = 0; i < to_send_size; ++i) {
    vertex ver{};
    if (rank == 0) {
      ver = to_send[i];
    }
    MPI_Bcast(&ver, 3, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
      graph[ver.id].push_back({ver.to, ver.w});
    }
  }
  to_send.clear();

  auto result = par_shortestPathFaster(graph, root, Vertex);

  if (rank == 0) {
    if (result.empty() != false) {
      const auto expected = result[0];
      for (const auto &evaluated : result) {
        EXPECT_EQ(expected, evaluated);
      }
    }
  }
}

TEST(alg_moore, vertex_num_50) {
  int rank;
  int Vertex = 50;
  std::vector<std::vector<std::pair<int, int>>> graph(Vertex + 1);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int root = 1;
  int a = 0;
  int b = 0;
  std::vector<std::pair<int, int>> temp;
  if (rank == 0) {
    for (int i = 0; i < 55; i++) {
      temp.resize(i + 1);
      a = randomValue(1, 50);
      b = randomValue(1, 50);
      for (const auto &j : temp) {
        while ((j.first == a && j.second == b) ||
               (j.first == b && j.second == a) || (a == b)) {
          a = randomValue(1, 50);
          b = randomValue(1, 50);
        }
      }
      temp[i].first = a;
      temp[i].second = b;
      graph[a].push_back({b, randomValue(1, 10)});
    }
  }
  int to_send_size = 0;
  int to_send_vertex_num = 0;

  struct vertex {
    int id;
    int to;
    int w;
  };
  std::vector<vertex> to_send;
  if (rank == 0) {
    for (auto i = 0; i < graph.size(); ++i) {
      for (const auto &n : graph[i]) {
        to_send.push_back({i, n.first, n.second});
      }
    }
    to_send_size = to_send.size();
    to_send_vertex_num = graph.size();
  }
  MPI_Bcast(&to_send_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&to_send_vertex_num, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    graph.clear();
    to_send.clear();
    graph.resize(to_send_vertex_num);
  }

  for (int i = 0; i < to_send_size; ++i) {
    vertex ver{};
    if (rank == 0) {
      ver = to_send[i];
    }
    MPI_Bcast(&ver, 3, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
      graph[ver.id].push_back({ver.to, ver.w});
    }
  }
  to_send.clear();

  auto result = par_shortestPathFaster(graph, root, Vertex);

  if (rank == 0) {
    if (result.empty() != false) {
      const auto expected = result[0];
      for (const auto &evaluated : result) {
        EXPECT_EQ(expected, evaluated);
      }
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
