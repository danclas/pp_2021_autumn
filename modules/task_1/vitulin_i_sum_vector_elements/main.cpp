// Copyright 2021 Vitulin Ivan 381908-1
#include <gtest/gtest.h>
#include <vector>
#include "./sum_vector_elements.h"
#include <gtest-mpi-listener.hpp>

TEST(Sum_Vector_Elements_MPI, Test_1_vec_size_50_k_49) {
    int rank, k, sum_vect_elements;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vector;
    const int count_size_vec = 50;

    if (rank == 0) {
        k = 49;
        global_vector = getRandomVectorElements(count_size_vec, k);
        sum_vect_elements = getSumOfVectorElementsSequentially(global_vector);
    }

    int global_sum_elements = getSumOfVectorElementsParallelly(global_vector, count_size_vec);

    if (rank == 0) {
        ASSERT_EQ(sum_vect_elements, global_sum_elements);
    }
}

TEST(Sum_Vector_Elements_MPI, Test_2_vec_size_100_k_49) {
    int rank, k, sum_vect_elements;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vector;
    const int count_size_vec = 100;

    if (rank == 0) {
        k = 49;
        global_vector = getRandomVectorElements(count_size_vec, k);
        sum_vect_elements = getSumOfVectorElementsSequentially(global_vector);
    }

    int global_sum_elements = getSumOfVectorElementsParallelly(global_vector, count_size_vec);

    if (rank == 0) {
        ASSERT_EQ(sum_vect_elements, global_sum_elements);
    }
}

TEST(Sum_Vector_Elements_MPI, Test_3_vec_size_100_k_50) {
    int rank, k, sum_vect_elements;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vector;
    const int count_size_vec = 100;

    if (rank == 0) {
        k = 50;
        global_vector = getRandomVectorElements(count_size_vec, k);
        sum_vect_elements = getSumOfVectorElementsSequentially(global_vector);
    }

    int global_sum_elements = getSumOfVectorElementsParallelly(global_vector, count_size_vec);

    if (rank == 0) {
        ASSERT_EQ(sum_vect_elements, global_sum_elements);
    }
}

TEST(Sum_Vector_Elements_MPI, Test_4_vec_size_50_k_100) {
    int rank, k, sum_vect_elements;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vector;
    const int count_size_vec = 50;

    if (rank == 0) {
        k = 100;
        global_vector = getRandomVectorElements(count_size_vec, k);
        sum_vect_elements = getSumOfVectorElementsSequentially(global_vector);
    }

    int global_sum_elements = getSumOfVectorElementsParallelly(global_vector, count_size_vec);

    if (rank == 0) {
        ASSERT_EQ(sum_vect_elements, global_sum_elements);
    }
}

TEST(Sum_Vector_Elements_MPI, Test_5_vec_size_150_k_100) {
    int rank, k, sum_vect_elements;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vector;
    const int count_size_vec = 150;

    if (rank == 0) {
        k = 100;
        global_vector = getRandomVectorElements(count_size_vec, k);
        sum_vect_elements = getSumOfVectorElementsSequentially(global_vector);
    }

    int global_sum_elements = getSumOfVectorElementsParallelly(global_vector, count_size_vec);

    if (rank == 0) {
        ASSERT_EQ(sum_vect_elements, global_sum_elements);
    }
}

TEST(Sum_Vector_Elements_MPI, Test_6_vec_size_500_k_1000) {
    int rank, k, sum_vect_elements;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vector;
    const int count_size_vec = 500;

    if (rank == 0) {
        k = 1000;
        global_vector = getRandomVectorElements(count_size_vec, k);
        sum_vect_elements = getSumOfVectorElementsSequentially(global_vector);
    }

    int global_sum_elements = getSumOfVectorElementsParallelly(global_vector, count_size_vec);

    if (rank == 0) {
        ASSERT_EQ(sum_vect_elements, global_sum_elements);
    }
}

TEST(Sum_Vector_Elements_MPI, Test_7_vec_size_1000_k_1000) {
    int rank, k, sum_vect_elements;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vector;
    const int count_size_vec = 1000;

    if (rank == 0) {
        k = 1000;
        global_vector = getRandomVectorElements(count_size_vec, k);
        sum_vect_elements = getSumOfVectorElementsSequentially(global_vector);
    }

    int global_sum_elements = getSumOfVectorElementsParallelly(global_vector, count_size_vec);

    if (rank == 0) {
        ASSERT_EQ(sum_vect_elements, global_sum_elements);
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
