// Copyright 2021 Lukashuk Diana
#include <gtest/gtest.h>
#include "../../../modules/task_2/lukashuk_d_gaus_horizon/gauss_horizontal.h"
#include <gtest-mpi-listener.hpp>

TEST(Test_Gauss_Horizontal, Test1_size_3) {
  int ProcRank, ProcCount;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);

  int size = 3;
  double start, end;
  // double seq_time, par_time;
  std::vector<double> seq;
  std::vector<double> par;
  std::vector<double>
    matrix = getRandomMatrix(size, size + 1, ProcRank, ProcCount);

  start = MPI_Wtime();
  par = parGauss(matrix, size, size + 1);

  if (ProcRank == 0) {
    end = MPI_Wtime();
    // par_time = end - start;

    start = MPI_Wtime();
    seq = seqGauss(matrix, size, size + 1);
    end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    for (int i = 0; i < size; i++)
      ASSERT_NEAR(seq[i], par[i], 1e-9);
  }
}

TEST(Test_Gauss_Horizontal, Test1_size_7) {
  int ProcRank, ProcCount;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);

  int size = 7;
  double start, end;
  // double seq_time, par_time;
  std::vector<double> seq;
  std::vector<double> par;
  std::vector<double> matrix =
    getRandomMatrix(size, size + 1, ProcRank, ProcCount);

  start = MPI_Wtime();
  par = parGauss(matrix, size, size + 1);

  if (ProcRank == 0) {
    end = MPI_Wtime();
    // par_time = end - start;

    start = MPI_Wtime();
    seq = seqGauss(matrix, size, size + 1);
    end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    for (int i = 0; i < size; i++)
      ASSERT_NEAR(seq[i], par[i], 1e-9);
  }
}

TEST(Test_Gauss_Horizontal, Test1_size_11) {
  int ProcRank, ProcCount;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);

  int size = 11;
  double start, end;
  // double seq_time, par_time;
  std::vector<double> seq;
  std::vector<double> par;
  std::vector<double> matrix =
    getRandomMatrix(size, size + 1, ProcRank, ProcCount);

  start = MPI_Wtime();
  par = parGauss(matrix, size, size + 1);

  if (ProcRank == 0) {
    end = MPI_Wtime();
    // par_time = end - start;

    start = MPI_Wtime();
    seq = seqGauss(matrix, size, size + 1);
    end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    for (int i = 0; i < size; i++)
      ASSERT_NEAR(seq[i], par[i], 1e-9);
  }
}

TEST(Test_Gauss_Horizontal, Test1_size_16) {
  int ProcRank, ProcCount;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);

  int size = 16;
  double start, end;
  // double seq_time, par_time;
  std::vector<double> seq;
  std::vector<double> par;
  std::vector<double> matrix =
    getRandomMatrix(size, size + 1, ProcRank, ProcCount);

  start = MPI_Wtime();
  par = parGauss(matrix, size, size + 1);

  if (ProcRank == 0) {
    end = MPI_Wtime();
    // par_time = end - start;

    start = MPI_Wtime();
    seq = seqGauss(matrix, size, size + 1);
    end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    for (int i = 0; i < size; i++)
      ASSERT_NEAR(seq[i], par[i], 1e-9);
  }
}

TEST(Test_Gauss_Horizontal, Test1_size_23) {
  int ProcRank, ProcCount;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);

  int size = 23;
  double start, end;
  // double seq_time, par_time;
  std::vector<double> seq;
  std::vector<double> par;
  std::vector<double> matrix =
    getRandomMatrix(size, size + 1, ProcRank, ProcCount);

  start = MPI_Wtime();
  par = parGauss(matrix, size, size + 1);

  if (ProcRank == 0) {
    end = MPI_Wtime();
    // par_time = end - start;

    start = MPI_Wtime();
    seq = seqGauss(matrix, size, size + 1);
    end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    for (int i = 0; i < size; i++)
      ASSERT_NEAR(seq[i], par[i], 1e-9);
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
