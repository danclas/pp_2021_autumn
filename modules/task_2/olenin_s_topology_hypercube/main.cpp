// Copyright 2021 Olenin Sergey
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./hypercube.h"

TEST(Hypercube_MPI, Test_Wrong_Arg_In_getDimsCount) {
  ASSERT_EQ(getDimensionCount(0), -0);
}

TEST(Hypercube_MPI, Test_procNum_Is_Not_Pow_Of_Two) {
  ASSERT_EQ(getDimensionCount(777), -1);
}

TEST(Hypercube_MPI, Test_Correct_getDimsCount) {
  EXPECT_EQ(getDimensionCount(32), 5);
}

TEST(Hypercube_MPI, Test_Wrong_Arg_In_createHypercubeTopology) {
  ASSERT_ANY_THROW(getHyperCube(0));
}

TEST(Hypercube_MPI, Test_Data_Transfer) {
  int numberProc, rankProc;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numberProc);

  if (numberProc == 1) {
    ASSERT_EQ(1, 1);

    return;
  }
  int dim_count = getDimensionCount(numberProc);

  if (dim_count == -1) {
    ASSERT_EQ(1, 1);

    return;
  }
  MPI_Comm hypercube = getHyperCube(dim_count);

  int dest = 0;
  int source = 1 % numberProc;

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
