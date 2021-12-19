  // Copyright 2021 Kurguzikov Kirill

#include <gtest/gtest.h>
#include "./kurguzikov_striping_signs_vector.h"
#include <./gtest-mpi-listener.hpp>

TEST(Tets_parallel_and_sequential, sequential_test) {
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procRank == 0) {
    const int size = 10;
    int vect[size]{ 10, 20, -30, -40, 50, -60, -70, -80, 90, 100 };

    int sequentia_res = sequentialCount(vect, size);
    ASSERT_EQ(sequentia_res, 4);
  }
}

TEST(Tets_parallel_and_sequential, parallel_test) {
  int procRank;
  int size = 0;
  int* vect = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procRank == 0) {
    size = 10;
    vect = new int[size] { 10, 20, -30, -40, 50, -60, -70, -80, 90, 100 };
  }

  int parallel_res = parallelCount(vect, size);

  if (procRank == 0) {
    delete[] vect;
    ASSERT_EQ(4, parallel_res);
  }
}

TEST(Tets_parallel_and_sequential, vec_size_10) {
  int procRank;
  int size = 0;
  int* vect = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procRank == 0) {
    size = 10;
    vect = getRandomValue(size);
  }

  int parallel_res = parallelCount(vect, size);

  if (procRank == 0) {
    int sequentia_res = sequentialCount(vect, size);
    delete[] vect;
    ASSERT_EQ(sequentia_res, parallel_res);
  }
}

TEST(Tets_parallel_and_sequential, vec_size_50) {
  int procRank;
  int size = 0;
  int* vect = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procRank == 0) {
    size = 50;
    vect = getRandomValue(size);
  }

  int parallel_res = parallelCount(vect, size);

  if (procRank == 0) {
    int sequentia_res = sequentialCount(vect, size);
    delete[] vect;
    ASSERT_EQ(sequentia_res, parallel_res);
  }
}

TEST(Tets_parallel_and_sequential, vec_size_100) {
  int procRank;
  int size = 0;
  int* vect = nullptr;

  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procRank == 0) {
    size = 100;
    vect = getRandomValue(size);
  }

  int parallel_res = parallelCount(vect, size);

  if (procRank == 0) {
    int sequentia_res = sequentialCount(vect, size);
    delete[] vect;
    ASSERT_EQ(sequentia_res, parallel_res);
  }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
