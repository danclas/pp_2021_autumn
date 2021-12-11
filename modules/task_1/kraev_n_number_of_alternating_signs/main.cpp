// Copyright 2021 Kraev Nikita
#include <gtest/gtest.h>
#include <vector>
#include "./number_of_alternating_signs.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Generate_Random_Vector) {
    int size = 5;
    std::vector<int> vec(size);
    vec = getRandomVector(size);
    ASSERT_NO_THROW(vec);
}

TEST(Sequential_Operations_MPI, Test_Sequential_Algorithm) {
    std::vector<int> vec = {1, -1, 0, 2, 0};
    int result = 1;
    int def = findingNumberOfSignAlternations(vec);
    ASSERT_EQ(result, def);
}

TEST(Parallel_Operations_MPI, Test_Small_Size_Vector) {
    int rank, def;
    std::vector<int> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        vec = getRandomVector(100);
        def = findingNumberOfSignAlternations(vec);
    }

    int parallel = parallelFindingNumberOfSignAlternations(vec, 100);

    if (rank == 0) {
        ASSERT_EQ(def, parallel);
    }

}

TEST(Parallel_Operations_MPI, Test_Normal_Size_Vector) {
    int rank, def;
    std::vector<int> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        vec = getRandomVector(1000);
        def = findingNumberOfSignAlternations(vec);
    }

    int parallel = parallelFindingNumberOfSignAlternations(vec, 1000);

    if (rank == 0) {
        ASSERT_EQ(def, parallel);
    }
}

TEST(Parallel_Operations_MPI, Test_Big_Size_Vector) {
    int rank, def;
    std::vector<int> vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        vec = getRandomVector(10000);
        def = findingNumberOfSignAlternations(vec);
    }

    int parallel = parallelFindingNumberOfSignAlternations(vec, 10000);

    if (rank == 0) {
        ASSERT_EQ(def, parallel);
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
