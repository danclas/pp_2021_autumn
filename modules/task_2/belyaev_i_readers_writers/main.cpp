// Copyright 2021 Belyaev Ilya
#include <gtest/gtest.h>
#include "./readers_writers.h"
#include <gtest-mpi-listener.hpp>

TEST(r_w_MPI, no_one) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    storage(rank, 1);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

TEST(r_w_MPI, equally) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    storage(rank, size);
  } else {
    if (rank % 2 == 0) {
      readers(rank);
    } else {
      writers(rank);
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

TEST(r_w_MPI, only_w) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    storage(rank, size);
  } else {
    writers(rank);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

TEST(r_w_MPI, only_r) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    storage(rank, size);
  } else {
    readers(rank);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}
TEST(r_w_MPI, one_w) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    storage(rank, size);
  } else {
    if (rank == 1) {
      writers(rank);
    } else {
      readers(rank);
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

TEST(r_w_MPI, two_w) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    storage(rank, size);
  } else {
    if (rank == 2 || rank == 1) {
      writers(rank);
    } else {
      readers(rank);
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

TEST(r_w_MPI, One_r) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    storage(rank, size);
  } else {
    if (rank == 1) {
      readers(rank);
    } else {
      writers(rank);
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

TEST(r_w_MPI, two_r) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    storage(rank, size);
  } else {
    if (rank == 2 || rank == 1) {
      readers(rank);
    } else {
      writers(rank);
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
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
