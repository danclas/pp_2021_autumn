// Copyright 2021 Zaytsev Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./linear_filtering_block_gauss.h"
#include <gtest-mpi-listener.hpp>
using std::vector;
TEST(Test1, 1) { ASSERT_NO_THROW(RandomMatrix(152, 152)); }
TEST(Test2, 2) { ASSERT_NO_THROW(RandomMatrix(152, 203)); }
TEST(Test3, 3) {
  vector<double> matrix = RandomMatrix(107, 107);

  ASSERT_NO_THROW(SequentialOp(matrix, 107, 107));
}
TEST(Test4, 4) {
  vector<double> matrix = RandomMatrix(107, 152);

  ASSERT_NO_THROW(SequentialOp(matrix, 107, 152));
}
TEST(Test5, 5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int row_count = 203, column_count = 203;
  vector<double> matrix(row_count * column_count);

  if (rank == 0) {
    matrix = RandomMatrix(row_count, column_count);
  }
  ASSERT_NO_THROW(ParallelOp(matrix, 203));
}

TEST(Test6, 6) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int matrix_size = 183;
  vector<double> matrix(matrix_size * matrix_size);

  if (rank == 0) {
    matrix = RandomMatrix(matrix_size, matrix_size);
  }

  Gauss_Core();
  vector<double> res = ParallelOp(matrix, matrix_size);

  if (rank == 0) {
    vector<double> expected_res =
        SequentialOp(matrix, matrix_size, matrix_size);

    ASSERT_EQ(res, expected_res);
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
  // MPI_Init(&argc, &argv);
  // int rank;
  // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // std::vector<int>::size_type image_size = 914;
  // std::vector<double> image(image_size * image_size);
  // if (rank == 0) {
  //  image = RandomMatrix(image_size, image_size);
  //}
  // Gauss_Core(/*2.0*/);
  // auto start_ParallelOp = MPI_Wtime();
  // std::vector<double> res = ParallelOp(image, image_size);
  // auto finish_ParallelOp = MPI_Wtime();
  // if (rank == 0) {
  //  auto start_SequentialOp = MPI_Wtime();
  //  std::vector<double> expected_res =
  //      SequentialOp(image, image_size, image_size);
  //  auto finish_SequentialOp = MPI_Wtime();

  //  auto SequentialOp_running_time = finish_SequentialOp - start_SequentialOp;
  //  auto ParallelOp_running_time = finish_ParallelOp - start_ParallelOp;
  //  auto acceleration = SequentialOp_running_time / ParallelOp_running_time;
  //  std::cout << "Running time of sequential algorithm= "
  //            << SequentialOp_running_time << "\n"
  //            << "Running time of parallel algorithm= "
  //            << ParallelOp_running_time << "\n";
  //  std::cout << "Acceleration= " << acceleration << std::endl;
  //}
  // MPI_Finalize();
}
