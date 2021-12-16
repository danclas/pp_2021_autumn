// Copyright 2021 Ivina Anastasiya
#include "alg_seidel.h"
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>


TEST(MPI, constnum3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> a;
  std::vector<double> b;
  double eps = 0.01;
  if (rank == 0) {
    a = {{10, 1, 1}, {2, 10, 1}, {2, 2, 10}};
    b = {12, 13, 14};
  }
  std::vector<double> res = parallSeidel(a, b, eps);
  if (rank == 0) {
    ASSERT_NEAR(res[0], 1, 0.01);
    ASSERT_NEAR(res[1], 1, 0.01);
    ASSERT_NEAR(res[2], 1, 0.01);
  }
}

TEST(MPI, randnum3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> a;
  std::vector<double> b;
  double eps = 0.01;
  int n = 3;
  double diag = randomValue(101, 150);

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
  }

  std::vector<double> res = parallSeidel(a, b, eps);
  if (rank == 0) {
    std::vector<double> seq_res = seqSeidel(a, b, eps);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(seq_res[i], res[i]);
    }
  }
}

TEST(MPI, randnum1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> a;
  std::vector<double> b;
  double eps = 0.01;
  int n = 1;
  double diag = randomValue(101, 150);

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
  }

  std::vector<double> res = parallSeidel(a, b, eps);
  if (rank == 0) {
    std::vector<double> seq_res = seqSeidel(a, b, eps);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(seq_res[i], res[i]);
    }
  }
}

TEST(MPI, randnum10) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> a;
  std::vector<double> b;
  double eps = 0.01;
  int n = 10;
  double diag = randomValue(101, 150);

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
  }

  std::vector<double> res = parallSeidel(a, b, eps);
  if (rank == 0) {
    std::vector<double> seq_res = seqSeidel(a, b, eps);
    for (int i = 0; i < n; i++) {
      ASSERT_EQ(seq_res[i], res[i]);
    }
  }
}

TEST(MPI, randnum50) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> a;
  std::vector<double> b;
  double eps = 0.01;
  int n = 50;
  double diag = randomValue(101, 150);

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
