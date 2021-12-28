// Copyright 2021 Ivina Anastasiya
#include <gtest/gtest.h>
#include <vector>
#include "./vector_out_of_order.h"
#include <gtest-mpi-listener.hpp>

TEST(out_of_order_count, vec_size_30) {
  int Rank;
  int len = 30;
  std::vector<int> vec(len);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    vec = randomValues(len);
  }
  MPI_Bcast(vec.data(), vec.size(), MPI_INT, 0, MPI_COMM_WORLD);
  int glCount = parallelCountViolation(vec);
  if (Rank == 0) {
    int count = countViolation(vec);
    ASSERT_EQ(count, glCount);
  }
}

TEST(out_of_order_count, vec_size_3000) {
  int Rank;
  int len = 3000;
  std::vector<int> vec(len);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    vec = randomValues(len);
  }
  MPI_Bcast(vec.data(), vec.size(), MPI_INT, 0, MPI_COMM_WORLD);
  int glCount = parallelCountViolation(vec);
  if (Rank == 0) {
    int count = countViolation(vec);
    ASSERT_EQ(count, glCount);
  }
}

TEST(out_of_order_count, vec_size_2) {
  int Rank;
  int len = 2;
  std::vector<int> vec(len);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    vec = randomValues(len);
  }
  MPI_Bcast(vec.data(), vec.size(), MPI_INT, 0, MPI_COMM_WORLD);
  int glCount = parallelCountViolation(vec);
  if (Rank == 0) {
    int count = countViolation(vec);
    ASSERT_EQ(count, glCount);
  }
}

TEST(out_of_order_count, vec_size_1) {
  int Rank;
  int len = 1;
  std::vector<int> vec(len);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    vec = randomValues(len);
  }
  MPI_Bcast(vec.data(), vec.size(), MPI_INT, 0, MPI_COMM_WORLD);
  int glCount = parallelCountViolation(vec);
  if (Rank == 0) {
    int count = countViolation(vec);
    ASSERT_EQ(count, glCount);
  }
}

TEST(out_of_order_count, vec_size_0) {
  int Rank;
  int len = 0;
  std::vector<int> vec(len);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    vec = randomValues(len);
  }
  MPI_Bcast(vec.data(), vec.size(), MPI_INT, 0, MPI_COMM_WORLD);
  int glCount = parallelCountViolation(vec);
  if (Rank == 0) {
    int count = countViolation(vec);
    ASSERT_EQ(count, glCount);
  }
}

TEST(equivalense_of_sequential_count, negative_vec) {
  int Rank;
  int len = 20;
  std::vector<int> vec(len);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

  if (Rank == 0) {
    vec = randomValues(len);
    for (int i = 0; i < len; i++) {
      vec[i] = -vec[i];
    }
  }
  MPI_Bcast(vec.data(), vec.size(), MPI_INT, 0, MPI_COMM_WORLD);
  int glCount = parallelCountViolation(vec);
  if (Rank == 0) {
    int count = countViolation(vec);
    ASSERT_EQ(count, glCount);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}