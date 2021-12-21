// Copyright 2021 Kraev Nikita
#include <gtest/gtest.h>
#include "./topology_ruler.h"
#include <gtest-mpi-listener.hpp>

TEST(checking_the_ruler_topology, number_of_neighbors_first_process) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm ruler = create_topology_ruler(MPI_COMM_WORLD);
    if (size < 2) {
        ASSERT_TRUE(true);
    } else {
        if (rank == 0) {
            int number_neighbors;
            MPI_Graph_neighbors_count(ruler, 0, &number_neighbors);
            ASSERT_EQ(1, number_neighbors);
        }
    }
}

TEST(checking_the_ruler_topology, number_of_neighbors_last_process) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm ruler = create_topology_ruler(MPI_COMM_WORLD);
    if (size < 2) {
        ASSERT_TRUE(true);
    } else {
        if (rank == size - 1) {
            int number_neighbors;
            MPI_Graph_neighbors_count(ruler, size - 1, &number_neighbors);
            ASSERT_EQ(1, number_neighbors);
        }
    }
}

TEST(checking_the_ruler_topology, number_of_neighbors_middle_process) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm ruler = create_topology_ruler(MPI_COMM_WORLD);
    if (size < 3) {
        ASSERT_TRUE(true);
    } else {
        if (rank == 1) {
            int number_neighbors;
            MPI_Graph_neighbors_count(ruler, 1, &number_neighbors);
            ASSERT_EQ(2, number_neighbors);
        }
    }
}

TEST(checking_the_ruler_topology, checking_neighboring_process) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm ruler = create_topology_ruler(MPI_COMM_WORLD);
    if (size < 5) {
        ASSERT_TRUE(true);
    } else {
        if (rank == 3) {
            int number_neighbors;
            MPI_Graph_neighbors_count(ruler, 3, &number_neighbors);
            int* neighbors = new int[number_neighbors];
            MPI_Graph_neighbors(ruler, 3, number_neighbors, neighbors);
            ASSERT_EQ(2, neighbors[0]);
            ASSERT_EQ(4, neighbors[1]);
        }
    }
}

TEST(checking_the_ruler_topology, transferring_data_to_the_last_process) {
  int size, rank;
  char result;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm ruler = create_topology_ruler(MPI_COMM_WORLD);
  if (size < 4) {
    ASSERT_TRUE(true);
  } else {
    if (rank == 0) {
      result = 'a';
      MPI_Send(&result, 1, MPI_CHAR, 3, 0, ruler);
    }
    if (rank == 3) {
      MPI_Recv(&result, 1, MPI_CHAR, 0, 0, ruler, &status);
      ASSERT_EQ('a', result);
    }
  }
}

TEST(checking_the_ruler_topology, transferring_data_to_the_first_process) {
  int size, rank;
  int result;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm ruler = create_topology_ruler(MPI_COMM_WORLD);
  if (size < 5) {
    ASSERT_TRUE(true);
  } else {
    if (rank == 4) {
      result = 5;
      MPI_Send(&result, 1, MPI_INT, 0, 0, ruler);
    }
    if (rank == 0) {
      MPI_Recv(&result, 1, MPI_INT, 4, 0, ruler, &status);
      ASSERT_EQ(5, result);
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
