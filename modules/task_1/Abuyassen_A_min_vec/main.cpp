// Copyright 2021 Abuyassen Albara

#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "../../../modules/task_1/Abuyassen_A_min_vec/min_vec.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Operations_MPI, min_of_120) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_min = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_min = getSequentialOperations(global_vec);
        ASSERT_EQ(reference_min, global_min);
    }
}

TEST(Parallel_Operations_MPI, min_of_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 1000;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_min = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_min = getSequentialOperations(global_vec);
        ASSERT_EQ(reference_min, global_min);
    }
}

TEST(Parallel_Operations_MPI, min_of_50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 50;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_min = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_min = getSequentialOperations(global_vec);
        ASSERT_EQ(reference_min, global_min);
    }
}

TEST(Parallel_Operations_MPI, min_of_250) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 250;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_min = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_min = getSequentialOperations(global_vec);
        ASSERT_EQ(reference_min, global_min);
    }
}

TEST(Parallel_Operations_MPI, min_of_3000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 3000;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_min = getParallelOperations(global_vec, count_size_vector);

    if (rank == 0) {
        int reference_min = getSequentialOperations(global_vec);
        ASSERT_EQ(reference_min, global_min);
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
