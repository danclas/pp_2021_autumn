// Copyright 2021 Pichugin Ilya
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <gtest-mpi-listener.hpp>


#include "./gather.h"

TEST(Gather_TEST, MPI_INT_TEST) {
  int Rank, Size, root;
  double T1, T2;

  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);

  int Elem_Count = 30 / Size;

  std::vector<int> input_vec(30);
  std::vector<int> res(30);
  std::vector<int> local(Elem_Count);

  if (Rank == 0) {
    input_vec = getRandomVector<int>(30);
    std::random_device dev;
    std::mt19937 gen(dev());

    root = gen() % Size;
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Scatter(input_vec.data(), Elem_Count, MPI_INT, local.data(), Elem_Count,
              MPI_INT, 0, MPI_COMM_WORLD);

  if (Rank == root) T1 = MPI_Wtime();

  My_New_Gather(local.data(), Elem_Count, MPI_INT, res.data(), Elem_Count,
                MPI_INT, root, MPI_COMM_WORLD);

  if (Rank == root) {
    T1 = MPI_Wtime() - T1;
    T2 = MPI_Wtime();
  }

  MPI_Gather(local.data(), Elem_Count, MPI_INT, res.data(), Elem_Count, MPI_INT,
             root, MPI_COMM_WORLD);

  if (Rank == root) {
    T2 = MPI_Wtime() - T2;
    ASSERT_LT(abs(T2 - T1), 1);
  }
}

TEST(Gather_TEST, MPI_FLOAT_TEST) {
  int Rank, Size, root;
  double T1, T2;

  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);

  int Elem_Count = 30 / Size;

  std::vector<float> input_vec(30);
  std::vector<float> res(30);
  std::vector<float> local(Elem_Count);

  if (Rank == 0) {
    input_vec = getRandomVector<float>(30);
    std::random_device dev;
    std::mt19937 gen(dev());
    root = gen() % Size;
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(input_vec.data(), Elem_Count, MPI_FLOAT, local.data(), Elem_Count,
              MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (Rank == root) T1 = MPI_Wtime();

  My_New_Gather(local.data(), Elem_Count, MPI_FLOAT, res.data(), Elem_Count,
                MPI_FLOAT, root, MPI_COMM_WORLD);

  if (Rank == root) {
    T1 = MPI_Wtime() - T1;
    T2 = MPI_Wtime();
  }

  MPI_Gather(local.data(), Elem_Count, MPI_FLOAT, res.data(), Elem_Count,
             MPI_FLOAT, root, MPI_COMM_WORLD);

  if (Rank == root) {
    T2 = MPI_Wtime() - T2;
    ASSERT_LT(abs(T2 - T1), 1);
  }
}

TEST(Gather_TEST, MPI_DOUBLE_TEST) {
  int Rank, Size, root;
  double T1, T2;

  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);

  int Elem_Count = 30 / Size;

  std::vector<double> input_vec(30);
  std::vector<double> res(30);
  std::vector<double> local(Elem_Count);

  if (Rank == 0) {
    input_vec = getRandomVector<double>(30);
    std::random_device dev;
    std::mt19937 gen(dev());
    root = gen() % Size;
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(input_vec.data(), Elem_Count, MPI_DOUBLE, local.data(),
              Elem_Count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (Rank == root) T1 = MPI_Wtime();

  My_New_Gather(local.data(), Elem_Count, MPI_DOUBLE, res.data(), Elem_Count,
                MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (Rank == root) {
    T1 = MPI_Wtime() - T1;
    T2 = MPI_Wtime();
  }

  MPI_Gather(local.data(), Elem_Count, MPI_DOUBLE, res.data(), Elem_Count,
             MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (Rank == root) {
    T2 = MPI_Wtime() - T2;

    ASSERT_LT(abs(T2 - T1), 1);
  }
}

TEST(Gather_TEST, MPI_INT_TEST_BIG) {
  int Rank, Size, root;
  double T1, T2;

  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);

  int Elem_Count = 150;

  std::vector<int> vec(Size * Elem_Count);
  std::vector<int> res(Size * Elem_Count);
  std::vector<int> local(Elem_Count);

  if (Rank == 0) {
    vec = getRandomVector<int>(Size * Elem_Count);
    std::random_device dev;
    std::mt19937 gen(dev());
    root = gen() % Size;
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(vec.data(), Elem_Count, MPI_INT, local.data(), Elem_Count,
              MPI_INT, 0, MPI_COMM_WORLD);

  if (Rank == root) T1 = MPI_Wtime();

  My_New_Gather(local.data(), Elem_Count, MPI_INT, res.data(), Elem_Count,
                MPI_INT, root, MPI_COMM_WORLD);

  if (Rank == root) {
    T1 = MPI_Wtime() - T1;
    T2 = MPI_Wtime();
  }

  MPI_Gather(local.data(), Elem_Count, MPI_INT, res.data(), Elem_Count, MPI_INT,
             root, MPI_COMM_WORLD);

  if (Rank == root) {
    T2 = MPI_Wtime() - T2;

    ASSERT_LT(abs(T2 - T1), 1);
  }
}

TEST(Gather_TEST, MPI_ERR) {
  int Rank, Size, root;

  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  MPI_Comm_size(MPI_COMM_WORLD, &Size);

  int Elem_Count1 = 40 / Size;
  int Elem_Count2 = 30 / Size;

  std::vector<int> input_vec(30);
  std::vector<int> res(30);
  std::vector<int> local(Elem_Count1);

  if (Rank == 0) {
    input_vec = getRandomVector<int>(30);
    std::random_device dev;
    std::mt19937 gen(dev());

    root = gen() % Size;
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (Rank == root) {
    if (Elem_Count1 != Elem_Count2) {
      ASSERT_ANY_THROW(My_New_Gather(local.data(), Elem_Count1, MPI_INT,
                                     res.data(), Elem_Count2, MPI_INT, root,
                                     MPI_COMM_WORLD));
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
