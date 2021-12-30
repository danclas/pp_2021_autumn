// Copyright 2021 Krapivenskikh Stepan
#include <gtest/gtest.h>
#include "./star_topology.h"
#include <gtest-mpi-listener.hpp>

TEST(Star_Topology, Test_Create_Star_Topology_Data_10) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) {
        return;
    }

    MPI_Comm starComm = createStarTopology();
    int sendData = 10, recvData = 0;

    if (rank == 0) {
        recvData = 10;
        for (int i = 1; i < size; i++) {
            MPI_Send(&sendData, 1, MPI_INT, i, 0, starComm);
        }
    } else {
        MPI_Recv(&recvData, 1, MPI_INT, 0, 0, starComm, MPI_STATUS_IGNORE);
    }

    ASSERT_EQ(sendData, recvData);
}

TEST(Star_Topology, Test_Create_Star_Topology_Data_100) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) {
        return;
    }

    MPI_Comm starComm = createStarTopology();
    int sendData = 100, recvData = 0;

    if (rank == 0) {
        recvData = 100;
        for (int i = 1; i < size; i++) {
            MPI_Send(&sendData, 1, MPI_INT, i, 0, starComm);
        }
    } else {
        MPI_Recv(&recvData, 1, MPI_INT, 0, 0, starComm, MPI_STATUS_IGNORE);
    }

    ASSERT_EQ(sendData, recvData);
}

TEST(Star_Topology, Test_Create_Star_Topology_Data_1000) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) {
        return;
    }

    MPI_Comm starComm = createStarTopology();
    int sendData = 1000, recvData = 0;

    if (rank == 0) {
        recvData = 1000;
        for (int i = 1; i < size; i++) {
            MPI_Send(&sendData, 1, MPI_INT, i, 0, starComm);
        }
    } else {
        MPI_Recv(&recvData, 1, MPI_INT, 0, 0, starComm, MPI_STATUS_IGNORE);
    }

    ASSERT_EQ(sendData, recvData);
}

TEST(Star_Topology, Test_Create_Star_Topology_Data_10000) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) {
        return;
    }

    MPI_Comm starComm = createStarTopology();
    int sendData = 10000, recvData = 0;

    if (rank == 0) {
        recvData = 10000;
        for (int i = 1; i < size; i++) {
            MPI_Send(&sendData, 1, MPI_INT, i, 0, starComm);
        }
    } else {
        MPI_Recv(&recvData, 1, MPI_INT, 0, 0, starComm, MPI_STATUS_IGNORE);
    }

    ASSERT_EQ(sendData, recvData);
}

TEST(Star_Topology, Test_Create_Star_Topology_Data_100000) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) {
        return;
    }

    MPI_Comm starComm = createStarTopology();
    int sendData = 100000, recvData = 0;

    if (rank == 0) {
        recvData = 100000;
        for (int i = 1; i < size; i++) {
            MPI_Send(&sendData, 1, MPI_INT, i, 0, starComm);
        }
    } else {
        MPI_Recv(&recvData, 1, MPI_INT, 0, 0, starComm, MPI_STATUS_IGNORE);
    }

    ASSERT_EQ(sendData, recvData);
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
