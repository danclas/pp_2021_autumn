// Copyright 2021 Lazarev Alexey
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "../../../modules/task_2/lazarev_a_horiz_schem/horiz_schem.h"

TEST(Horiz_schem, Multiplication_1x1) {
  int size = 1;
  int ProcRank;
  std::vector<int> a(1, 10);
  std::vector<int> b(1, -3);
  std::vector<int> c;
  std::vector<int> flag(1, -30);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    c = multiplication(a, b, size);
    ASSERT_EQ(c, flag);
  }
}

TEST(Horiz_schem, Multiplication_16x16) {
  int size = 16;
  int ProcRank;
  std::vector<int> a, b, c;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    a = random_vector(size);
    b = random_vector(size);
    c = multiplication(a, b, size);
    // ASSERT_NO_THROW(true);
  }
}

TEST(Horiz_schem, Multiplication_and_parallel_1x1) {
  int size = 1;
  int ProcRank;
  std::vector<int> a(1, 10);
  std::vector<int> b(1, -3);
  std::vector<int> c;
  std::vector<int> C;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    c = multiplication(a, b, size);
  }
  C = multiplication_parallel(a, b, size);
  if (ProcRank == 0) {
    ASSERT_EQ(c, C);
  }
}

TEST(Horiz_schem, Multiplication_and_parallel_50x50) {
  int size = 50;
  int ProcRank;
  std::vector<int> a, b, c, C;
  double t1, t2, t3, t4;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    a = random_vector(size);
    b = random_vector(size);
    t1 = MPI_Wtime();
    c = multiplication(a, b, size);
    t2 = MPI_Wtime();
  }
  t3 = MPI_Wtime();
  C = multiplication_parallel(a, b, size);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    t4 = MPI_Wtime();
    printf("mult : %lf\n", t2 - t1);
    fflush(stdout);
    printf("schem : %lf\n", t4 - t3);
    fflush(stdout);
    ASSERT_EQ(c, C);
  }
}

TEST(Horiz_schem, Multiplication_and_parallel_458x458) {
  int size = 458;
  int ProcRank;
  std::vector<int> a, b, c, C;
  double t1, t2, t3, t4;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank == 0) {
    a = random_vector(size);
    b = random_vector(size);
    t1 = MPI_Wtime();
    c = multiplication(a, b, size);
    t2 = MPI_Wtime();
  }
  t3 = MPI_Wtime();
  C = multiplication_parallel(a, b, size);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    t4 = MPI_Wtime();
    printf("mult : %lf\n", t2 - t1);
    fflush(stdout);
    printf("schem : %lf\n", t4 - t3);
    fflush(stdout);
    ASSERT_EQ(c, C);
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
