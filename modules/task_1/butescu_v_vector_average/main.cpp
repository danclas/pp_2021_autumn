  // Copyright 2021 Butescu Vladimir
#include <gtest/gtest.h>
#include <vector>
#include "./butescu_v_vector_average.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_NEGATIVE_SIZE) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = -1;

    if (rank == 0)
        global_vec = getRandomPositiveVector(count_size_vector);

    double Savverage = getParallelAverage(global_vec, count_size_vector);

    if (rank == 0) {
        double Pavverage = getSequentialAverage(global_vec);
        ASSERT_EQ(Savverage, Pavverage);
    }
}

TEST(Parallel_Operations_MPI, Test_0_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 0;

    if (rank == 0)
        global_vec = getRandomPositiveVector(count_size_vector);

    double Savverage = getParallelAverage(global_vec, count_size_vector);

    if (rank == 0) {
        double Pavverage = getSequentialAverage(global_vec);
        ASSERT_EQ(Savverage, Pavverage);
    }
}

TEST(Parallel_Operations_MPI, Test_10_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 12;

    if (rank == 0)
        global_vec = getRandomPositiveVector(count_size_vector);

    double Savverage = getParallelAverage(global_vec, count_size_vector);

    if (rank == 0) {
        double Pavverage = getSequentialAverage(global_vec);
        ASSERT_EQ(Savverage, Pavverage);
    }
}

TEST(Parallel_Operations_MPI, Test_12_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 12;

    if (rank == 0)
        global_vec = getRandomPositiveVector(count_size_vector);

    double Savverage = getParallelAverage(global_vec, count_size_vector);

    if (rank == 0) {
        double Pavverage = getSequentialAverage(global_vec);
        ASSERT_EQ(Savverage, Pavverage);
    }
}

TEST(Parallel_Operations_MPI, Test_120_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0)
        global_vec = getRandomPositiveVector(count_size_vector);

    double Savverage = getParallelAverage(global_vec, count_size_vector);

    if (rank == 0) {
        double Pavverage = getSequentialAverage(global_vec);
        ASSERT_EQ(Savverage, Pavverage);
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
