// Copyright 2021 Vorobyova Anna
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./component_labeling.h"

TEST(COMPONENT_LABELING_PARALLEL, TEST_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<int> image;
  int n = 200, m = 300;

  if (rank == 0) {
    image = getRandomImage(n, m);
  }

  pair<vector<int>, int> parallel_result =
      ComponentLabelingParallel(image, n, m);

  if (rank == 0) {
    pair<vector<int>, int> reference_result = ComponentLabeling(image, n, m);
    ASSERT_EQ(parallel_result.second, reference_result.second);
  }
}

TEST(COMPONENT_LABELING_PARALLEL, TEST_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<int> image;
  int n = 500, m = 600;

  if (rank == 0) {
    image = getRandomImage(n, m);
  }

  pair<vector<int>, int> parallel_result =
      ComponentLabelingParallel(image, n, m);

  if (rank == 0) {
    pair<vector<int>, int> reference_result = ComponentLabeling(image, n, m);
    ASSERT_EQ(parallel_result.second, reference_result.second);
  }
}

TEST(COMPONENT_LABELING_PARALLEL, TEST_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<int> image;
  int n = 400, m = 170;

  if (rank == 0) {
    image = getRandomImage(n, m);
  }

  pair<vector<int>, int> parallel_result =
      ComponentLabelingParallel(image, n, m);

  if (rank == 0) {
    pair<vector<int>, int> reference_result = ComponentLabeling(image, n, m);
    ASSERT_EQ(parallel_result.second, reference_result.second);
  }
}

TEST(COMPONENT_LABELING_PARALLEL, TEST_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<int> image;
  int n = 239, m = 547;

  if (rank == 0) {
    image = getRandomImage(n, m);
  }

  pair<vector<int>, int> parallel_result =
      ComponentLabelingParallel(image, n, m);

  if (rank == 0) {
    pair<vector<int>, int> reference_result = ComponentLabeling(image, n, m);
    ASSERT_EQ(parallel_result.second, reference_result.second);
  }
}

TEST(COMPONENT_LABELING_PARALLEL, TEST_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<int> image;
  int n = 1300, m = 600;

  if (rank == 0) {
    image = getRandomImage(n, m);
  }

  double start = MPI_Wtime();
  pair<vector<int>, int> parallel_result =
      ComponentLabelingParallel(image, n, m);
  double end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;

    start = MPI_Wtime();
    pair<vector<int>, int> reference_result = ComponentLabeling(image, n, m);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << stime / ptime << std::endl;

    ASSERT_EQ(parallel_result.second, reference_result.second);
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
