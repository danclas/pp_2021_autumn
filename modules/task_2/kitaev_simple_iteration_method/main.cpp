// Copyright 2021 Kitaev Pavel

#include <gtest/gtest.h>
#include <mpi.h>
#include "./kitaev_simple_iteration_method.h"
#include <gtest-mpi-listener.hpp>

TEST(Simple_iteration_method, Sequential_test) {
  int ProcRank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  /*
  * {10,  1,  1,  1} {13}
  * { 2, 10,  1,  1} {14}
  * { 2,  2, 10,  1} {15}
  * { 2,  2,  2, 10} {16}
  */

  if (ProcRank == 0) {
    bool ok = true;
    double prec = 0.0001;

    std::vector<double> matrix{ 10, 1, 1, 1, 2, 10, 1, 1,
      2, 2, 10, 1, 2, 2, 2, 10 };

    std::vector<double> vect{ 13, 14, 15, 16 };
    std::vector<double> res(vect.size());

    std::vector<double> ref_res{ 1, 1, 1, 1 };

    res = SequentialSimpleIterationMethod(matrix, vect, prec);

    for (int i = 0; i < static_cast<int>(res.size()); i++) {
      if (std::abs(res[i] - ref_res[i]) > prec) {
        ok = false;
      }
    }

    ASSERT_EQ(ok, true);
  }
}

TEST(Simple_iteration_method, Parallel_test) {
  int ProcRank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  /*
  * {10,  1,  1,  1} {13}
  * { 2, 10,  1,  1} {14}
  * { 2,  2, 10,  1} {15}
  * { 2,  2,  2, 10} {16}
  */

  bool ok = true;
  double prec = 0.0001;

  std::vector<double> matrix{ 10, 1, 1, 1, 2, 10, 1, 1,
    2, 2, 10, 1, 2, 2, 2, 10 };
  std::vector<double> vect{ 13, 14, 15, 16 };
  std::vector<double> res(vect.size());

  std::vector<double> ref_res{ 1, 1, 1, 1 };

  res = ParallelSimpleIterationMethod(matrix, vect, prec);

  if (ProcRank == 0) {
    for (int i = 0; i < static_cast<int>(res.size()); i++) {
      if (std::abs(res[i] - ref_res[i]) > prec) {
        ok = false;
      }
    }
  }

  ASSERT_EQ(ok, true);
}

TEST(Simple_iteration_method, Sequential_and_parallel_test) {
  int ProcRank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  /*
  * {10,  1,  1,  1} {13}
  * { 2, 10,  1,  1} {14}
  * { 2,  2, 10,  1} {15}
  * { 2,  2,  2, 10} {16}
  */

  std::vector<double> matrix{ 10, 1, 1, 1, 2, 10, 1, 1,
    2, 2, 10, 1, 2, 2, 2, 10 };
  std::vector<double> vect{ 13, 14, 15, 16 };
  std::vector<double> sequential_res(vect.size());
  std::vector<double> parallel_res(vect.size());
  double prec = 0.0001;

  parallel_res = ParallelSimpleIterationMethod(matrix, vect, prec);

  if (ProcRank == 0) {
    sequential_res = SequentialSimpleIterationMethod(matrix, vect, prec);

    ASSERT_EQ(sequential_res, parallel_res);
  }
}
TEST(Simple_iteration_method, Sequential_and_parallel_test_other_matrix) {
  int ProcRank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  /*
  * {10,  1,  1} {12}
  * { 2, 10,  1} {13}
  * { 2,  2, 10} {14}
  */

  std::vector<double> matrix{ 10, 1, 1, 2, 10, 1, 2, 2, 10 };
  std::vector<double> vect{ 12, 13, 14};
  std::vector<double> sequential_res(vect.size());
  std::vector<double> parallel_res(vect.size());
  double prec = 0.0001;

  parallel_res = ParallelSimpleIterationMethod(matrix, vect, prec);

  if (ProcRank == 0) {
    sequential_res = SequentialSimpleIterationMethod(matrix, vect, prec);

    ASSERT_EQ(sequential_res, parallel_res);
  }
}

TEST(Simple_iteration_method, Sequential_and_parallel_test_other_matrix_2) {
  int ProcRank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  /*
  * {10,  1,  1,  1,  1} {14}
  * { 2, 10,  1,  1,  1} {15}
  * { 2,  2, 10,  1,  1} {16}
  * { 2,  2,  2, 10,  1} {17}
  * { 2,  2,  2,  2, 10} {18}
  */

  std::vector<double> matrix{ 10, 1, 1, 1, 1, 2, 10, 1, 1, 1, 2,
    2, 10, 1, 1, 2, 2, 2, 10, 1, 2, 2, 2, 2, 10 };
  std::vector<double> vect{ 14, 15, 16, 17, 18};
  std::vector<double> sequential_res(vect.size());
  std::vector<double> parallel_res(vect.size());
  double prec = 0.0001;

  parallel_res = ParallelSimpleIterationMethod(matrix, vect, prec);

  if (ProcRank == 0) {
    sequential_res = SequentialSimpleIterationMethod(matrix, vect, prec);

    ASSERT_EQ(sequential_res, parallel_res);
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
