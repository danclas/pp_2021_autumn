// Copyright 2021 Kirillov Nikita
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./shell_sort.h"

TEST(Parallel_Operations_MPI, test_sequential) {
  int rank;
  int size = 10;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    int vec[] = {5, 7, 2, 4, 8, 3, 1, 6};
    int vec_res[] = {1, 2, 3, 4, 5, 6, 7, 8};
    getSequentialOperations(vec, 8);
    for (int i = 0; i < 8; i++) {
      ASSERT_EQ(vec[i], vec_res[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, test_sequential_and_parallel) {
  int rank;
  double s, f, t_seq, t_par;
  int size = 10;
  int* vec = 0;
  int* _vec = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    vec = new int[size];
    _vec = new int[size];
    getRandomVector(vec, size);
    _vec = vec;
    s = MPI_Wtime();
    getSequentialOperations(vec, size);
    f = MPI_Wtime();
    t_seq = f - s;
    s = MPI_Wtime();
  }
  getParallelOperations(_vec, size);
  if (rank == 0) {
    f = MPI_Wtime();
    t_par = f - s;
    std::cout << "Time parallel " << t_par << std::endl;
    std::cout << "Time sequential " << t_seq << std::endl;
    std::cout << "Boost " << t_seq / t_par << std::endl;
    for (int i = 0; i < size; i++) {
      ASSERT_EQ(vec[i], _vec[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, test_sequential_and_parallel_size_1000) {
  int rank;
  double s, f, t_seq, t_par;
  int size = 1000;
  int* vec = 0;
  int* _vec = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    vec = new int[size];
    _vec = new int[size];
    getRandomVector(vec, size);
    _vec = vec;
    s = MPI_Wtime();
    getSequentialOperations(vec, size);
    f = MPI_Wtime();
    t_seq = f - s;
    s = MPI_Wtime();
  }
  getParallelOperations(_vec, size);
  if (rank == 0) {
    f = MPI_Wtime();
    t_par = f - s;
    std::cout << "Time parallel " << t_par << std::endl;
    std::cout << "Time sequential " << t_seq << std::endl;
    std::cout << "Boost " << t_seq / t_par << std::endl;
    for (int i = 0; i < size; i++) {
      ASSERT_EQ(vec[i], _vec[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, test_sequential_and_parallel_size_1501) {
  int rank;
  double s, f, t_seq, t_par;
  int size = 1501;
  int* vec = 0;
  int* _vec = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    vec = new int[size];
    _vec = new int[size];
    getRandomVector(vec, size);
    _vec = vec;
    s = MPI_Wtime();
    getSequentialOperations(vec, size);
    f = MPI_Wtime();
    t_seq = f - s;
    s = MPI_Wtime();
  }
  getParallelOperations(_vec, size);
  if (rank == 0) {
    f = MPI_Wtime();
    t_par = f - s;
    std::cout << "Time parallel " << t_par << std::endl;
    std::cout << "Time sequential " << t_seq << std::endl;
    std::cout << "Boost " << t_seq / t_par << std::endl;
    for (int i = 0; i < size; i++) {
      ASSERT_EQ(vec[i], _vec[i]);
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
