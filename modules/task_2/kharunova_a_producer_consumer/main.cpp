// Copyright 2021 Kharunova Alina
#include <gtest/gtest.h>

#include "./producer_consumer.h"

#include <gtest-mpi-listener.hpp>

TEST(Test_Producer_Consumer, 1_consumer_entering) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start;
  int count, consumerEnterings = 1, producerEnterings = 2;
  int enteringCons = consumerEnterings * ((size - 1) / 2);
  int enteringProd = producerEnterings * (size / 2);

  if (rank == 0) {
    int enterings = enteringCons + enteringProd;
    start = MPI_Wtime();
    count = process(enterings);
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < consumerEnterings; i++) {
        consumer();
      }
    } else {
      for (int i = 0; i < producerEnterings; i++) {
        producer();
      }
    }
  }

  if (rank == 0) {
    int assert_value = enteringProd - enteringCons;
    double end = MPI_Wtime();
    double time = end - start;
    std::cout << time << std::endl;
    ASSERT_EQ(count, assert_value);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Test_Producer_Consumer, 2_consumer_enterings) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start;
  int count, consumerEnterings = 2, producerEnterings = 4;
  int enteringCons = consumerEnterings * ((size - 1) / 2);
  int enteringProd = producerEnterings * (size / 2);

  if (rank == 0) {
    int enterings = enteringProd + enteringCons;
    start = MPI_Wtime();
    count = process(enterings);
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < consumerEnterings; i++) {
        consumer();
      }
    } else {
      for (int i = 0; i < producerEnterings; i++) {
        producer();
      }
    }
  }

  if (rank == 0) {
    int assert_value = enteringProd - enteringCons;
    double end = MPI_Wtime();
    double time = end - start;
    std::cout << time << std::endl;
    ASSERT_EQ(count, assert_value);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Test_Producer_Consumer, 4_consumer_enterings) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start;
  int count, consumerEnterings = 4, producerEnterings = 4;
  int enteringCons = consumerEnterings * ((size - 1) / 2);
  int enteringProd = producerEnterings * (size / 2);

  if (rank == 0) {
    int enterings = enteringProd + enteringCons;
    start = MPI_Wtime();
    count = process(enterings);
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < consumerEnterings; i++) {
        consumer();
      }
    } else {
      for (int i = 0; i < producerEnterings; i++) {
        producer();
      }
    }
  }
  if (rank == 0) {
    int assert_value = enteringProd - enteringCons;
    double end = MPI_Wtime();
    double time = end - start;
    std::cout << time << std::endl;
    ASSERT_EQ(count, assert_value);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Test_Producer_Consumer, 5_consumers_enterings) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start;
  int count, consumerEnterings = 5, producerEnterings = 20;
  int enteringCons = consumerEnterings * ((size - 1) / 2);
  int enteringProd = producerEnterings * (size / 2);

  if (rank == 0) {
    int enterings = enteringProd + enteringCons;
    start = MPI_Wtime();
    count = process(enterings);
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < consumerEnterings; i++) {
        consumer();
      }
    } else {
      for (int i = 0; i < producerEnterings; i++) {
        producer();
      }
    }
  }
  if (rank == 0) {
    int assert_value = enteringProd - enteringCons;
    double end = MPI_Wtime();
    double time = end - start;
    std::cout << time << std::endl;
    ASSERT_EQ(count, assert_value);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Test_Producer_Consumer, 20_consumer_enterings) {
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double start;
  int count, consumerEnterings = 20, producerEnterings = 20;
  int enteringCons = consumerEnterings * ((size - 1) / 2);
  int enteringProd = producerEnterings * (size / 2);

  if (rank == 0) {
    int enterings = enteringProd + enteringCons;
    start = MPI_Wtime();
    count = process(enterings);
  }

  if (rank != 0) {
    if (rank % 2 == 0) {
      for (int i = 0; i < consumerEnterings; i++) {
        consumer();
      }
    } else {
      for (int i = 0; i < producerEnterings; i++) {
        producer();
      }
    }
  }
  if (rank == 0) {
    int assert_value = enteringProd - enteringCons;
    double end = MPI_Wtime();
    double time = end - start;
    std::cout << time << std::endl;
    ASSERT_EQ(count, assert_value);
  }
  MPI_Barrier(MPI_COMM_WORLD);
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
