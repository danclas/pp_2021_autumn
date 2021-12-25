// Copyright 2021 Boyarskov Anton
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./image_smoothing.h"

TEST(Parallel_Gauss_MPI, Test_Radius_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int* image = nullptr;
  const int width = 500, height = 400;
  const int radius = 1;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  int* new_image_parallel = getParallelGauss(image, width, height, radius);

  if (rank == 0) {
    int* reference_new_image = getSequentialGauss(image, width, height, radius);

    for (int i = 0; i < (width - radius * 2) * (height - radius * 2);
         i += (width - radius * 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(Parallel_Gauss_MPI, Test_Radius_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int* image = nullptr;
  const int width = 500, height = 400;
  const int radius = 2;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  int* new_image_parallel = getParallelGauss(image, width, height, radius);

  if (rank == 0) {
    int* reference_new_image = getSequentialGauss(image, width, height, radius);

    for (int i = 0; i < (width - radius * 2) * (height - radius * 2);
         i += (width - radius * 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(Parallel_Gauss_MPI, Test_Radius_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int* image = nullptr;
  const int width = 500, height = 400;
  const int radius = 3;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  int* new_image_parallel = getParallelGauss(image, width, height, radius);

  if (rank == 0) {
    int* reference_new_image = getSequentialGauss(image, width, height, radius);

    for (int i = 0; i < (width - radius * 2) * (height - radius * 2);
         i += (width - radius * 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(Parallel_Gauss_MPI, Test_Radius_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int* image = nullptr;
  const int width = 500, height = 400;
  const int radius = 4;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  int* new_image_parallel = getParallelGauss(image, width, height, radius);

  if (rank == 0) {
    int* reference_new_image = getSequentialGauss(image, width, height, radius);

    for (int i = 0; i < (width - radius * 2) * (height - radius * 2);
         i += (width - radius * 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(Parallel_Gauss_MPI, Test_Radius_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double start, end, stime, ptime;

  int* image = nullptr;
  const int width = 500, height = 400;
  const int radius = 5;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  start = MPI_Wtime();
  int* new_image_parallel = getParallelGauss(image, width, height, radius);
  end = MPI_Wtime();

  if (rank == 0) {
    ptime = end - start;
    std::cout << "Parallel time: " << ptime << std::endl;

    start = MPI_Wtime();
    int* reference_new_image = getSequentialGauss(image, width, height, radius);
    end = MPI_Wtime();

    stime = end - start;
    std::cout << "Sequential time: " << stime << std::endl;

    std::cout << "Speedup: " << stime / ptime << std::endl;

    for (int i = 0; i < (width - radius * 2) * (height - radius * 2);
         i += (width - radius * 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
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
