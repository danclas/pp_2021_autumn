// Copyright 2021 Butescu Vladimir
#include <gtest/gtest.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>
#include "./butescu_v_radix_sort.h"
#include <gtest-mpi-listener.hpp>

TEST(RadixSortWithSimpleMerge, Test_Radix_Sort_With_Simple_Merge_Size_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 5;
    std::vector<double> vec1 = Get_Random_Vector(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = Parallel_Radix_Sort(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(RadixSortWithSimpleMerge, Test_Radix_Sort_With_Simple_Merge_Size_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 100;
    std::vector<double> vec1 = Get_Random_Vector(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = Parallel_Radix_Sort(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(RadixSortWithSimpleMerge, Test_Radix_Sort_With_Simple_Merge_Size_105) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 105;
    std::vector<double> vec1 = Get_Random_Vector(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = Parallel_Radix_Sort(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(RadixSortWithSimpleMerge, Test_Radix_Sort_With_Simple_Merge_Size_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1000;
    std::vector<double> vec1 = Get_Random_Vector(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = Parallel_Radix_Sort(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
    }
}

TEST(RadixSortWithSimpleMerge, Test_Radix_Sort_With_Simple_Merge_Size_1005) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sizeVec = 1005;
    std::vector<double> vec1 = Get_Random_Vector(sizeVec);
    std::vector<double> vec2 = vec1;
    vec1 = Parallel_Radix_Sort(vec1);
    if (rank == 0) {
        vec2 = radixSort(vec2);
        EXPECT_EQ(vec1, vec2);
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
