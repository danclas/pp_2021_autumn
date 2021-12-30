// Copyright 2021 Miheev Ivan
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "../../../modules/task_2/miheev_i_hypercube/hypercube.h"

TEST(Hypercube, Wrong_dimensions_hybercube) {
  ASSERT_ANY_THROW(hypercube(1, 2));
}

TEST(Hypercube, Wrong_size) { ASSERT_ANY_THROW(hypercube(2, 1)); }

TEST(Hypercube, Hypercube_Comm) {
  int ProcRank, ProcNum;
  int dimensions_hybercube = 3;
  int size = 2;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum == 8) {
    MPI_Comm hypercube_comm = hypercube(dimensions_hybercube, size);
    int test_dim;
    int status;
    std::vector<int> dims(dimensions_hybercube, size);
    MPI_Cartdim_get(hypercube_comm, &test_dim);
    if (test_dim != dimensions_hybercube) {
      throw std::logic_error("dims");
    }
    MPI_Topo_test(hypercube_comm, &status);
    if (status != MPI_CART) {
      throw std::logic_error("topo test");
    }
    if (ProcRank == 0) {
      ASSERT_TRUE(true);
    }
  } else {
    ASSERT_FALSE(false);
  }
}

TEST(Hypercube, Shift) {
  int ProcRank, ProcNum;
  int dimensions_hybercube = 3;
  int size = 2;
  int left, right;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  if (ProcNum == 8) {
    MPI_Comm hypercube_comm = hypercube(dimensions_hybercube, size);
    for (int i = 0; i < dimensions_hybercube; i++) {
      MPI_Cart_shift(hypercube_comm, i, 1, &left, &right);
      if (left == MPI_PROC_NULL || right == MPI_PROC_NULL) {
        throw std::logic_error("left or right");
      }
    }
    if (ProcRank == 0) {
      ASSERT_TRUE(true);
    }
  } else {
    ASSERT_FALSE(false);
  }
}

TEST(Hypercube_, Wrong_periods) {
  int ProcRank, ProcNum;
  int dimensions_hybercube = 3;
  int size = 2;
  MPI_Comm hypercube_comm;
  std::vector<int> dims(dimensions_hybercube, size);
  std::vector<int> periods(dimensions_hybercube, 0);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum == 8) {
    MPI_Cart_create(MPI_COMM_WORLD, dimensions_hybercube, dims.data(),
                    periods.data(), true, &hypercube_comm);
    MPI_Comm hypercube_comm2 = hypercube(dimensions_hybercube, size);
    if (hypercube_comm == hypercube_comm2) {
      throw std::logic_error("topo test");
    }
    if (ProcRank == 0) {
      ASSERT_TRUE(true);
    }
  } else {
    ASSERT_FALSE(false);
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
