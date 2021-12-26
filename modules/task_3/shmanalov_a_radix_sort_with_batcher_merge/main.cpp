// Copyright 2021 Shmanalov Alexander
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./radix_sort_with_batcher_merge.h"
#include <gtest-mpi-listener.hpp>

TEST(radixSortWithMergeBatcher, vectorSize10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 10;
    std::vector<int> array = { 56, 12, 48, 32, 66, 11, 3, 27, 86, 112 };
    std::vector<int> reference = { 3, 11, 12, 27, 32, 48, 56, 66, 86, 112 };
    std::vector<int> parallelSort = parallelRadixSortMergeBatcher(array, size);
    if (rank == 0) {
        ASSERT_EQ(reference, parallelSort);
    }
}

TEST(radixSortWithMergeBatcher, randomVectorSize77) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array;
    int size = 77;
    if (rank == 0) {
        array = generationRandomVector(size);
    }
    std::vector<int> parallelSort = parallelRadixSortMergeBatcher(array, size);
    if (rank == 0) {
        std::vector<int> sequentialSort = sequentialRadixSort(array);
        ASSERT_EQ(sequentialSort, parallelSort);
    }
}

TEST(radixSortWithMergeBatcher, randomVectorSize808) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array;
    int size = 808;
    if (rank == 0) {
        array = generationRandomVector(size);
    }
    std::vector<int> parallelSort = parallelRadixSortMergeBatcher(array, size);
    if (rank == 0) {
        std::vector<int> sequentialSort = sequentialRadixSort(array);
        ASSERT_EQ(sequentialSort, parallelSort);
    }
}

TEST(radixSortWithMergeBatcher, randomVectorSize1065) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array;
    int size = 1065;
    if (rank == 0) {
        array = generationRandomVector(size);
    }
    std::vector<int> parallelSort = parallelRadixSortMergeBatcher(array, size);
    if (rank == 0) {
        std::vector<int> sequentialSort = sequentialRadixSort(array);
        ASSERT_EQ(sequentialSort, parallelSort);
    }
}

TEST(radixSortWithMergeBatcher, randomVectorSize10000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array;
    int size = 10000;
    // double startTime, finalTime, seqTime, parTime;
    if (rank == 0) {
        array = generationRandomVector(size);
        // startTime = MPI_Wtime();
    }
    std::vector<int> v1 = parallelRadixSortMergeBatcher(array, size);
    if (rank == 0) {
        /*finalTime = MPI_Wtime();
        parTime = finalTime - startTime;
        std::cout << "P = " << parTime << std::endl;
        startTime = MPI_Wtime();*/
        std::vector<int> v2 = sequentialRadixSort(array);
        /*finalTime = MPI_Wtime();
        seqTime = finalTime - startTime;
        std::cout << "S = " << seqTime << std::endl;
        std::cout << "Efficiency = " << seqTime / parTime << std::endl;*/
        ASSERT_EQ(v1, v2);
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
