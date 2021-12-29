// Copyright 2021 Kirillov Nikita
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./shell_sort.h"

TEST(Parallel_Operations_MPI, test_sequential_size_8) {
  int rank;
  int size = 8;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    int vec[] = {5, 7, 2, 4, 8, 3, 1, 6};
    int vec_res[] = {1, 2, 3, 4, 5, 6, 7, 8};
    getSequentialOperations(vec, size);
    for (int i = 0; i < size; i++) {
      ASSERT_EQ(vec[i], vec_res[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, test_sequential_and_parallel_size_16) {
  int rank;
  double s, f, t_seq, t_par;
  int size = 16;
  int* vec = 0;
  int* _vec = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    vec = new int[size];
    _vec = new int[size];
    getRandomVector(vec, size);
    for (int i = 0; i < size; i++) {
      _vec[i] = vec[i];
    }
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

TEST(Parallel_Operations_MPI, test_sequential_and_parallel_size_128) {
  int rank;
  double s, f, t_seq, t_par;
  int size = 128;
  int* vec = 0;
  int* _vec = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    vec = new int[size];
    _vec = new int[size];
    getRandomVector(vec, size);
    for (int i = 0; i < size; i++) {
      _vec[i] = vec[i];
    }
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

TEST(Parallel_Operations_MPI, test_sequential_and_parallel_size_512) {
  int rank;
  double s, f, t_seq, t_par;
  int size = 512;
  int* vec = 0;
  int* _vec = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    vec = new int[size];
    _vec = new int[size];
    getRandomVector(vec, size);
    for (int i = 0; i < size; i++) {
      _vec[i] = vec[i];
    }
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

TEST(Parallel_Operations_MPI, test_sequential_and_parallel_size_2048) {
  int rank;
  double s, f, t_seq, t_par;
  int size = 2048;
  int* vec = 0;
  int* _vec = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    vec = new int[size];
    _vec = new int[size];
    getRandomVector(vec, size);
    for (int i = 0; i < size; i++) {
      _vec[i] = vec[i];
    }
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
