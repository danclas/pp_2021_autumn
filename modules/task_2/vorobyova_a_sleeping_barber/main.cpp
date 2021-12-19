// Copyright 2021 Vorobyova Anna
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./sleeping_barber.h"

TEST(Barber, TEST_LINE_LENGTH_1) {
  fflush(stdout);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int line_length = 1;

  if (size > 1 && line_length < size) {
    if (rank == 0) {
      Barber(line_length, size - 1);
    } else {
      Client(rank);
    }
  }
  fflush(stdout);
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Barber, TEST_LINE_LENGTH_2) {
  fflush(stdout);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int line_length = 2;

  if (size > 1 && line_length < size) {
    if (rank == 0) {
      Barber(line_length, size - 1);
    } else {
      Client(rank);
    }
  }
  fflush(stdout);
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Barber, TEST_LINE_LENGTH_3) {
  fflush(stdout);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int line_length = 3;

  if (size > 1 && line_length < size) {
    if (rank == 0) {
      Barber(line_length, size - 1);
    } else {
      Client(rank);
    }
  }
  fflush(stdout);
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Barber, TEST_LINE_LENGTH_4) {
  fflush(stdout);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int line_length = 4;

  if (size > 1 && line_length < size) {
    if (rank == 0) {
      Barber(line_length, size - 1);
    } else {
      Client(rank);
    }
  }
  fflush(stdout);
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Barber, TEST_LINE_LENGTH_5) {
  fflush(stdout);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int line_length = 5;

  if (size > 1 && line_length < size) {
    if (rank == 0) {
      Barber(line_length, size - 1);
    } else {
      Client(rank);
    }
  }
  fflush(stdout);
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
