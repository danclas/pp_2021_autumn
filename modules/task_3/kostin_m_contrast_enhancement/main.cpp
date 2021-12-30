// Copyright 2021 Kostin Maksim
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./contrast_enhancement.h"

TEST(Parallel_Operations_MPI, 2_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int width = 4;
  int height = 2;
  int* image;
  image = nullptr;
  if (rank == 0) {
    image = getRandomImg(width, height);
  }
  int* par_img = ParallelLinHist(image, width, height);
  if (rank == 0) {
    int min = GetLocalMin(image, width, height);
    int max = GetLocalMax(image, width, height);
    int* seq_img = SequentialLinHist(image, width, height, min, max);
    for (int i = 0; i < width * height; i++) {
      ASSERT_EQ(par_img[i], seq_img[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, 5_7) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int width = 7;
  int height = 5;
  int* image;
  image = nullptr;
  if (rank == 0) image= getRandomImg(width, height);

  int* par_img = ParallelLinHist(image, width, height);
  if (rank == 0) {
    int min = GetLocalMin(image, width, height);
    int max = GetLocalMax(image, width, height);
    int* seq_img = SequentialLinHist(image, width, height, min, max);
    for (int i = 0; i < width * height; i++) {
      ASSERT_EQ(par_img[i], seq_img[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, 35_70) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int width = 70;
  int height = 35;
  int* image;
  image = nullptr;
  if (rank == 0) {
    image = getRandomImg(width, height);
  }
  int* par_img = ParallelLinHist(image, width, height);
  if (rank == 0) {
    int min = GetLocalMin(image, width, height);
    int max = GetLocalMax(image, width, height);
    int* seq_img = SequentialLinHist(image, width, height, min, max);
    for (int i = 0; i < width * height; i++) {
      ASSERT_EQ(par_img[i], seq_img[i]);
    }
  }
}

TEST(Parallel_Operations_MPI, 65_130) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int width = 130;
  int height = 65;
  int* image;
  image = nullptr;
  if (rank == 0) {
    image = getRandomImg(width, height);
  }
  int* par_img = ParallelLinHist(image, width, height);
  if (rank == 0) {
    int min = GetLocalMin(image, width, height);
    int max = GetLocalMax(image, width, height);
    int* seq_img = SequentialLinHist(image, width, height, min, max);
    for (int i = 0; i < width * height; i++) {
      ASSERT_EQ(par_img[i], seq_img[i]);
    }
  }
}
TEST(Parallel_Operations_MPI, 1_55) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int width = 55;
  int height = 1;
  int* image;
  image = nullptr;
  if (rank == 0) {
    image = getRandomImg(width, height);
  }
  int* par_img = ParallelLinHist(image, width, height);
  if (rank == 0) {
    int min = GetLocalMin(image, width, height);
    int max = GetLocalMax(image, width, height);
    int* seq_img = SequentialLinHist(image, width, height, min, max);
    for (int i = 0; i < width * height; i++) {
      ASSERT_EQ(par_img[i], seq_img[i]);
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
