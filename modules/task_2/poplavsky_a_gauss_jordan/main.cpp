// Copyright 2021 Poplavsky Anton
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "../../../modules/task_2/poplavsky_a_gauss_jordan/gauss_jordan.h"

TEST(Gauss_Jordan, size_1) {
  int* rows;
  double *A, *right_col, *current;
  int size = 1, count_rows, ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcRank < size % ProcNum) {
    count_rows = size / ProcNum + 1;
  } else {
    count_rows = size / ProcNum;
  }
  rows = new int[count_rows];
  A = new double[count_rows * (size + 1)];
  right_col = new double[size];
  current = new double[size + 1];
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < count_rows; i++) {
    rows[i] = ProcRank + ProcNum * i;
    gen.seed(rows[i] * (size + 1));
    for (int j = 0; j < size + 1; j++) {
      A[i * (size + 1) + j] = gen() % 10;
    }
  }
  double* basis = GaussJordan(count_rows, rows, size, A, right_col, current);
  if (ProcRank == 0) {
    for (int i = 0; i < size; i++) {
      printf("%.2lf\n", basis[i]);
    }
  }
}

TEST(Gauss_Jordan, size_3) {
  int* rows;
  double *A, *right_col, *current;
  int size = 3, count_rows, ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcRank < size % ProcNum) {
    count_rows = size / ProcNum + 1;
  } else {
    count_rows = size / ProcNum;
  }
  rows = new int[count_rows];
  A = new double[count_rows * (size + 1)];
  right_col = new double[size];
  current = new double[size + 1];
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < count_rows; i++) {
    rows[i] = ProcRank + ProcNum * i;
    gen.seed(rows[i] * (size + 1));
    for (int j = 0; j < size + 1; j++) {
      A[i * (size + 1) + j] = gen() % 10;
    }
  }
  double* basis = GaussJordan(count_rows, rows, size, A, right_col, current);
  if (ProcRank == 0) {
    for (int i = 0; i < size; i++) {
      printf("%.2lf\n", basis[i]);
    }
  }
}

TEST(Gauss_Jordan, size_5) {
  int* rows;
  double *A, *right_col, *current;
  int size = 5, count_rows, ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcRank < size % ProcNum) {
    count_rows = size / ProcNum + 1;
  } else {
    count_rows = size / ProcNum;
  }
  rows = new int[count_rows];
  A = new double[count_rows * (size + 1)];
  right_col = new double[size];
  current = new double[size + 1];
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < count_rows; i++) {
    rows[i] = ProcRank + ProcNum * i;
    gen.seed(rows[i] * (size + 1));
    for (int j = 0; j < size + 1; j++) {
      A[i * (size + 1) + j] = gen() % 10;
    }
  }
  double* basis = GaussJordan(count_rows, rows, size, A, right_col, current);
  if (ProcRank == 0) {
    for (int i = 0; i < size; i++) {
      printf("%.2lf\n", basis[i]);
    }
  }
}

TEST(Gauss_Jordan, size_9) {
  int* rows;
  double *A, *right_col, *current;
  int size = 9, count_rows, ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcRank < size % ProcNum) {
    count_rows = size / ProcNum + 1;
  } else {
    count_rows = size / ProcNum;
  }
  rows = new int[count_rows];
  A = new double[count_rows * (size + 1)];
  right_col = new double[size];
  current = new double[size + 1];
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < count_rows; i++) {
    rows[i] = ProcRank + ProcNum * i;
    gen.seed(rows[i] * (size + 1));
    for (int j = 0; j < size + 1; j++) {
      A[i * (size + 1) + j] = gen() % 10;
    }
  }
  double* basis = GaussJordan(count_rows, rows, size, A, right_col, current);
  if (ProcRank == 0) {
    for (int i = 0; i < size; i++) {
      printf("%.2lf\n", basis[i]);
    }
  }
}

TEST(Gauss_Jordan, size_12) {
  int* rows;
  double *A, *right_col, *current;
  int size = 12, count_rows, ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcRank < size % ProcNum) {
    count_rows = size / ProcNum + 1;
  } else {
    count_rows = size / ProcNum;
  }
  rows = new int[count_rows];
  A = new double[count_rows * (size + 1)];
  right_col = new double[size];
  current = new double[size + 1];
  std::random_device dev;
  std::mt19937 gen(dev());
  for (int i = 0; i < count_rows; i++) {
    rows[i] = ProcRank + ProcNum * i;
    gen.seed(rows[i] * (size + 1));
    for (int j = 0; j < size + 1; j++) {
      A[i * (size + 1) + j] = gen() % 10;
    }
  }
  double* basis = GaussJordan(count_rows, rows, size, A, right_col, current);
  if (ProcRank == 0) {
    for (int i = 0; i < size; i++) {
      printf("%.2lf\n", basis[i]);
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
