// Copyright 2021 Fedoseev Mikhail
#include <gtest/gtest.h>

#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./odd_even.h"

TEST(ODD_EVEN_SORT_MPI, TEST_SIZE_128) {
  int rank, num;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  std::vector<int> vec, vec1, vec2;
  int vec_size = 128;
  double start, end;

  if (rank == 0) {
    vec = getRandomVector(vec_size);
    vec1 = vec;
    start = MPI_Wtime();
  }
  ParallelOddEvenSort(&vec1);
  if (rank == 0) {
    end = MPI_Wtime();
    double ptime = end - start;
    std::cout << "P time " << ptime << std::endl;
    vec2 = vec;
    start = MPI_Wtime();
    SequentialOddEvenSort(&vec2);
    end = MPI_Wtime();
    double stime = end - start;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < vec_size; i++) {
      ASSERT_EQ(vec1[i], vec2[i]);
    }
  }
}

TEST(ODD_EVEN_SORT_MPI, TEST_SIZE_123) {
  int rank, num;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  std::vector<int> vec, vec1, vec2;
  int vec_size = 123;
  double start, end;

  if (rank == 0) {
    vec = getRandomVector(vec_size);
    vec1 = vec;
    start = MPI_Wtime();
  }
  ParallelOddEvenSort(&vec1);
  if (rank == 0) {
    end = MPI_Wtime();
    double ptime = end - start;
    std::cout << "P time " << ptime << std::endl;
    vec2 = vec;
    start = MPI_Wtime();
    SequentialOddEvenSort(&vec2);
    end = MPI_Wtime();
    double stime = end - start;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < vec_size; i++) {
      ASSERT_EQ(vec1[i], vec2[i]);
    }
  }
}

TEST(ODD_EVEN_SORT_MPI, TEST_SIZE_1001) {
  int rank, num;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  std::vector<int> vec, vec1, vec2;
  int vec_size = 1001;
  double start, end;

  if (rank == 0) {
    vec = getRandomVector(vec_size);
    vec1 = vec;
    start = MPI_Wtime();
  }
  ParallelOddEvenSort(&vec1);
  if (rank == 0) {
    end = MPI_Wtime();
    double ptime = end - start;
    std::cout << "P time " << ptime << std::endl;
    vec2 = vec;
    start = MPI_Wtime();
    SequentialOddEvenSort(&vec2);
    end = MPI_Wtime();
    double stime = end - start;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < vec_size; i++) {
      ASSERT_EQ(vec1[i], vec2[i]);
    }
  }
}

TEST(ODD_EVEN_SORT_MPI, TEST_SIZE_4096) {
  int rank, num;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  std::vector<int> vec, vec1, vec2;
  int vec_size = 4096;
  double start, end;

  if (rank == 0) {
    vec = getRandomVector(vec_size);
    vec1 = vec;
    // for (int elem : vec) {
    //  std::cout << elem << " ";
    //}
    // std::cout << std::endl;
    start = MPI_Wtime();
  }
  ParallelOddEvenSort(&vec1);
  if (rank == 0) {
    end = MPI_Wtime();
    double ptime = end - start;
    std::cout << "P time " << ptime << std::endl;
    vec2 = vec;
    start = MPI_Wtime();
    SequentialOddEvenSort(&vec2);
    end = MPI_Wtime();
    double stime = end - start;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < vec_size; i++) {
      ASSERT_EQ(vec1[i], vec2[i]);
    }
  }
}

TEST(ODD_EVEN_SORT_MPI, TEST_SIZE_5123) {
  int rank, num;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  std::vector<int> vec, vec1, vec2;
  int vec_size = 5123;
  double start, end;

  if (rank == 0) {
    vec = getRandomVector(vec_size);
    vec1 = vec;
    start = MPI_Wtime();
  }
  ParallelOddEvenSort(&vec1);
  if (rank == 0) {
    end = MPI_Wtime();
    double ptime = end - start;
    std::cout << "P time " << ptime << std::endl;
    vec2 = vec;
    start = MPI_Wtime();
    SequentialOddEvenSort(&vec2);
    end = MPI_Wtime();
    double stime = end - start;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < vec_size; i++) {
      ASSERT_EQ(vec1[i], vec2[i]);
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
