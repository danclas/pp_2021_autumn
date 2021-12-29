// Copyright 2021 Kharunova Alina
#include <gtest/gtest.h>

#include "./producer_consumer.h"

#include <gtest-mpi-listener.hpp>

TEST(Test_Producer_Consumer, Test_1) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start;
  int count;
  int first = 2;
  int second = 2;

  if (rank == 0) {
    int value = second * (size / 2) + first * ((size - 1) / 2);
    start = MPI_Wtime();
    count = process(value);
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < first; i++) {
        consumer();
      }
    } else {
      for (int i = 0; i < second; i++) {
        producer();
      }
    }
  }

  if (rank == 0) {
    int assert_value = second * (size / 2) - first * ((size - 1) / 2);
    double end = MPI_Wtime();
    double time = end - start;
    std::cout << time << std::endl;
    ASSERT_EQ(count, assert_value);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Test_Producer_Consumer, Test_2) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start;
  int count;
  int first = 4;
  int second = 4;

  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) {
    int value = second * (size / 2) + first * ((size - 1) / 2);
    start = MPI_Wtime();
    count = process(value);
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < first; i++) {
        consumer();
      }
    } else {
      for (int i = 0; i < second; i++) {
        producer();
      }
    }
  }

  if (rank == 0) {
    int assert_value = second * (size / 2) - first * ((size - 1) / 2);
    double end = MPI_Wtime();
    double time = end - start;
    std::cout << time << std::endl;
    ASSERT_EQ(count, assert_value);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Test_Producer_Consumer, Test_3) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start;
  int count;
  int first = 2;
  int second = 4;

  if (rank == 0) {
    int value = second * (size / 2) + first * ((size - 1) / 2);
    start = MPI_Wtime();
    count = process(value);
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < first; i++) {
        consumer();
      }
    } else {
      for (int i = 0; i < second; i++) {
        producer();
      }
    }
  }
  if (rank == 0) {
    int assert_value = second * (size / 2) - first * ((size - 1) / 2);
    double end = MPI_Wtime();
    double time = end - start;
    std::cout << time << std::endl;
    ASSERT_EQ(count, assert_value);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Test_Producer_Consumer, Test_4) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start;
  int count;
  int first = 5;
  int second = 10;

  if (rank == 0) {
    int value = second * (size / 2) + first * ((size - 1) / 2);
    start = MPI_Wtime();
    count = process(value);
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < first; i++) {
        consumer();
      }
    } else {
      for (int i = 0; i < second; i++) {
        producer();
      }
    }
  }
  if (rank == 0) {
    int assert_value = second * (size / 2) - first * ((size - 1) / 2);
    double end = MPI_Wtime();
    double time = end - start;
    std::cout << time << std::endl;
    ASSERT_EQ(count, assert_value);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Test_Producer_Consumer, Test_5) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start;
  int count;
  int first = 20;
  int second = 20;

  if (rank == 0) {
    int value = second * (size / 2) + first * ((size - 1) / 2);
    start = MPI_Wtime();
    count = process(value);
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < first; i++) {
        consumer();
      }
    } else {
      for (int i = 0; i < second; i++) {
        producer();
      }
    }
  }
  if (rank == 0) {
    int assert_value = second * (size / 2) - first * ((size - 1) / 2);
    double end = MPI_Wtime();
    double time = end - start;
    std::cout << time << std::endl;
    ASSERT_EQ(count, assert_value);
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
