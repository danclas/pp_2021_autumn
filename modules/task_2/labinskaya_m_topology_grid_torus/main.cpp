// Copyright 2021 Labinskaya Marina
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./topology_grid_torus.h"

TEST(TEST_TOPOLOGY, can_create_grid_torus_comm) {
    int ProcNum, ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int dims[2] = { 0, 0 };
    MPI_Dims_create(ProcNum, 2, dims);

    MPI_Comm grid_torus_comm = creating_topology_grid_torus(dims[0], dims[1]);

    if (ProcRank == 0) {
        ASSERT_EQ(is_topology_grid_torus(grid_torus_comm, 2, dims), 1);
    }
}

TEST(TEST_TOPOLOGY, grid_torus_has_correct_size) {
    int ProcNum, ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int dims[2] = { 0, 0 };
    MPI_Dims_create(ProcNum, 2, dims);

    MPI_Comm grid_torus_comm = creating_topology_grid_torus(dims[0], dims[1]);

    std::vector<int>dims_real(2);
    std::vector<int>periods_real(2);
    std::vector<int>coords_real(2);

    MPI_Cart_get(grid_torus_comm, 2, dims_real.data(), periods_real.data(), coords_real.data());

    if (ProcRank == 0) {
        ASSERT_EQ((dims_real[0] == dims[0] && dims_real[1] == dims[1]), 1);
    }
}

TEST(TEST_TOPOLOGY, grid_torus_has_correct_coords) {
    int ProcNum, ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int dims[2] = { 0, 0 };
    MPI_Dims_create(ProcNum, 2, dims);
    MPI_Comm grid_torus_comm = creating_topology_grid_torus(dims[0], dims[1]);

    int coords[2] = { ProcRank / dims[1], ProcRank % dims[1]};

    std::vector<int>coords_real(2);

    MPI_Cart_coords(grid_torus_comm, ProcRank, 2, coords_real.data());

    ASSERT_EQ((coords_real[0] == coords[0] && coords_real[1] == coords[1]), 1);
}

TEST(TEST_TOPOLOGY, correct_paired_sending) {
    int ProcNum, ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int dims[2] = { 0, 0 };
    MPI_Dims_create(ProcNum, 2, dims);

    MPI_Comm grid_torus_comm = creating_topology_grid_torus(dims[0], dims[1]);

    int message = 5;
    int message_real = 0;

    if (ProcNum == 1) {
        ASSERT_TRUE(true);
    } else {
        int receiving_node[2] = { ProcNum / 2, ProcNum - 1 };
        if (ProcRank == 0) {
            message_real = 5;
            MPI_Send(&message_real, 1, MPI_INT, receiving_node[0], 1, grid_torus_comm);
            if (receiving_node[0] != receiving_node[1])
                MPI_Send(&message_real, 1, MPI_INT, receiving_node[1], 1, grid_torus_comm);
        } else {
            if ((ProcRank == receiving_node[0]) || (ProcRank == receiving_node[1])) {
                MPI_Status status;
                MPI_Recv(&message_real, 1, MPI_INT, 0, 1, grid_torus_comm, &status);
                ASSERT_EQ(message_real, message);
            }
        }
    }
}

TEST(TEST_TOPOLOGY, correct_collective_sending) {
    int ProcNum, ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int dims[2] = { 0, 0 };
    MPI_Dims_create(ProcNum, 2, dims);

    MPI_Comm grid_torus_comm = creating_topology_grid_torus(dims[0], dims[1]);

    int message = 0;
    if (ProcRank == 0) {
        message = 10;
    }
    MPI_Bcast(&message, 1, MPI_INT, 0, grid_torus_comm);
    int new_message = 2 * message;
    int res = 0;
    MPI_Reduce(&new_message, &res, 1, MPI_INT, MPI_SUM, 0, grid_torus_comm);
    if (ProcRank == 0) {
        int correct_res = 2 * message * ProcNum;
        ASSERT_EQ(res, correct_res);
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
