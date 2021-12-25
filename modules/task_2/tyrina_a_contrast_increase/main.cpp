// Copyright 2021 Tyrina Anastasia
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./contrast_increase.h"

TEST(Parallel_Contrast_Increase_MPI, Test_Contrast_10) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double* image = nullptr;
  const int width = 200, height = 100;
  const double contrast = 10;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  double* new_image_parallel =
      getParallelContrastIncrease(image, width, height, contrast);

  if (rank == 0) {
    double* reference_new_image =
        getSequentialContrastIncrease(image, width, height, contrast);

    for (int i = 0; i < width * height; i += width) {
      // printf("-------\n");
      // printf("parallel: %f\n", new_image_parallel[i]);
      // printf("seq: %f\n", reference_new_image[i]);

      ASSERT_DOUBLE_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(Parallel_Contrast_Increase_MPI, Test_Contrast_20) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double* image = nullptr;
  const int width = 200, height = 100;
  const double contrast = 20;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  double* new_image_parallel =
      getParallelContrastIncrease(image, width, height, contrast);

  if (rank == 0) {
    double* reference_new_image =
        getSequentialContrastIncrease(image, width, height, contrast);

    for (int i = 0; i < width * height; i += width) {
      // printf("-------\n");
      // printf("parallel: %f\n", new_image_parallel[i]);
      // printf("seq: %f\n", reference_new_image[i]);

      ASSERT_DOUBLE_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(Parallel_Contrast_Increase_MPI, Test_Contrast_30) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double* image = nullptr;
  const int width = 200, height = 100;
  const double contrast = 30;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  double* new_image_parallel =
      getParallelContrastIncrease(image, width, height, contrast);

  if (rank == 0) {
    double* reference_new_image =
        getSequentialContrastIncrease(image, width, height, contrast);

    for (int i = 0; i < width * height; i += width) {
      // printf("-------\n");
      // printf("parallel: %f\n", new_image_parallel[i]);
      // printf("seq: %f\n", reference_new_image[i]);

      ASSERT_DOUBLE_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(Parallel_Contrast_Increase_MPI, Test_Contrast_40) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double* image = nullptr;
  const int width = 200, height = 100;
  const double contrast = 40;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  double* new_image_parallel =
      getParallelContrastIncrease(image, width, height, contrast);

  if (rank == 0) {
    double* reference_new_image =
        getSequentialContrastIncrease(image, width, height, contrast);

    for (int i = 0; i < width * height; i += width) {
      // printf("-------\n");
      // printf("parallel: %f\n", new_image_parallel[i]);
      // printf("seq: %f\n", reference_new_image[i]);

      ASSERT_DOUBLE_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(Parallel_Contrast_Increase_MPI, Test_Contrast_50) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double* image = nullptr;
  const int width = 200, height = 100;
  const double contrast = 50;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  double* new_image_parallel =
      getParallelContrastIncrease(image, width, height, contrast);

  if (rank == 0) {
    double* reference_new_image =
        getSequentialContrastIncrease(image, width, height, contrast);

    for (int i = 0; i < width * height; i += width) {
      // printf("-------\n");
      // printf("parallel: %f\n", new_image_parallel[i]);
      // printf("seq: %f\n", reference_new_image[i]);

      ASSERT_DOUBLE_EQ(new_image_parallel[i], reference_new_image[i]);
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
