// Copyright 2021 Yashin Kirill

#include <gtest/gtest.h>
#include <cmath>
#include "./fox.h"
#include <gtest-mpi-listener.hpp>

TEST(fox, Test_on_Matrix_size_4) {
  int size = 4;
  double* A = nullptr;
  double* B = nullptr;
  double* C = nullptr;
  double* C_fox = nullptr;
  double t_fox_end, t_fox_start, t_seq_end, t_seq_start;
  int rank, procNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int cartSize = static_cast<int>(std::sqrt(procNum));
  if (cartSize * cartSize == procNum && size % cartSize == 0) {
    if (rank == 0) {
      A = new double[size * size];
      B = new double[size * size];
      random_matrix(A, size);
      random_matrix(B, size);
      C = new double[size * size];
      C_fox = new double[size * size];
      t_seq_start = MPI_Wtime();
      sequential_alg(A, B, C, size);
      t_seq_end = MPI_Wtime() - t_seq_start;
    }
    if (rank == 0) {
      t_fox_start = MPI_Wtime();
    }
    int result = fox(A, B, C_fox, size);
    ASSERT_EQ(result, 0);
    if (rank == 0) {
      t_fox_end = MPI_Wtime() - t_fox_start;
      for (int i = 0; i < size * size; ++i) {
        ASSERT_NEAR(C_fox[i], C[i], 0.0001);
      }
    }

    if (rank == 0) {
      delete[] A;
      delete[] B;
      delete[] C;
      delete[] C_fox;
      std::cout << "Seq time (sec)" << t_seq_end << "\n"
                << " Parallel time (sec) " << t_fox_end << std::endl;
      std::cout << " Acceleration " << t_seq_end / t_fox_end << std::endl;
    }
  }
}

TEST(fox, Test_on_Matrix_size_16) {
  int size = 16;
  double* A = nullptr;
  double* B = nullptr;
  double* C = nullptr;
  double* C_fox = nullptr;
  double t_fox_end, t_fox_start, t_seq_end, t_seq_start;
  int rank, procNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int cartSize = static_cast<int>(std::sqrt(procNum));
  if (cartSize * cartSize == procNum && size % cartSize == 0) {
    if (rank == 0) {
      A = new double[size * size];
      B = new double[size * size];
      random_matrix(A, size);
      random_matrix(B, size);
      C = new double[size * size];
      C_fox = new double[size * size];
      t_seq_start = MPI_Wtime();
      sequential_alg(A, B, C, size);
      t_seq_end = MPI_Wtime() - t_seq_start;
    }
    if (rank == 0) {
      t_fox_start = MPI_Wtime();
    }
    int result = fox(A, B, C_fox, size);
    ASSERT_EQ(result, 0);
    if (rank == 0) {
      t_fox_end = MPI_Wtime() - t_fox_start;
      for (int i = 0; i < size * size; ++i) {
        ASSERT_NEAR(C_fox[i], C[i], 0.0001);
      }
    }

    if (rank == 0) {
      delete[] A;
      delete[] B;
      delete[] C;
      delete[] C_fox;
      std::cout << "Seq time (sec)" << t_seq_end << "\n"
                << " Parallel time (sec) " << t_fox_end << std::endl;
      std::cout << " Acceleration " << t_seq_end / t_fox_end << std::endl;
    }
  }
}

TEST(fox, Test_on_Matrix_size_64) {
  int size = 64;
  double* A = nullptr;
  double* B = nullptr;
  double* C = nullptr;
  double* C_fox = nullptr;
  double t_fox_end, t_fox_start, t_seq_end, t_seq_start;
  int rank, procNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int cartSize = static_cast<int>(std::sqrt(procNum));
  if (cartSize * cartSize == procNum && size % cartSize == 0) {
    if (rank == 0) {
      A = new double[size * size];
      B = new double[size * size];
      random_matrix(A, size);
      random_matrix(B, size);
      C = new double[size * size];
      C_fox = new double[size * size];
      t_seq_start = MPI_Wtime();
      sequential_alg(A, B, C, size);
      t_seq_end = MPI_Wtime() - t_seq_start;
    }
    if (rank == 0) {
      t_fox_start = MPI_Wtime();
    }
    int result = fox(A, B, C_fox, size);
    ASSERT_EQ(result, 0);
    if (rank == 0) {
      t_fox_end = MPI_Wtime() - t_fox_start;
      for (int i = 0; i < size * size; ++i) {
        ASSERT_NEAR(C_fox[i], C[i], 0.0001);
      }
    }

    if (rank == 0) {
      delete[] A;
      delete[] B;
      delete[] C;
      delete[] C_fox;
      std::cout << "Seq time (sec)" << t_seq_end << "\n"
                << " Parallel time (sec) " << t_fox_end << std::endl;
      std::cout << " Acceleration " << t_seq_end / t_fox_end << std::endl;
    }
  }
}

TEST(fox, Test_on_Matrix_size_100) {
  int size = 100;
  double* A = nullptr;
  double* B = nullptr;
  double* C = nullptr;
  double* C_fox = nullptr;
  double t_fox_end, t_fox_start, t_seq_end, t_seq_start;
  int rank, procNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int cartSize = static_cast<int>(std::sqrt(procNum));
  if (cartSize * cartSize == procNum && size % cartSize == 0) {
    if (rank == 0) {
      A = new double[size * size];
      B = new double[size * size];
      random_matrix(A, size);
      random_matrix(B, size);
      C = new double[size * size];
      C_fox = new double[size * size];
      t_seq_start = MPI_Wtime();
      sequential_alg(A, B, C, size);
      t_seq_end = MPI_Wtime() - t_seq_start;
    }
    if (rank == 0) {
      t_fox_start = MPI_Wtime();
    }
    int result = fox(A, B, C_fox, size);
    ASSERT_EQ(result, 0);
    if (rank == 0) {
      t_fox_end = MPI_Wtime() - t_fox_start;
      for (int i = 0; i < size * size; ++i) {
        ASSERT_NEAR(C_fox[i], C[i], 0.0001);
      }
    }

    if (rank == 0) {
      delete[] A;
      delete[] B;
      delete[] C;
      delete[] C_fox;
      std::cout << "Seq time (sec)" << t_seq_end << "\n"
                << " Parallel time (sec) " << t_fox_end << std::endl;
      std::cout << " Acceleration " << t_seq_end / t_fox_end << std::endl;
    }
  }
}

TEST(fox, Test_on_Matrix_size_256) {
  int size = 256;
  double* A = nullptr;
  double* B = nullptr;
  double* C = nullptr;
  double* C_fox = nullptr;
  double t_fox_end, t_fox_start, t_seq_end, t_seq_start;
  int rank, procNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);

  int cartSize = static_cast<int>(std::sqrt(procNum));
  if (cartSize * cartSize == procNum && size % cartSize == 0) {
    if (rank == 0) {
      A = new double[size * size];
      B = new double[size * size];
      random_matrix(A, size);
      random_matrix(B, size);
      C = new double[size * size];
      C_fox = new double[size * size];
      t_seq_start = MPI_Wtime();
      sequential_alg(A, B, C, size);
      t_seq_end = MPI_Wtime() - t_seq_start;
    }
    if (rank == 0) {
      t_fox_start = MPI_Wtime();
    }
    int result = fox(A, B, C_fox, size);
    ASSERT_EQ(result, 0);
    if (rank == 0) {
      t_fox_end = MPI_Wtime() - t_fox_start;
      for (int i = 0; i < size * size; ++i) {
        ASSERT_NEAR(C_fox[i], C[i], 0.0001);
      }
    }

    if (rank == 0) {
      delete[] A;
      delete[] B;
      delete[] C;
      delete[] C_fox;
      std::cout << "Seq time (sec)" << t_seq_end << "\n"
                << " Parallel time (sec) " << t_fox_end << std::endl;
      std::cout << " Acceleration " << t_seq_end / t_fox_end << std::endl;
    }
  }
}

TEST(fox, Throw_Matrix_size_17) {
  int size = 17;
  double tmp = 0;
  double* matrix = &tmp;
  int procNum;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  if (procNum > 1) {
    ASSERT_EQ(fox(matrix, matrix, matrix, size), -1);
  }
}

TEST(fox, Throw_Matrix_size_0) {
  int size = 0;
  double tmp = 0;
  double* matrix = &tmp;
  ASSERT_EQ(fox(matrix, matrix, matrix, size), -1);
  ASSERT_EQ(sequential_alg(matrix, matrix, matrix, size), -1);
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
