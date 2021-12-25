// Copyright 2021 Zarubin Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./bubble_sort_even_odd_algorithm.h"
#include <gtest-mpi-listener.hpp>


TEST(GENERATE_VECTOR, can_generate_random_vector) {
    std::vector<int> vector(5);
    ASSERT_NO_THROW(generateRandomVector(&vector, 5));
}

TEST(SEQUENTIAL_OPERATIONS, can_run_sequential_operations) {
    std::vector<int> vector(5);
    generateRandomVector(&vector, 5);
    ASSERT_NO_THROW(getSequentialOperations(vector.begin(), vector.end()));
}

TEST(SEQUENTIAL_OPERATIONS, correct_work_on_vector_even_size) {
    std::vector<int> vector{ 3, 2, 5, 0, 4, 2 };
    std::vector<int> expected_result{ 0, 2, 2, 3, 4, 5 };
    getSequentialOperations(vector.begin(), vector.end());
    ASSERT_EQ(vector, expected_result);
}

TEST(SEQUENTIAL_OPERATIONS, correct_work_on_vector_odd_size) {
    std::vector<int> vector{ 3, 2, 5, 0, 4 };
    std::vector<int> expected_result{ 0, 2, 3, 4, 5 };
    getSequentialOperations(vector.begin(), vector.end());
    ASSERT_EQ(vector, expected_result);
}

TEST(PARALLEL_OPERATIONS, can_run_parallel_operations) {
    std::vector<int> vector{ 3, 2, 5, 4, 0 };
    ASSERT_NO_THROW(getParallelOperations(vector, 5));
}

TEST(PARALLEL_OPERATIONS, correct_work_on_vector_even_size_v1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>::size_type size = 400;
    std::vector<int> vector(size, 0);

    if (rank == 0) {
        generateRandomVector(&vector, size);
    }

    std::vector<int> result = getParallelOperations(vector, size);

    if (rank == 0) {
        getSequentialOperations(vector.begin(), vector.end());
        ASSERT_EQ(vector, result);
    }
}

TEST(PARALLEL_OPERATIONS, correct_work_on_vector_even_size_v2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>::size_type size = 34;
    std::vector<int> vector(size, 0);

    if (rank == 0) {
        generateRandomVector(&vector, size);
    }

    std::vector<int> result = getParallelOperations(vector, size);

    if (rank == 0) {
        getSequentialOperations(vector.begin(), vector.end());
        ASSERT_EQ(vector, result);
    }
}

TEST(PARALLEL_OPERATIONS, correct_work_on_vector_odd_size_v1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>::size_type size = 41;
    std::vector<int> vector(size, 0);

    if (rank == 0) {
        generateRandomVector(&vector, size);
    }

    std::vector<int> result = getParallelOperations(vector, size);

    if (rank == 0) {
        getSequentialOperations(vector.begin(), vector.end());
        ASSERT_EQ(vector, result);
    }
}

TEST(PARALLEL_OPERATIONS, correct_work_on_vector_odd_size_v2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>::size_type size = 33;
    std::vector<int> vector(size, 0);

    if (rank == 0) {
        generateRandomVector(&vector, size);
    }

    std::vector<int> result = getParallelOperations(vector, size);

    if (rank == 0) {
        getSequentialOperations(vector.begin(), vector.end());
        ASSERT_EQ(vector, result);
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
