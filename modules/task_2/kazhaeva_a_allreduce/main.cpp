// Copyright 2021 Kazhaeva Anastasia
#include <gtest/gtest.h>
#include <vector>
#include "./allreduce.h"
#include <gtest-mpi-listener.hpp>

TEST(MPI_Allreduce_1, Test_Sum) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vector1 = GetRandomVector<int>(100, 100);
  std::vector<int> vec1(vector1.size());
  std::vector<int> vec2(vector1.size());

  My_MPI_Allreduce(vector1.data(), vec1.data(),
    vector1.size(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(vector1.data(), vec2.data(),
    vector1.size(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(vec1, vec2);
  }
}

TEST(MPI_Allreduce_2, Test_MAX) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vector1 = GetRandomVector<int>(100, 100);
  std::vector<int> vec1(vector1.size());
  std::vector<int> vec2(vector1.size());

  My_MPI_Allreduce(vector1.data(), vec1.data(),
    vector1.size(), MPI_INT, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(vector1.data(), vec2.data(),
    vector1.size(), MPI_INT, MPI_MAX, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(vec1, vec2);
  }
}

TEST(MPI_Allreduce_3, Test_MIN) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vector1 = GetRandomVector<int>(100, 100);
  std::vector<int> vec1(vector1.size());
  std::vector<int> vec2(vector1.size());

  My_MPI_Allreduce(vector1.data(), vec1.data(),
    vector1.size(), MPI_INT, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(vector1.data(), vec2.data(),
    vector1.size(), MPI_INT, MPI_MIN, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(vector1, vec2);
  }
}

TEST(MPI_Allreduce_4, Test_Sum_DOUBLE) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> vector1 = GetRandomVector<double>(100, 100);
  std::vector<double> vec1(vector1.size());
  std::vector<double> vec2(vector1.size());

  My_MPI_Allreduce(vector1.data(), vec1.data(),
    vector1.size(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(vector1.data(), vec2.data(),
    vector1.size(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(vec1, vec2);
  }
}

TEST(MPI_Allreduce_5, Test_MAX_FLOAT) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<float> vector1 = GetRandomVector<float>(100, 100);
  std::vector<float> vec1(vector1.size());
  std::vector<float> vec2(vector1.size());

  My_MPI_Allreduce(vector1.data(), vec1.data(),
    vector1.size(), MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(vector1.data(), vec2.data(),
    vector1.size(), MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(vec1, vec2);
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
