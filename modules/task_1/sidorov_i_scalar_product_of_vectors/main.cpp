// Copyright 2021 Sidorov Ilya
#include <gtest/gtest.h>
#include <vector>
#include "./scalar_product_of_vectors.h"
#include <gtest-mpi-listener.hpp>



TEST(Parallel_scalar_product_MPI, Test_10) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int* global_vec1 = nullptr;
  const int count_size_vector = 10;
  int* global_vec2 = nullptr;

  if (rank == 0) {
    global_vec1 = getRandomVector(count_size_vector, 50);
    global_vec2 = getRandomVector(count_size_vector, 50);
  }

  int global_product = parallCulc(global_vec1, global_vec2, count_size_vector);

  if (rank == 0) {
    int reference_product =
        scalar_product(global_vec1, global_vec2, count_size_vector);

    ASSERT_EQ(reference_product, global_product);
  }
}

TEST(Parallel_scalar_product_MPI, Test_20) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int* global_vec1 = nullptr;
  const int count_size_vector = 20;
  int* global_vec2 = nullptr;

  if (rank == 0) {
    global_vec1 = getRandomVector(count_size_vector, 50);
    global_vec2 = getRandomVector(count_size_vector, 50);
  }

  int global_product = parallCulc(global_vec1, global_vec2, count_size_vector);

  if (rank == 0) {
    int reference_product =
        scalar_product(global_vec1, global_vec2, count_size_vector);

    ASSERT_EQ(reference_product, global_product);
  }
}

TEST(Parallel_scalar_product_MPI, Test_50) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int* global_vec1 = nullptr;
  const int count_size_vector = 50;
  int* global_vec2 = nullptr;

  if (rank == 0) {
    global_vec1 = getRandomVector(count_size_vector, 50);
    global_vec2 = getRandomVector(count_size_vector, 50);
  }

  int global_product = parallCulc(global_vec1, global_vec2, count_size_vector);

  if (rank == 0) {
    int reference_product =
        scalar_product(global_vec1, global_vec2, count_size_vector);

    ASSERT_EQ(reference_product, global_product);
  }
}

TEST(Parallel_scalar_product_MPI, Test_200) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int* global_vec1 = nullptr;
  const int count_size_vector = 200;
  int* global_vec2 = nullptr;

  if (rank == 0) {
    global_vec1 = getRandomVector(count_size_vector, 50);
    global_vec2 = getRandomVector(count_size_vector, 50);
  }

  int global_product = parallCulc(global_vec1, global_vec2, count_size_vector);

  if (rank == 0) {
    int reference_product =
        scalar_product(global_vec1, global_vec2, count_size_vector);

    ASSERT_EQ(reference_product, global_product);
  }
}

TEST(Parallel_scalar_product_MPI, Test_100) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int* global_vec1 = nullptr;
  const int count_size_vector = 100;
  int* global_vec2 = nullptr;

  if (rank == 0) {
    global_vec1 = getRandomVector(count_size_vector, 50);
    global_vec2 = getRandomVector(count_size_vector, 50);
  }

  int global_product = parallCulc(global_vec1, global_vec2, count_size_vector);

  if (rank == 0) {
    int reference_product =
        scalar_product(global_vec1, global_vec2, count_size_vector);

    ASSERT_EQ(reference_product, global_product);
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
