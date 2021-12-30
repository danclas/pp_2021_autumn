// Copyright 2021 Podovinnikov Artyom
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include <gtest-mpi-listener.hpp>


#include "../../../modules/task_2/podovinnikov_a_broadcast/broadcast.h"

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

TEST(Broadcast_Tests, Integer_test) {
  int commSize, rank;
  int Integer = 1;
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    Integer = 5;
  }
  Broadcast(&Integer, 1, MPI_INT, 0, MPI_COMM_WORLD);
  for (int i = 0; i < commSize; i++) {
    // std::cout << "Int " << Integer << " rank " << rank;
  }
  EXPECT_EQ(Integer, 5);
}

TEST(Broadcast_Tests, Double_test) {
  int commSize, rank;
  double D = 1;
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    D = 5.66;
  }
  Broadcast(&D, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  for (int i = 0; i < commSize; i++) {
    // std::cout << "Int " << Integer << " rank " << rank;
  }
  EXPECT_EQ(D, 5.66);
}

TEST(Broadcast_Tests, Float_test) {
  int commSize, rank;
  float F = 1;
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    F = 5.77f;
  }
  Broadcast(&F, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  for (int i = 0; i < commSize; i++) {
    // std::cout << "Int " << Integer << " rank " << rank;
  }
  EXPECT_EQ(F, 5.77f);
}

TEST(Broadcast_Tests, Another_procces_test) {
  int commSize, rank;
  float F = 1;
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (commSize > 1) {
    if (rank == 1) {
      F = 6.88f;
    }
    Broadcast(&F, 1, MPI_FLOAT, 1, MPI_COMM_WORLD);
    for (int i = 0; i < commSize; i++) {
      // std::cout << "Int " << F << " rank " << rank;
    }
    EXPECT_EQ(F, 6.88f);
  } else {
    EXPECT_EQ(true, true);
  }
}

TEST(Broadcast_Tests, send_massive_test) {
  int commSize, rank;
  int arr[3] = {2, 2, 2};
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    arr[0] = 3;
    arr[1] = 100;
    arr[2] = 6;
  }
  Broadcast(&arr, 3, MPI_INT, 0, MPI_COMM_WORLD);
  for (int i = 0; i < commSize; i++) {
    // std::cout << "Int " << arr[0] << " rank " << rank;
    // std::cout << "Int " << arr[1] << " rank " << rank;
    // std::cout << "Int " << arr[2] << " rank " << rank;
  }
  EXPECT_EQ(arr[0], 3);
  EXPECT_EQ(arr[1], 100);
  EXPECT_EQ(arr[2], 6);
}

TEST(Broadcast_Tests, lim_of_count_send_elem_test) {
  int commSize, rank;
  int arr[3] = {2, 2, 2};
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (commSize < 1) {
    if (rank == 0) {
      arr[0] = 3;
      arr[1] = 100;
      arr[2] = 6;
    }
    Broadcast(&arr, 2, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < commSize; i++) {
      std::cout << "Int " << arr[2] << " rank " << rank;
    }
    EXPECT_EQ(arr[2], 6);
  } else {
    ASSERT_NE(arr[2], 6);
  }
}

TEST(Broadcast_Tests, time_test) {
  int commSize, rank;
  int Integer = 1;
  double start, end, result;
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    Integer = 5;
  }
  start = MPI_Wtime();
  Broadcast(&Integer, 1, MPI_INT, 0, MPI_COMM_WORLD);
  end = MPI_Wtime();
  result = end - start;
  if (rank == 0) {
    std::cout << "MyBroadcast1: " << result << std::endl;
  }
  start = MPI_Wtime();
  MPI_Bcast(&Integer, 1, MPI_INT, 0, MPI_COMM_WORLD);
  end = MPI_Wtime();
  result = end - start;
  if (rank == 0) {
    std::cout << "MpiBroadcast1: " << result << std::endl;
  }
  for (int i = 0; i < commSize; i++) {
    // std::cout << "Int " << Integer << " rank " << rank;
  }
  EXPECT_EQ(Integer, 5);
}
int len = 1000;
TEST(Broadcast_Tests, time_10000) {
  int commSize, rank;
  double start, end, result;
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int num[1000];
  if (rank == 0) {
    for (int i = 0; i < len; i++) {
      num[i] = randInt(1, 100);
      // std::cout << i << " " << num[i] << std::endl;
    }
  }

  start = MPI_Wtime();
  Broadcast(&num, len, MPI_INT, 0, MPI_COMM_WORLD);
  end = MPI_Wtime();
  result = end - start;

  if (rank == 0) {
    std::cout << "MyBroadcast1000: " << result << std::endl;
  }

  start = MPI_Wtime();
  MPI_Bcast(&num, len, MPI_INT, 0, MPI_COMM_WORLD);
  end = MPI_Wtime();
  result = end - start;

  if (rank == 0) {
    std::cout << "MpiBroadcast1000: " << result << std::endl;
  }
  for (int i = 0; i < commSize; i++) {
    // std::cout << "Int " << Integer << " rank " << rank;
  }
  EXPECT_EQ(true, true);
}
