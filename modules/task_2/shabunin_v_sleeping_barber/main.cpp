// Copyright 2021 Shabunin Vladislav
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./sleeping_barber.h"

TEST(SLEEPING_BARBER_MPI, TEST_One_seat) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    barber(rank, size - 2);
  } else if (rank == 1) {
    queue(1, size - 2);
  } else {
    client(rank);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(SLEEPING_BARBER_MPI, TEST_Two_seats) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    barber(rank, size - 2);
  } else if (rank == 1) {
    queue(2, size - 2);
  } else {
    client(rank);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(SLEEPING_BARBER_MPI, TEST_Three_seats) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    barber(rank, size - 2);
  } else if (rank == 1) {
    queue(3, size - 2);
  } else {
    client(rank);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(SLEEPING_BARBER_MPI, TEST_Four_seats) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    barber(rank, size - 2);
  } else if (rank == 1) {
    queue(4, size - 2);
  } else {
    client(rank);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(SLEEPING_BARBER_MPI, TEST_Five_seats) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (rank == 0) {
    barber(rank, size - 2);
  } else if (rank == 1) {
    queue(5, size - 2);
  } else {
    client(rank);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  std::srand(std::time(nullptr));

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
