// Copyright 2021 Zotov Maksim
#include <gtest/gtest.h>
#include <vector>
#include "./bubble_sort_odd_even_algorithm.h"
#include <gtest-mpi-listener.hpp>

TEST(Bubble_sort_odd_even_algorithm_tets, sequential_sort_even_vector_size) {
    std::vector<int> vec{ 7, 5, 3, 0, 1, 4, 2, 6 };
    std::vector<int> sorted_vec{ 0, 1, 2, 3, 4, 5, 6, 7 };
    vec = BubbleSortSequential(vec);

    ASSERT_EQ(vec, sorted_vec);
}

TEST(Bubble_sort_odd_even_algorithm_tets, sequential_sort_odd_vector_size) {
    std::vector<int> vec{ 7, 5, 3, 0, 1, 4, 2 };
    std::vector<int> sorted_vec{ 0, 1, 2, 3, 4, 5, 7 };
    vec = BubbleSortSequential(vec);

    ASSERT_EQ(vec, sorted_vec);
}

TEST(Bubble_sort_odd_even_algorithm_tets, vector_size_100) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    const int size_vector = 100;
    std::vector<int> parallel_vec, sequential_vec, tmp_vec;

    if (ProcRank == 0) {
        parallel_vec = getRandomVector(size_vector);
        tmp_vec = parallel_vec;
    }

    parallel_vec = BubbleSortParallel(parallel_vec);

    if (ProcRank == 0) {
        sequential_vec = BubbleSortSequential(tmp_vec);
        ASSERT_EQ(sequential_vec, parallel_vec);
    }
}

TEST(Bubble_sort_odd_even_algorithm_tets, vector_size_1000) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    const int size_vector = 1000;
    std::vector<int> parallel_vec, sequential_vec, tmp_vec;

    if (ProcRank == 0) {
        parallel_vec = getRandomVector(size_vector);
        tmp_vec = parallel_vec;
    }

    parallel_vec = BubbleSortParallel(parallel_vec);

    if (ProcRank == 0) {
        sequential_vec = BubbleSortSequential(tmp_vec);
        ASSERT_EQ(sequential_vec, parallel_vec);
    }
}

TEST(Bubble_sort_odd_even_algorithm_tets, vector_size_10000) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    const int size_vector = 5000;
    std::vector<int> parallel_vec, sequential_vec, tmp_vec;

    if (ProcRank == 0) {
        parallel_vec = getRandomVector(size_vector);
        tmp_vec = parallel_vec;
    }

    parallel_vec = BubbleSortParallel(parallel_vec);

    if (ProcRank == 0) {
        sequential_vec = BubbleSortSequential(tmp_vec);
        ASSERT_EQ(sequential_vec, parallel_vec);
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
