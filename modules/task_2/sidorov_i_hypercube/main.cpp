// Copyright 2021 Sidorov Ilya
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./hypercube.h"

TEST(Hypercube_MPI, Test_wrong_argument_in_getLevel) {
  ASSERT_EQ(getLevel(-3), -0);
}

TEST(Hypercube_MPI, Test_NumProc_not_a_power_of_two) {
  ASSERT_EQ(getLevel(100), -1);
}

TEST(Hypercube_MPI, Test_NICE_getLevel) { EXPECT_EQ(getLevel(16), 4); }

TEST(Hypercube_MPI, Test_Wrong_wrong_argument_in_HyperCube) {
  ASSERT_ANY_THROW(hyperCube(0));
}

TEST(Hypercube_MPI, Test_Data_Transfer) {
  int numProc, rankProc;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  if (numProc == 1) {
    ASSERT_EQ(1, 1);
    return;
  }
  int dim_count = getLevel(numProc);

  if (dim_count == -1) {
    ASSERT_EQ(1, 1);
    return;
  }

  MPI_Comm hypercube = hyperCube(dim_count);
  int dest = 0;
  int source = 1 % numProc;

  int sendData = 666, receiveData = 777;

  if (dest == source) {
    ASSERT_EQ(1, 1);

    return;
  }
  if (rankProc == source) MPI_Send(&sendData, 1, MPI_INT, dest, 0, hypercube);
  if (rankProc == dest) {
    MPI_Status status;
    MPI_Recv(&receiveData, 1, MPI_INT, source, 0, hypercube, &status);
    ASSERT_EQ(sendData, receiveData);
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
