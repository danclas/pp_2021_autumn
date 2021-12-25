// Copyright 2021 Vershinin Daniil
#include <gtest/gtest.h>

#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./vector_max_diff.h"

TEST(Sequential_Operations, Test_On_Own_Vector) {
  std::vector<int> vec = {1, 3, 4, 5};

  int reference_sum = getSequentialOperations(vec);
  ASSERT_EQ(reference_sum, 2);
}

TEST(Sequential_Operations, Test_On_Own_Vector2) {
  std::vector<int> vec = {1, -5, 2, 5};

  int reference_sum = getSequentialOperations(vec);
  ASSERT_EQ(reference_sum, 7);
}

TEST(Parallel_Operations_MPI, Test_On_Own_Vector) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;

  if (rank == 0) {
    global_vec = {1, 3, 4, 5};
  }

  int global_sum = getParallelOperations(global_vec);

  if (rank == 0) {
    int reference_sum = getSequentialOperations(global_vec);
    ASSERT_EQ(reference_sum, global_sum);
  }
}

TEST(Parallel_Operations_MPI, Test_On_Own_Vector2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;

  if (rank == 0) {
    global_vec = {1, -5, 2, 5};
  }

  int global_sum = getParallelOperations(global_vec);

  if (rank == 0) {
    int reference_sum = getSequentialOperations(global_vec);
    ASSERT_EQ(reference_sum, global_sum);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_Diff_Size_10) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 10;

  if (rank == 0) {
    global_vec = getRandomVector(count_size_vector);
  }

  int global_sum = getParallelOperations(global_vec);

  if (rank == 0) {
    int reference_sum = getSequentialOperations(global_vec);
    ASSERT_EQ(reference_sum, global_sum);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_Diff_Size_20) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 20;

  if (rank == 0) {
    global_vec = getRandomVector(count_size_vector);
  }

  int global_sum = getParallelOperations(global_vec);

  if (rank == 0) {
    int reference_sum = getSequentialOperations(global_vec);
    ASSERT_EQ(reference_sum, global_sum);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_Diff_Size_50) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 50;

  if (rank == 0) {
    global_vec = getRandomVector(count_size_vector);
  }

  int global_sum = getParallelOperations(global_vec);

  if (rank == 0) {
    int reference_sum = getSequentialOperations(global_vec);
    ASSERT_EQ(reference_sum, global_sum);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_Diff_Size_100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 100;

  if (rank == 0) {
    global_vec = getRandomVector(count_size_vector);
  }

  int global_sum = getParallelOperations(global_vec);

  if (rank == 0) {
    int reference_sum = getSequentialOperations(global_vec);
    ASSERT_EQ(reference_sum, global_sum);
  }
}

TEST(Parallel_Operations_MPI, Test_Max_Diff_Size_150) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 150;

  if (rank == 0) {
    global_vec = getRandomVector(count_size_vector);
  }

  int global_sum = getParallelOperations(global_vec);

  if (rank == 0) {
    int reference_sum = getSequentialOperations(global_vec);
    ASSERT_EQ(reference_sum, global_sum);
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
