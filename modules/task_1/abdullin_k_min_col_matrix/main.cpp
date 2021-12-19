// Copyright 2021 Abdullin Konstantin
#include <gtest/gtest.h>

#include "./min_columns_matrix.h"

#include <gtest-mpi-listener.hpp>

  TEST(min_columns_matrix, matrix_3x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 3, m = 10;
    int* matrix;

    matrix = random_matrix(n, m);

    int* par_min = parallel_min(matrix, n, m);

    if (rank == 0) {
      int* seq_min = sequential_min(matrix, n, m);

      for (int i = 0; i < m; i++)
        ASSERT_EQ(seq_min[i], par_min[i]);
    }
  }

  TEST(min_columns_matrix, matrix_5x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 5, m = 5;
    int* matrix;

    matrix = random_matrix(n, m);

    int* par_min = parallel_min(matrix, n, m);

    if (rank == 0) {
      int* seq_min = sequential_min(matrix, n, m);

      for (int i = 0; i < m; i++)
        ASSERT_EQ(seq_min[i], par_min[i]);
    }
  }

  TEST(min_columns_matrix, matrix_7x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 7, m = 4;
    int* matrix;

    matrix = random_matrix(n, m);

    int* par_min = parallel_min(matrix, n, m);

    if (rank == 0) {
      int* seq_min = sequential_min(matrix, n, m);

      for (int i = 0; i < m; i++)
        ASSERT_EQ(seq_min[i], par_min[i]);
    }
  }

  TEST(min_columns_matrix, matrix_10x11) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 10, m = 11;
    int* matrix;

    matrix = random_matrix(n, m);

    int* par_min = parallel_min(matrix, n, m);

    if (rank == 0) {
      int* seq_min = sequential_min(matrix, n, m);

      for (int i = 0; i < m; i++)
        ASSERT_EQ(seq_min[i], par_min[i]);
    }
  }

  TEST(min_columns_matrix, matrix_6x12) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 6, m = 12;
    int* matrix;

    matrix = random_matrix(n, m);

    int* par_min = parallel_min(matrix, n, m);

    if (rank == 0) {
      int* seq_min = sequential_min(matrix, n, m);

      for (int i = 0; i < m; i++)
        ASSERT_EQ(seq_min[i], par_min[i]);
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


