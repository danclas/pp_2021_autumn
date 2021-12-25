// Copyright 2021 Kazhaeva Anastasia
#include <gtest/gtest.h>
#include <vector>
#include "./horizontal_filter.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_1) {
  int currentProcess;
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
  int height = 500;
  int weight = 400;
  std::vector<float> matrix;
  std::vector<float> img(height * weight);
  Kernell(&matrix, 3.0f);
  if (currentProcess == 0)
    img = RandomImg(weight, height);
  MPI_Bcast(img.data(), weight * height, MPI_FLOAT, 0, MPI_COMM_WORLD);
  std::vector<float> globalMatrix = Parallel(matrix, img, weight, height);
  if (currentProcess == 0) {
    std::vector<float> referenceMatrix;
    referenceMatrix = Sequential(matrix, img, weight, height);
    ASSERT_EQ(globalMatrix, referenceMatrix);
  }
}

TEST(Parallel_Operations_MPI, Test_2) {
  int currentProcess;
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
  int height = 200;
  int weight = 200;
  std::vector<float> matrix, img(height * weight);
  Kernell(&matrix, 3.0f);
  if (currentProcess == 0) {
    img = RandomImg(weight, height);
  }
  MPI_Bcast(img.data(), weight * height, MPI_FLOAT, 0, MPI_COMM_WORLD);

  std::vector<float> globalMatrix = Parallel(matrix, img, weight, height);

  if (currentProcess == 0) {
    std::vector<float> referenceMatrix = Sequential(matrix,
      img, weight, height);
    ASSERT_EQ(globalMatrix, referenceMatrix);
  }
}

TEST(Parallel_Operations_MPI, Test_3) {
  int currentProcess;
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
  int height = 500;
  int weight = 500;
  std::vector<float> matrix, img(height * weight);
  Kernell(&matrix, 3.0f);
  if (currentProcess == 0) {
    img = RandomImg(weight, height);
  }
  MPI_Bcast(img.data(), weight * height, MPI_FLOAT, 0, MPI_COMM_WORLD);

  std::vector<float> globalMatrix = Parallel(matrix, img, weight, height);

  if (currentProcess == 0) {
    std::vector<float> referenceMatrix = Sequential(matrix,
      img, weight, height);
    ASSERT_EQ(globalMatrix, referenceMatrix);
  }
}

TEST(Parallel_Operations_MPI, Test_4) {
  int currentProcess;
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
  int height = 100;
  int weight = 150;
  std::vector<float> matrix, img(height * weight);
  Kernell(&matrix, 3.0f);
  if (currentProcess == 0) {
    img = RandomImg(weight, height);
  }
  MPI_Bcast(img.data(), weight * height, MPI_FLOAT, 0, MPI_COMM_WORLD);

  std::vector<float> globalMatrix = Parallel(matrix, img, weight, height);

  if (currentProcess == 0) {
    std::vector<float> referenceMatrix = Sequential(matrix,
      img, weight, height);
    ASSERT_EQ(globalMatrix, referenceMatrix);
  }
}

TEST(Parallel_Operations_MPI, Test_5) {
  int currentProcess;
  MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
  int height = 100;
  int weight = 100;
  std::vector<float> matrix, img(height*weight);
  Kernell(&matrix, 3.0f);
  if (currentProcess == 0) {
    img = RandomImg(weight, height);
  }
  MPI_Bcast(img.data(), weight * height, MPI_FLOAT, 0, MPI_COMM_WORLD);

  std::vector<float> globalMatrix = Parallel(matrix, img, weight, height);

  if (currentProcess == 0) {
    std::vector<float> referenceMatrix = Sequential(matrix,
      img, weight, height);
    ASSERT_EQ(globalMatrix, referenceMatrix);
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
