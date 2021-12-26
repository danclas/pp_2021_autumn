// Copyright 2021 Gleb "belgad" Danshin
#include <mpi.h>
#include <iostream>

#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>

#include "../../../modules/task_3/danshin_g_ccs_matrix_product/ccs_matrix_product.hh"

TEST(DanshinGlebCCSMatrixProduct, MatrixTypes02x100x100n02x100x100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  CCSMatrix* matrix1 = nullptr;
  CCSMatrix* matrix2 = nullptr;
  CCSMatrix* answer_sequence;
  CCSMatrix* answer_parallel;

  double seq_start_time, seq_end_time, par_start_time, par_end_time;

  if (rank == 0) {
    matrix1 = GenerateRandomMatrix(0.2, 100, 100);
    matrix2 = GenerateRandomMatrix(0.2, 100, 100);

    seq_start_time = MPI_Wtime();
    answer_sequence = MatrixProduct(matrix1, matrix2);
    seq_end_time = MPI_Wtime();
  }

  par_start_time = MPI_Wtime();
  answer_parallel = MatrixProductParallel(matrix1, matrix2);
  par_end_time = MPI_Wtime();

  if (rank == 0) {
    double seq_time_elapsed = seq_end_time - seq_start_time;
    double par_time_elapsed = par_end_time - par_start_time;
    double eff = seq_time_elapsed / par_time_elapsed;

    std::cout << "Sequence time: " << seq_time_elapsed << std::endl
              << "Parallel time: " << par_time_elapsed << std::endl
              << "Efficiency: "    << eff              << std::endl;

    EXPECT_TRUE(*answer_sequence == *answer_parallel);

    delete matrix1;
    delete matrix2;
    delete answer_sequence;
    delete answer_parallel;
  }

  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(DanshinGlebCCSMatrixProduct, MatrixTypes04x100x100n03x100x100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  CCSMatrix* matrix1 = nullptr;
  CCSMatrix* matrix2 = nullptr;
  CCSMatrix* answer_sequence;
  CCSMatrix* answer_parallel;

  double seq_start_time, seq_end_time, par_start_time, par_end_time;

  if (rank == 0) {
    matrix1 = GenerateRandomMatrix(0.4, 100, 100);
    matrix2 = GenerateRandomMatrix(0.3, 100, 100);

    seq_start_time = MPI_Wtime();
    answer_sequence = MatrixProduct(matrix1, matrix2);
    seq_end_time = MPI_Wtime();
  }

  par_start_time = MPI_Wtime();
  answer_parallel = MatrixProductParallel(matrix1, matrix2);
  par_end_time = MPI_Wtime();

  if (rank == 0) {
    double seq_time_elapsed = seq_end_time - seq_start_time;
    double par_time_elapsed = par_end_time - par_start_time;
    double eff = seq_time_elapsed / par_time_elapsed;

    std::cout << "Sequence time: " << seq_time_elapsed << std::endl
              << "Parallel time: " << par_time_elapsed << std::endl
              << "Efficiency: "    << eff              << std::endl;

    EXPECT_TRUE(*answer_sequence == *answer_parallel);

    delete matrix1;
    delete matrix2;
    delete answer_sequence;
    delete answer_parallel;
  }

  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(DanshinGlebCCSMatrixProduct, MatrixTypes02x1000x100n02x100x1000) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  CCSMatrix* matrix1 = nullptr;
  CCSMatrix* matrix2 = nullptr;
  CCSMatrix* answer_sequence;
  CCSMatrix* answer_parallel;

  double seq_start_time, seq_end_time, par_start_time, par_end_time;

  if (rank == 0) {
    matrix1 = GenerateRandomMatrix(0.2, 1000, 100);
    matrix2 = GenerateRandomMatrix(0.2, 100, 1000);

    seq_start_time = MPI_Wtime();
    answer_sequence = MatrixProduct(matrix1, matrix2);
    seq_end_time = MPI_Wtime();
  }

  par_start_time = MPI_Wtime();
  answer_parallel = MatrixProductParallel(matrix1, matrix2);
  par_end_time = MPI_Wtime();

  if (rank == 0) {
    double seq_time_elapsed = seq_end_time - seq_start_time;
    double par_time_elapsed = par_end_time - par_start_time;
    double eff = seq_time_elapsed / par_time_elapsed;

    std::cout << "Sequence time: " << seq_time_elapsed << std::endl
              << "Parallel time: " << par_time_elapsed << std::endl
              << "Efficiency: "    << eff              << std::endl;

    EXPECT_TRUE(*answer_sequence == *answer_parallel);

    delete matrix1;
    delete matrix2;
    delete answer_sequence;
    delete answer_parallel;
  }

  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(DanshinGlebCCSMatrixProduct, MatrixTypes01x1000x1000n01x1000x1000) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  CCSMatrix* matrix1 = nullptr;
  CCSMatrix* matrix2 = nullptr;
  CCSMatrix* answer_sequence;
  CCSMatrix* answer_parallel;

  double seq_start_time, seq_end_time, par_start_time, par_end_time;

  if (rank == 0) {
    matrix1 = GenerateRandomMatrix(0.2, 1000, 1000);
    matrix2 = GenerateRandomMatrix(0.2, 1000, 1000);

    seq_start_time = MPI_Wtime();
    answer_sequence = MatrixProduct(matrix1, matrix2);
    seq_end_time = MPI_Wtime();
  }

  par_start_time = MPI_Wtime();
  answer_parallel = MatrixProductParallel(matrix1, matrix2);
  par_end_time = MPI_Wtime();

  if (rank == 0) {
    double seq_time_elapsed = seq_end_time - seq_start_time;
    double par_time_elapsed = par_end_time - par_start_time;
    double eff = seq_time_elapsed / par_time_elapsed;

    std::cout << "Sequence time: " << seq_time_elapsed << std::endl
              << "Parallel time: " << par_time_elapsed << std::endl
              << "Efficiency: "    << eff              << std::endl;

    EXPECT_TRUE(*answer_sequence == *answer_parallel);

    delete matrix1;
    delete matrix2;
    delete answer_sequence;
    delete answer_parallel;
  }

  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(DanshinGlebCCSMatrixProduct, MatrixTypes00001x2000x3000n00001x3000x1000) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  CCSMatrix* matrix1 = nullptr;
  CCSMatrix* matrix2 = nullptr;
  CCSMatrix* answer_sequence = nullptr;
  CCSMatrix* answer_parallel = nullptr;

  double seq_start_time, seq_end_time, par_start_time, par_end_time;

  if (rank == 0) {
    matrix1 = GenerateRandomMatrix(0.0001, 2000, 3000);
    matrix2 = GenerateRandomMatrix(0.0001, 3000, 1000);

    seq_start_time = MPI_Wtime();
    answer_sequence = MatrixProduct(matrix1, matrix2);
    seq_end_time = MPI_Wtime();
  }

  par_start_time = MPI_Wtime();
  answer_parallel = MatrixProductParallel(matrix1, matrix2);
  par_end_time = MPI_Wtime();

  if (rank == 0) {
    double seq_time_elapsed = seq_end_time - seq_start_time;
    double par_time_elapsed = par_end_time - par_start_time;
    double eff = seq_time_elapsed / par_time_elapsed;

    std::cout << "Sequence time: " << seq_time_elapsed << std::endl
              << "Parallel time: " << par_time_elapsed << std::endl
              << "Efficiency: "    << eff              << std::endl;

    EXPECT_TRUE(*answer_sequence == *answer_parallel);

    delete matrix1;
    delete matrix2;
    delete answer_sequence;
    delete answer_parallel;
  }

  MPI_Barrier(MPI_COMM_WORLD);
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
