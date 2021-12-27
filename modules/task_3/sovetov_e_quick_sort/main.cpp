// Copyright 2021 Sovetov Egor
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./quick_sort.h"
#include <gtest-mpi-listener.hpp>

TEST(Quick_sort, Test_Wrong_Size_Random_Vector) {
  std::vector<int> wektorTees;
  ASSERT_ANY_THROW(wektorTees = GetWektorRandom(-100));
}

TEST(Quick_sort, Test_Init_Sort) {
  int bees;
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);

  if (bees == 0) {
    std::vector<int> massive = {108, -64, 22, 912,   -140,
                                84,  32,  10, -4222, 4866};
    std::vector<int> final = {-4222, -140, -64, 10, 22, 32, 84, 108, 912, 4866};
    QuickSortNO_MPI(&massive, 0, massive.size() - 1);
    ASSERT_EQ(massive, final);
  }
}

TEST(Quick_sort, Test_Random_Sort) {
  int bees;
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);

  if (bees == 0) {
    std::vector<int> wektorRandom;
    std::vector<int> massive = GetWektorRandom(100);
    QuickSortNO_MPI(&massive, 0, massive.size() - 1);
    EXPECT_TRUE(SortWektor(massive));
  }
}

TEST(Quick_sort, Test_Random_Sort_With_MPI) {
  int bees;
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);
  std::vector<int> massive(100);
  if (bees == 0) {
    massive = GetWektorRandom(100);
  }
  QuickSortMPI(&massive);

  if (bees == 0) {
    EXPECT_TRUE(SortWektor(massive));
  }
}

TEST(Quick_sort, Test_Random_Sort_With_MPI_Large_Size) {
  int bees;
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);
  std::vector<int> massive(100000);
  if (bees == 0) {
    massive = GetWektorRandom(100000);
  }
  QuickSortMPI(&massive);
  if (bees == 0) {
    EXPECT_TRUE(SortWektor(massive));
  }
}

TEST(Quick_sort, Test_Empty_Array) {
  int bees;
  MPI_Comm_rank(MPI_COMM_WORLD, &bees);

  if (bees == 0) {
    std::vector<int> massive(0);
    ASSERT_ANY_THROW(QuickSortNO_MPI(&massive, 0, massive.size() - 1));
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
