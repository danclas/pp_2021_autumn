// Copyright 2021 Groshev Nikolay
#include <gtest/gtest.h>
#include <random>
#include <gtest-mpi-listener.hpp>

#include "../../../modules/task_2/groshev_n_reduce/reduce.h"

TEST(MyReduce, Sum) {
  int ProcNum, ProcRank;
  int my_value = 0, reduce_value = 0;
  int data = 10;
  double begin1, begin2, end1, end2;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  begin1 = MPI_Wtime();
  MPI_Reduce(&data, &my_value, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    end1 = MPI_Wtime();
  }
  begin2 = MPI_Wtime();
  my_reduce(&data, &reduce_value, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    end2 = MPI_Wtime();
    printf("result Reduce: %lf\n", end1 - begin1);
    fflush(stdout);
    printf("my result: %lf\n", end2 - begin2);
    fflush(stdout);
    ASSERT_EQ(reduce_value, my_value);
  }
}
TEST(MyReduce, Sum_Vector_2) {
  int ProcNum, ProcRank;
  int count = 2;
  int* reduce_value = new int[count];
  int* my_value = new int[count];
  int* data = new int[count];
  double begin1, begin2, end1, end2;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  for (int i = 0; i < count; i++) {
    my_value[i] = 0;
    data[i] = (i + 7) / 2;
  }
  begin1 = MPI_Wtime();
  MPI_Reduce(data, reduce_value, count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    end1 = MPI_Wtime();
  }
  begin2 = MPI_Wtime();
  my_reduce(data, my_value, count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    end2 = MPI_Wtime();
    printf("result Reduce: %lf\n", end1 - begin1);
    fflush(stdout);
    printf("my result: %lf\n", end2 - begin2);
    fflush(stdout);
    for (int i = 0; i < count; i++) {
      ASSERT_EQ(reduce_value[i], my_value[i]);
    }
  }
}
TEST(MyReduce, Multiplication_Vector_10) {
  int ProcNum, ProcRank;
  int count = 10;
  int* reduce_value = new int[count];
  int* my_value = new int[count];
  int* data = new int[count];
  double begin1, begin2, end1, end2;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  for (int i = 0; i < count; i++) {
    my_value[i] = 1;
    reduce_value[i] = 1;
    data[i] = (i + 7) / 2;
  }
  begin1 = MPI_Wtime();
  MPI_Reduce(data, reduce_value, count, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    end1 = MPI_Wtime();
  }
  begin2 = MPI_Wtime();
  my_reduce(data, my_value, count, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    end2 = MPI_Wtime();
    printf("result Reduce: %lf\n", end1 - begin1);
    fflush(stdout);
    printf("my result: %lf\n", end2 - begin2);
    fflush(stdout);
    for (int i = 0; i < count; i++) {
      ASSERT_EQ(reduce_value[i], my_value[i]);
    }
  }
}
TEST(MyReduce, Max_Vector_20) {
  int ProcNum, ProcRank;
  int count = 20;
  int* reduce_value = new int[count];
  int* my_value = new int[count];
  int* data = new int[count];
  double begin1, begin2, end1, end2;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  for (int i = 0; i < count; i++) {
    my_value[i] = -2147483647;
    reduce_value[i] = -2147483647;
    data[i] = (i + 7) / 2;
  }
  begin1 = MPI_Wtime();
  MPI_Reduce(data, reduce_value, count, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    end1 = MPI_Wtime();
  }
  begin2 = MPI_Wtime();
  my_reduce(data, my_value, count, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    end2 = MPI_Wtime();
    printf("result Reduce: %lf\n", end1 - begin1);
    fflush(stdout);
    printf("my result: %lf\n", end2 - begin2);
    fflush(stdout);
    for (int i = 0; i < count; i++) {
      ASSERT_EQ(reduce_value[i], my_value[i]);
    }
  }
}
TEST(MyReduce, Min_Vector_13) {
  int ProcNum, ProcRank;
  int count = 13;
  int* reduce_value = new int[count];
  int* my_value = new int[count];
  int* data = new int[count];
  double begin1, begin2, end1, end2;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  for (int i = 0; i < count; i++) {
    my_value[i] = 2147483647;
    reduce_value[i] = 2147483647;
    data[i] = (i + 7) / 2;
  }
  begin1 = MPI_Wtime();
  MPI_Reduce(data, reduce_value, count, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    end1 = MPI_Wtime();
  }
  begin2 = MPI_Wtime();
  my_reduce(data, my_value, count, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (ProcRank == 0) {
    end2 = MPI_Wtime();
    printf("result Reduce: %lf\n", end1 - begin1);
    fflush(stdout);
    printf("my result: %lf\n", end2 - begin2);
    fflush(stdout);
    for (int i = 0; i < count; i++) {
      ASSERT_EQ(reduce_value[i], my_value[i]);
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
