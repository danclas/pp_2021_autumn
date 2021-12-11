// Copyright 2021 Kamenev Ilya

#include <gtest/gtest.h>
#include "../../../modules/task_2/kamenev_i_broadcast/broadcast.h"
#include <gtest-mpi-listener.hpp>

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

TEST(BroadcastTest, IntSum) {
  int length = 100;
  int root = 0;
  int mpi_result = 0;
  int seq_sum = 0;
  std::vector<int> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<int>(length * size, 50);
    for (int i = 0; i < length * size; i++) {
      seq_sum += vec[i];
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_INT, root, MPI_SUM,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_sum, 1e-2);
    vec.clear();
  }
}

TEST(BroadcastTest, IntProd) {
  int length = 100;
  int root = 0;
  int mpi_result = 1;
  int seq_prod = 1;
  std::vector<int> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<int>(length * size, 3);
    for (int i = 0; i < length * size; i++) {
      seq_prod *= vec[i];
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_INT, root, MPI_PROD,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_prod, 1e-2);
    vec.clear();
  }
}


TEST(BroadcastTest, IntMin) {
  int length = 100;
  int root = 0;
  int mpi_result = 0;
  int seq_min = INT_MAX;
  std::vector<int> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<int>(length * size, 50);
    for (int i = 0; i < length * size; i++) {
      if (vec[i] < seq_min) {
        seq_min = vec[i];
      }
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_INT, root, MPI_MIN,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_min, 1e-2);
    vec.clear();
  }
}

TEST(BroadcastTest, IntMax) {
  int length = 100;
  int root = 0;
  int mpi_result = 0;
  int seq_max = INT_MIN;
  std::vector<int> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<int>(length * size, 50);
    for (int i = 0; i < length * size; i++) {
      if (vec[i] > seq_max) {
        seq_max = vec[i];
      }
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_INT, root, MPI_MAX,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_max, 1e-2);
    vec.clear();
  }
}

TEST(BroadcastTest, DoubleSum) {
  int length = 100;
  int root = 0;
  double mpi_result = 0;
  double seq_sum = 0;
  std::vector<double> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<double>(length * size, 50);
    for (int i = 0; i < length * size; i++) {
      seq_sum += vec[i];
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_DOUBLE, root, MPI_SUM,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_sum, 1e-2);
    vec.clear();
  }
}

TEST(BroadcastTest, DoubleProd) {
  int length = 100;
  int root = 0;
  double mpi_result = 0;
  double seq_prod = 1;
  std::vector<double> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<double>(length * size, 3);
    for (int i = 0; i < length * size; i++) {
      seq_prod *= vec[i];
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_DOUBLE, root, MPI_PROD,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_prod, 1e-2);
    vec.clear();
  }
}

TEST(BroadcastTest, DoubleMin) {
  int length = 100;
  int root = 0;
  double mpi_result = 0;
  double seq_min = DBL_MAX;
  std::vector<double> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<double>(length * size, 50);
    for (int i = 0; i < length * size; i++) {
      if (vec[i] < seq_min) {
        seq_min = vec[i];
      }
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_DOUBLE, root, MPI_MIN,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_min, 1e-2);
    vec.clear();
  }
}

TEST(BroadcastTest, DoubleMax) {
  int length = 100;
  int root = 0;
  double mpi_result = 0;
  double seq_max = DBL_MIN;
  std::vector<double> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<double>(length * size, 50);
    for (int i = 0; i < length * size; i++) {
      if (vec[i] > seq_max) {
        seq_max = vec[i];
      }
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_DOUBLE, root, MPI_MAX,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_max, 1e-2);
    vec.clear();
  }
}

TEST(BroadcastTest, FloatSum) {
  int length = 100;
  int root = 0;
  float mpi_result = 0;
  float seq_sum = 0;
  std::vector<float> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<float>(length * size, 50);
    for (int i = 0; i < length * size; i++) {
      seq_sum += vec[i];
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_FLOAT, root, MPI_SUM,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_sum, 1e-2);
    vec.clear();
  }
}

TEST(BroadcastTest, FloatProd) {
  int length = 100;
  int root = 0;
  float mpi_result = 0;
  float seq_prod = 1;
  std::vector<float> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<float>(length * size, 3);
    for (int i = 0; i < length * size; i++) {
      seq_prod *= vec[i];
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_FLOAT, root, MPI_PROD,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_prod, 1e-2);
    vec.clear();
  }
}

TEST(BroadcastTest, FloatMin) {
  int length = 100;
  int root = 0;
  float mpi_result = 0;
  float seq_min = FLT_MAX;
  std::vector<float> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<float>(length * size, 50);
    for (int i = 0; i < length * size; i++) {
      if (vec[i] < seq_min) {
        seq_min = vec[i];
      }
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_FLOAT, root, MPI_MIN,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_min, 1e-2);
    vec.clear();
  }
}

TEST(BroadcastTest, FloatMax) {
  int length = 100;
  int root = 0;
  float mpi_result = 0;
  float seq_max = FLT_MIN;
  std::vector<float> vec;
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == root) {
    vec = getRandomVector<float>(length * size, 50);
    for (int i = 0; i < length * size; i++) {
      if (vec[i] > seq_max) {
        seq_max = vec[i];
      }
    }
  }
  Bcast(vec.data(), &mpi_result, length, MPI_FLOAT, root, MPI_MAX,
        MPI_COMM_WORLD);
  if (rank == root) {
    EXPECT_NEAR(mpi_result, seq_max, 1e-2);
    vec.clear();
  }
}
