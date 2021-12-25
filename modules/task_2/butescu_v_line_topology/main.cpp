  // Copyright 2021 Butescu Vladimir
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_2/butescu_v_line_topology/butescu_v_line_topology.h"

TEST(Line_Topology_MPI, Create_Line_Topology) {
    int ProcNum, index, edges;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    if (ProcNum == 1 || ProcNum == 2) {
        ASSERT_TRUE(true);
        return;
    }

    MPI_Comm line_comm = create_line_topology(MPI_COMM_WORLD, ProcNum);
    MPI_Graphdims_get(line_comm, &index, &edges);
    ASSERT_EQ(index, ProcNum);
    ASSERT_EQ(edges, 2 * (ProcNum - 1));
}

TEST(Line_Topology_MPI, Check_0_Proc_Neighbords) {
    int ProcNum, neighbors_count;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm line_comm = create_line_topology(MPI_COMM_WORLD, ProcNum);
    MPI_Graph_neighbors_count(line_comm, 0, &neighbors_count);
    ASSERT_EQ(1, neighbors_count);
}

TEST(Line_Topology_MPI, Check_1_Proc_Neighbords) {
    int ProcNum, neighbors_count;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    if (ProcNum == 1 || ProcNum == 2) {
        ASSERT_TRUE(true);
        return;
    }

    MPI_Comm line_comm = create_line_topology(MPI_COMM_WORLD, ProcNum);
    MPI_Graph_neighbors_count(line_comm, 1, &neighbors_count);
    ASSERT_EQ(2, neighbors_count);
}

TEST(Line_Topology_MPI, Check_Left_Neighbour_Of_1_Process) {
    int ProcNum, ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    MPI_Comm line_comm = create_line_topology(MPI_COMM_WORLD, ProcNum);

    if (ProcNum == 1 || ProcNum == 2) {
        ASSERT_TRUE(true);
        return;
    }

    int neighbors_count = 2;
    int neighbors[2];
    MPI_Graph_neighbors(line_comm, 1, neighbors_count, neighbors);
    ASSERT_EQ(0, neighbors[0]);
}

TEST(Line_Topology_MPI, Check_Right_Neighbour_Of_1_Process) {
    int ProcNum, ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    MPI_Comm line_comm = create_line_topology(MPI_COMM_WORLD, ProcNum);

    if (ProcNum == 1 || ProcNum == 2) {
        ASSERT_TRUE(true);
        return;
    }

    int neighbors_count = 2;
    int neighbors[2];
    MPI_Graph_neighbors(line_comm, 1, neighbors_count, neighbors);
    ASSERT_EQ(2, neighbors[1]);
}

TEST(Line_Topology_MPI, Test_1_Proc_Sends_Data_To_2) {
    int ProcNum, ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcNum == 1 || ProcNum == 2) {
        ASSERT_TRUE(true);
        return;
    }

    MPI_Comm line_comm = create_line_topology(MPI_COMM_WORLD, ProcNum);
    if (ProcRank == 1) {
        int send_data = 0;
        MPI_Send(&send_data, 1, MPI_INT, 2, 0, line_comm);
    }

    if (ProcRank == 2) {
        int result_data;
        MPI_Status status;
        MPI_Recv(&result_data, 1, MPI_INT, 1, MPI_ANY_TAG, line_comm, &status);
        ASSERT_EQ(result_data, 0);
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
