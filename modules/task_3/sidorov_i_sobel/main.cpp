// Copyright 2021 Sidorov Ilya
#include <gtest/gtest.h>
#include <vector>
#include "./sobel.h"
#include <gtest-mpi-listener.hpp>
TEST(SOBEL_MPI, test_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end;

  int* image = nullptr;
  const int width = 2223, height = 1556;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }
  start = MPI_Wtime();
  int* new_image_parallel = getParallelSobel(image, width, height);
  end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    start = MPI_Wtime();
    int* reference_new_image = getSequentialSobel(image, width, height);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "P time " << ptime << std::endl;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < (width - 2) * (height - 2);
      i += (width - 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(SOBEL_MPI, test_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end;

  int* image = nullptr;
  const int width = 100, height = 200;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }
  start = MPI_Wtime();
  int* new_image_parallel = getParallelSobel(image, width, height);
  end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    start = MPI_Wtime();
    int* reference_new_image = getSequentialSobel(image, width, height);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "P time " << ptime << std::endl;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < (width - 2) * (height - 2); i += (width - 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(SOBEL_MPI, test_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end;

  int* image = nullptr;
  const int width = 300, height = 500;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }
  start = MPI_Wtime();
  int* new_image_parallel = getParallelSobel(image, width, height);
  end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    start = MPI_Wtime();
    int* reference_new_image = getSequentialSobel(image, width, height);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "P time " << ptime << std::endl;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < (width - 2) * (height - 2); i += (width - 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(SOBEL_MPI, test_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end;

  int* image = nullptr;
  const int width = 333, height = 456;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }
  start = MPI_Wtime();
  int* new_image_parallel = getParallelSobel(image, width, height);
  end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    start = MPI_Wtime();
    int* reference_new_image = getSequentialSobel(image, width, height);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "P time " << ptime << std::endl;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < (width - 2) * (height - 2); i += (width - 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(SOBEL_MPI, test_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end;

  int* image = nullptr;
  const int width = 121, height = 434;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }
  start = MPI_Wtime();
  int* new_image_parallel = getParallelSobel(image, width, height);
  end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    start = MPI_Wtime();
    int* reference_new_image = getSequentialSobel(image, width, height);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "P time " << ptime << std::endl;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < (width - 2) * (height - 2); i += (width - 2)) {
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
