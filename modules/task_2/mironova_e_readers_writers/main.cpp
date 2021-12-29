// Copyright 2021 Mironova Ekaterina
#include <gtest/gtest.h>
#include <vector>
#include "./readers_writers.h"
#include <gtest-mpi-listener.hpp>

TEST(readers_writers_MPI, writers_0_readers_0) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        manager(rank, 1);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    ASSERT_NO_THROW();
}

TEST(readers_writers_MPI, writers_0_readers_all) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        manager(rank, size);
    } else {
        reader(rank);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    ASSERT_NO_THROW();
}

TEST(readers_writers_MPI, writers_1_readers_other) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        manager(rank, size);
    } else {
        if (rank == 1) {
            writer(rank);
        } else {
            reader(rank);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    ASSERT_NO_THROW();
}

TEST(readers_writers_MPI,  readers_1_writers_other) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        manager(rank, size);
    } else {
        if (rank == 1) {
            reader(rank);
        } else {
            writer(rank);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    ASSERT_NO_THROW();
}

TEST(readers_writers_MPI, readers_0_writers_all) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        manager(rank, size);
    } else {
        writer(rank);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    ASSERT_NO_THROW();
}

TEST(readers_writers_MPI, equal_readers_writers) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        manager(rank, size);
    } else {
        if (rank % 2 == 0) {
            reader(rank);
        } else {
            writer(rank);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    ASSERT_NO_THROW();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    std::srand(std::time(nullptr));

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
