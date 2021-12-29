// Copyright 2021 Ivina Anastasiya
#include <gtest/gtest.h>
#include <vector>
#include "./alg_seidel.h"
#include <gtest-mpi-listener.hpp>

TEST(alg_Seidel, constnum3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> a;
  std::vector<double> b;
  double eps = 0.01;
  int n = 3;
  double diag = randomValue(101, 150);
  std::vector<double> buff_to_send;
  buff_to_send.resize(n * n);
  b.resize(n);

  if (rank == 0) {
    a = {{10, 1, 1}, {2, 10, 1}, {2, 2, 10}};
    b = {12, 13, 14};

    buff_to_send.clear();
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) buff_to_send.push_back(a[i][j]);
  }

  MPI_Bcast(buff_to_send.data(), buff_to_send.size(), MPI_DOUBLE, 0,
            MPI_COMM_WORLD);
  MPI_Bcast(b.data(), b.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  a.clear();
  int index = 0;
  for (int i = 0; i < n; i++) {
    std::vector<double> row;
    for (int j = 0; j < n; ++j) {
      row.push_back(buff_to_send[index++]);
    }
    a.push_back(row);
  }
  std::vector<double> res = parallSeidel(a, b, eps);
  if (rank == 0) {
    std::vector<double> seq_res = seqSeidel(a, b, eps);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(seq_res[i], res[i]);
    }
  }
}

TEST(alg_Seidel, randnum3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> a;
  std::vector<double> b;
  double eps = 0.01;
  int n = 3;
  double diag = randomValue(101, 150);
  std::vector<double> buff_to_send;
  buff_to_send.resize(n * n);
  b.resize(n);
  if (rank == 0) {
    b.clear();
    for (int i = 0; i < n; i++) {
      std::vector<double> row;
      for (int j = 0; j < n; j++) {
        row.push_back(randomValue(0, 50));
      }
      a.push_back(row);
      a[i][i] = diag;
      b.push_back(randomValue(0, 50));
    }
    buff_to_send.clear();
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) buff_to_send.push_back(a[i][j]);
  }

  MPI_Bcast(buff_to_send.data(), buff_to_send.size(), MPI_DOUBLE, 0,
            MPI_COMM_WORLD);
  MPI_Bcast(b.data(), b.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  a.clear();
  int index = 0;
  for (int i = 0; i < n; i++) {
    std::vector<double> row;
    for (int j = 0; j < n; ++j) {
      row.push_back(buff_to_send[index++]);
    }
    a.push_back(row);
  }

  std::vector<double> res = parallSeidel(a, b, eps);

  if (rank == 0) {
    std::vector<double> seq_res = seqSeidel(a, b, eps);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(seq_res[i], res[i]);
    }
  }
}

TEST(alg_Seidel, randnum1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> a;
  std::vector<double> b;
  double eps = 0.01;
  int n = 1;
  double diag = randomValue(101, 150);
  std::vector<double> buff_to_send;
  buff_to_send.resize(n * n);
  b.resize(n);
  if (rank == 0) {
    b.clear();
    for (int i = 0; i < n; i++) {
      std::vector<double> row;
      for (int j = 0; j < n; j++) {
        row.push_back(randomValue(0, 50));
      }
      a.push_back(row);
      a[i][i] = diag;
      b.push_back(randomValue(0, 50));
    }
    buff_to_send.clear();
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) buff_to_send.push_back(a[i][j]);
  }

  MPI_Bcast(buff_to_send.data(), buff_to_send.size(), MPI_DOUBLE, 0,
            MPI_COMM_WORLD);
  MPI_Bcast(b.data(), b.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  a.clear();
  int index = 0;
  for (int i = 0; i < n; i++) {
    std::vector<double> row;
    for (int j = 0; j < n; ++j) {
      row.push_back(buff_to_send[index++]);
    }
    a.push_back(row);
  }

  std::vector<double> res = parallSeidel(a, b, eps);

  if (rank == 0) {
    std::vector<double> seq_res = seqSeidel(a, b, eps);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(seq_res[i], res[i]);
    }
  }
}

TEST(alg_Seidel, randnum10) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> a;
  std::vector<double> b;
  double eps = 0.01;
  int n = 10;
  double diag = randomValue(101, 150);
  std::vector<double> buff_to_send;
  buff_to_send.resize(n * n);
  b.resize(n);
  if (rank == 0) {
    b.clear();
    for (int i = 0; i < n; i++) {
      std::vector<double> row;
      for (int j = 0; j < n; j++) {
        row.push_back(randomValue(0, 20));
      }
      a.push_back(row);
      a[i][i] = diag;
      b.push_back(randomValue(0, 20));
    }
    buff_to_send.clear();
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) buff_to_send.push_back(a[i][j]);
  }

  MPI_Bcast(buff_to_send.data(), buff_to_send.size(), MPI_DOUBLE, 0,
            MPI_COMM_WORLD);
  MPI_Bcast(b.data(), b.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  a.clear();
  int index = 0;
  for (int i = 0; i < n; i++) {
    std::vector<double> row;
    for (int j = 0; j < n; ++j) {
      row.push_back(buff_to_send[index++]);
    }
    a.push_back(row);
  }

  std::vector<double> res = parallSeidel(a, b, eps);

  if (rank == 0) {
    std::vector<double> seq_res = seqSeidel(a, b, eps);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(seq_res[i], res[i]);
    }
  }
}

TEST(alg_Seidel, randnum50) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> a;
  std::vector<double> b;
  double eps = 0.01;
  int n = 50;
  double diag = randomValue(101, 150);
  std::vector<double> buff_to_send;
  buff_to_send.resize(n * n);
  b.resize(n);
  if (rank == 0) {
    b.clear();
    for (int i = 0; i < n; i++) {
      std::vector<double> row;
      for (int j = 0; j < n; j++) {
        row.push_back(randomValue(0, 20));
      }
      a.push_back(row);
      a[i][i] = diag;
      b.push_back(randomValue(0, 20));
    }
    buff_to_send.clear();
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) buff_to_send.push_back(a[i][j]);
  }

  MPI_Bcast(buff_to_send.data(), buff_to_send.size(), MPI_DOUBLE, 0,
            MPI_COMM_WORLD);
  MPI_Bcast(b.data(), b.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  a.clear();
  int index = 0;
  for (int i = 0; i < n; i++) {
    std::vector<double> row;
    for (int j = 0; j < n; ++j) {
      row.push_back(buff_to_send[index++]);
    }
    a.push_back(row);
  }

  std::vector<double> res = parallSeidel(a, b, eps);

  if (rank == 0) {
    std::vector<double> seq_res = seqSeidel(a, b, eps);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(seq_res[i], res[i]);
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
