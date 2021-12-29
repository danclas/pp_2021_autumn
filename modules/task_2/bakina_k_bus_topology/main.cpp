// Copyright 2021 Bakina Ksenia
#include <gtest/gtest.h>
#include <iostream>
#include <gtest-mpi-listener.hpp>
#include "./bus_topology.h"

TEST(bus_topology_check, can_create_bus_comm) {
    ASSERT_NO_THROW(get_Bus_Comm(MPI_COMM_WORLD));
}

TEST(bus_topology_check, check_proc_neighbors_count) {
    int proc_count;
    int proc_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    MPI_Comm bus_comm = get_Bus_Comm(MPI_COMM_WORLD);

    if (proc_rank == 0) {
        if (proc_count > 1) {
            int proc_neighbors_count;
            MPI_Graph_neighbors_count(bus_comm, 0, &proc_neighbors_count);
            ASSERT_EQ(1, proc_neighbors_count);
            MPI_Graph_neighbors_count(bus_comm, proc_count - 1, &proc_neighbors_count);
            ASSERT_EQ(1, proc_neighbors_count);
            for (int i = 1; i < proc_count - 1; i++) {
                MPI_Graph_neighbors_count(bus_comm, i, &proc_neighbors_count);
                ASSERT_EQ(2, proc_neighbors_count);
            }
        } else {
            ASSERT_EQ(true, true);
        }
        MPI_Comm_free(&bus_comm);
    }
}

TEST(bus_topology_check, neighbors_matching_check) {
    int proc_count;
    int proc_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    MPI_Comm bus_comm = get_Bus_Comm(MPI_COMM_WORLD);

    if (proc_rank == 0) {
        if (proc_count > 1) {
            int proc_neighbors_count;
            int* neighbors;
            for (int i = 0; i < proc_count; i++) {
                MPI_Graph_neighbors_count(bus_comm, i, &proc_neighbors_count);
                neighbors = new int[proc_neighbors_count];
                MPI_Graph_neighbors(bus_comm, i, proc_neighbors_count, neighbors);
                if (i == 0) {
                    ASSERT_EQ(1, neighbors[0]);
                } else {
                    if (i == proc_count - 1) {
                        ASSERT_EQ(i - 1, neighbors[0]);
                    } else {
                        ASSERT_TRUE((i-1 == neighbors[0] && i + 1 == neighbors[1]) ||
                            (i + 1 == neighbors[0] && i - 1 == neighbors[1]));
                    }
                }
                delete[] neighbors;
            }
        } else {
            ASSERT_TRUE(true);
        }
        MPI_Comm_free(&bus_comm);
    }
}

TEST(bus_topology_check, first_sent_message_to_last_proc) {
    int proc_count;
    int proc_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_count < 2) {
        ASSERT_TRUE(true);
        return;
    }

    MPI_Comm bus_comm = get_Bus_Comm(MPI_COMM_WORLD);

    int sent_message = 77;
    if (proc_rank == 0) {
        MPI_Send(&sent_message, 1, MPI_INT, proc_count - 1, 0, bus_comm);
    } else {
        if (proc_rank == proc_count - 1) {
            int received_message = 0;
            MPI_Status status;
            MPI_Recv(&received_message, 1, MPI_INT, 0, MPI_ANY_TAG, bus_comm, &status);
            ASSERT_EQ(received_message, sent_message);
            MPI_Comm_free(&bus_comm);
        }
    }
}

TEST(bus_topology_check, last_sent_message_to_first_proc) {
    int proc_count;
    int proc_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_count < 2) {
        ASSERT_TRUE(true);
        return;
    }

    MPI_Comm bus_comm = get_Bus_Comm(MPI_COMM_WORLD);

    int sent_message = 77;
    if (proc_rank == proc_count - 1) {
        MPI_Send(&sent_message, 1, MPI_INT, 0, 0, bus_comm);
    } else {
        if (proc_rank == 0) {
            int received_message = 0;
            MPI_Status status;
            MPI_Recv(&received_message, 1, MPI_INT, proc_count - 1, MPI_ANY_TAG, bus_comm, &status);
            ASSERT_EQ(received_message, sent_message);
            MPI_Comm_free(&bus_comm);
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    if (MPI_SUCCESS != MPI_Init(&argc, &argv)) {
        std::cout << "MPI_Init isn't MPI_Success" << std::endl;
        exit(0);
    }
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());
    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
