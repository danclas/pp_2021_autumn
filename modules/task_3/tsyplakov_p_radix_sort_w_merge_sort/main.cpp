// Copyright 2021 Tsyplakov Pavel
#include <gtest/gtest.h>
#include <vector>
#include "./radix_sort_w_merge_sort.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Sort_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 20;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }
    std::vector<double> global_vecc = ParallelRadixSort(global_vec,
                                                count_size_vector);

    if (rank == 0) {
        std::vector<double> reference_vec = SequentialRadixSort(global_vec);
        ASSERT_EQ(reference_vec, global_vecc);
    }
}

TEST(Parallel_Operations_MPI, Test_Sort_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 200;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }
    std::vector<double> global_vecc = ParallelRadixSort(global_vec,
                                                count_size_vector);

    if (rank == 0) {
        std::vector<double> reference_vec = SequentialRadixSort(global_vec);
        ASSERT_EQ(reference_vec, global_vecc);
    }
}
/*
TEST(Parallel_Operations_MPI, Test_Sort_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 400;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }
    std::vector<double> global_vecc = ParallelRadixSort(global_vec,
                                                count_size_vector);

    if (rank == 0) {
        std::vector<double> reference_vec = SequentialRadixSort(global_vec);
        ASSERT_EQ(reference_vec, global_vecc);
    }
}

TEST(Parallel_Operations_MPI, Test_Sort_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 800;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }
    std::vector<double> global_vecc = ParallelRadixSort(global_vec,
                                                count_size_vector);

    if (rank == 0) {
        std::vector<double> reference_vec = SequentialRadixSort(global_vec);
        ASSERT_EQ(reference_vec, global_vecc);
    }
}

TEST(Parallel_Operations_MPI, Test_Sort_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 1000;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }
    std::vector<double> global_vecc = ParallelRadixSort(global_vec,
                                                count_size_vector);

    if (rank == 0) {
        std::vector<double> reference_vec = SequentialRadixSort(global_vec);
        ASSERT_EQ(reference_vec, global_vecc);
    }
}
*/

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
