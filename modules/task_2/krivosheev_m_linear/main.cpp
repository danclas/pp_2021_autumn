// Copyright 2021 Krivosheev Miron
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "../../../modules/task_2/krivosheev_m_linear/linear.h"

TEST(Linear_Topology, Test_Can_Create_Linear_Comm) {
    ASSERT_NO_THROW(linear_topology(MPI_COMM_WORLD));
}

TEST(Linear_Topology, Test_Topology_MPI_COMM_WORLD) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        EXPECT_FALSE(is_linear_topology(MPI_COMM_WORLD));
    }
}

TEST(Linear_Topology, Test_Topology_Is_Not_MPI_COMM_WORLD) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    MPI_Comm line_comm = linear_topology(MPI_COMM_WORLD);
    if (ProcRank == 0) {
        if (ProcNum == 1) {
            ASSERT_EQ(MPI_COMM_WORLD, line_comm);
        } else {
            ASSERT_NE(MPI_COMM_WORLD, line_comm);
            MPI_Comm_free(&line_comm);
        }
    }
}

TEST(Linear_Topology, Test_Graph_Topology) {
    int ProcRank, ProcNum, status;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    MPI_Comm line_comm = linear_topology(MPI_COMM_WORLD);
    if (ProcRank == 0) {
        if (ProcNum == 1) {
            MPI_Topo_test(line_comm, &status);
            ASSERT_EQ(status, MPI_UNDEFINED);
        } else {
            MPI_Topo_test(line_comm, &status);
            ASSERT_EQ(status, MPI_GRAPH);
            MPI_Comm_free(&line_comm);
        }
    }
}

TEST(Linear_Topology, Test_Line_Topology) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    MPI_Comm line_comm = linear_topology(MPI_COMM_WORLD);
    if (ProcNum == 1) {
        EXPECT_FALSE(is_linear_topology(line_comm));
    } else {
        EXPECT_TRUE(is_linear_topology(line_comm));
        MPI_Comm_free(&line_comm);
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
