// Copyright 2021 Kozel Svetlana
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/kozel_s_vector_differ_elements_value/vector_differ_elements_value.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, DEFAULT_FIND_DIFFER_ELEMENT_VALUES_VECTOR_SIZE_6_GET_VAL) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
    std::vector<int> vector = { 0, 1, 5, 4, 100, 50 };
    if (commRank == 0) {
        int maxDiffer = differElementsValue(vector);
        ASSERT_EQ(maxDiffer, 96);
    }
}

TEST(Parallel_Operations_MPI, PARALLEL_FIND_DIFFER_ELEMENT_VALUES_VECTOR_SIZE_3_PUT_NEGATIVE_AND_POSTITVE_GET_VAL) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
    std::vector<int> vector = { 55, -5055, 1};
    int maxDiffer1;
    if (commRank == 0) {
        maxDiffer1 = differElementsValue(vector);
    }

    int maxDiffer2 = paralleldifferElementsValue(vector, vector.size());
    if (commRank == 0) {
        ASSERT_EQ(maxDiffer1, maxDiffer2);
    }
}

TEST(Parallel_Operations_MPI, PARALLEL_FIND_DIFFER_ELEMENT_VALUES_EQUAL_ELEMENTS_IN_VECTOR_GET_VAL) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
    std::vector<int> vector = {5, 5, 5};

    int maxDiffer1 = paralleldifferElementsValue(vector, vector.size());
    if (commRank == 0) {
        ASSERT_EQ(maxDiffer1, 0);
    }
}

TEST(Parallel_Operations_MPI, PARALLEL_FIND_DIFFER_ELEMENT_VALUES_VECTOR_ERROR_SIZE_0_GET_VAL) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
    std::vector<int> vector = fillVector(0);
    int maxDiffer1;
    if (commRank == 0) {
        maxDiffer1 = differElementsValue(vector);
    }
    int maxDiffer2 = paralleldifferElementsValue(vector, vector.size());
    if (commRank == 0) {
        ASSERT_EQ(maxDiffer1, maxDiffer2);
    }
}

TEST(Parallel_Operations_MPI, PARALLEL_FIND_DIFFER_ELEMENT_VALUES_VECTOR_LARGE_SIZE_100000_GET_VAL) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
    std::vector<int> vector = fillVector(100000);
    int maxDiffer1;
    if (commRank == 0) {
        maxDiffer1 = differElementsValue(vector);
    }
    int maxDiffer2 = paralleldifferElementsValue(vector, vector.size());
    if (commRank == 0) {
        ASSERT_EQ(maxDiffer1, maxDiffer2);
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
