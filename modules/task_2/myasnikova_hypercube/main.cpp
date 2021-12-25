// Copyright 2021 Myasnikova Varvara

#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./myasnikova_hypercube.h"


TEST(Hypercube, Correctness_test_1) {
    ASSERT_ANY_THROW(CreateHypercube(1, 1));
}

TEST(Hypercube, Correctness_test_2) {
  int ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum == 8) {
    ASSERT_NO_THROW(CreateHypercube(3, 2));
  }
}

TEST(Hypercube, Can_create_hypercube_4_proc) {
  int ProcRank, ProcNum;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum == 4) {
    MPI_Comm h_comm = CreateHypercube(2, 2);

    if (ProcRank == 0) {
      ASSERT_TRUE(HypercubeCorrection(h_comm, 2, 2));
    }
  }
}

TEST(Hypercube, Can_create_hypercube_8_proc) {
  int ProcRank, ProcNum;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum == 8) {
    MPI_Comm h_comm = CreateHypercube(3, 2);

    if (ProcRank == 0) {
      ASSERT_TRUE(HypercubeCorrection(h_comm, 3, 2));
    }
  }
}

TEST(Hypercube, Can_create_hypercube_16_proc) {
    int ProcRank, ProcNum;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    if (ProcNum == 16) {
        MPI_Comm h_comm = CreateHypercube(4, 2);

        if (ProcRank == 0) {
            ASSERT_TRUE(HypercubeCorrection(h_comm, 4, 2));
        }
    }
}

TEST(Hypercube, Successful_data_transfer) {
    int ProcRank, ProcNum;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    if (ProcNum == 8) {
        MPI_Comm h_comm = CreateHypercube(3, 2);
        bool res = HypercubeData(h_comm, 3, 2);

        if (ProcRank == 0) {
            ASSERT_TRUE(res);
        }
    }
}

TEST(Hypercube, Unsuccessful_data_transfer) {
  int ProcRank, ProcNum;

  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum == 8) {
    MPI_Comm h_comm = CreateHypercube(3, 2);
    bool res = HypercubeData(h_comm, 3, 4);

    if (ProcRank == 0) {
      ASSERT_FALSE(res);
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
