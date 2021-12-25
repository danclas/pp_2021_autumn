// Copyright 2021 Bakina Ksenia
#include <gtest/gtest.h>
#include <iostream>
#include <gtest-mpi-listener.hpp>
#include "./max_matrix.h"

TEST(parallel_search_check, all_elements_equal) {
    int procRank = 0;
    const int row_count = 5;
    const int column_count = 10;
    int** matrix = nullptr;
    int element = 1;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix = new int*[row_count];
        for (int i = 0; i < row_count; i++) {
            matrix[i] = new int[column_count];
        }
        for (int i = 0; i < row_count; i++) {
            for (int j = 0; j < column_count; j++) {
                matrix[i][j] = element;
            }
        }
    }
    int verifiable_max = parallel_search_max_el(matrix, row_count, column_count);
    if (procRank == 0) {
        for (int i = 0; i < row_count; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        ASSERT_EQ(element, verifiable_max);
    }
}

TEST(parallel_search_check, different_increasing_elements) {
    int procRank = 0;
    const int row_count = 7;
    const int column_count = 15;
    int** matrix = nullptr;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix = new int*[row_count];
        for (int i = 0; i < row_count; i++) {
            matrix[i] = new int[column_count];
        }
        for (int i = 0; i < row_count; i++) {
            for (int j = 0; j < column_count; j++) {
                matrix[i][j] = i* column_count +j;
            }
        }
    }
    int verifiable_max = parallel_search_max_el(matrix, row_count, column_count);
    if (procRank == 0) {
        int referenceAlternations = matrix[row_count - 1][column_count - 1];
        for (int i = 0; i < row_count; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        ASSERT_EQ(referenceAlternations, verifiable_max);
    }
}

TEST(parallel_search_check, different_decreasing_elements) {
    int procRank = 0;
    const int row_count = 11;
    const int column_count = 4;
    int** matrix = nullptr;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix = new int* [row_count];
        for (int i = 0; i < row_count; i++) {
            matrix[i] = new int[column_count];
        }
        for (int i = 0; i < row_count; i++) {
            for (int j = 0; j < column_count; j++) {
                matrix[i][j] = (row_count - i) * 10 + (column_count - j);
            }
        }
    }
    int verifiable_max = parallel_search_max_el(matrix, row_count, column_count);
    if (procRank == 0) {
        int referenceAlternations = matrix[0][0];
        for (int i = 0; i < row_count; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        ASSERT_EQ(referenceAlternations, verifiable_max);
    }
}

TEST(parallel_search_check, random_elements_matrix_size_3000) {
    int procRank = 0;
    const int row_count = 150;
    const int column_count = 20;
    int** matrix = nullptr;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix = new int* [row_count];
        for (int i = 0; i < row_count; i++) {
            matrix[i] = new int[column_count];
        }
        for (int i = 0; i < row_count; i++) {
            for (int j = 0; j < column_count; j++) {
                matrix[i][j] = (row_count - i) * 10 + (column_count - j);
            }
        }
        get_random_matrix(matrix, row_count, column_count);
    }

    int verifiable_max = parallel_search_max_el(matrix, row_count, column_count);
    if (procRank == 0) {
        int referenceAlternations = matrix[0][0];
        for (int i = 0; i < row_count; i++) {
            for (int j = 0; j < column_count; j++) {
                if (matrix[i][j] > referenceAlternations)
                    referenceAlternations = matrix[i][j];
            }
        }
        for (int i = 0; i < row_count; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        ASSERT_EQ(referenceAlternations, verifiable_max);
    }
}

TEST(parallel_search_check, random_elements_matrix_size_200) {
    int procRank = 0;
    const int row_count = 10;
    const int column_count = 20;
    int** matrix = nullptr;

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix = new int* [row_count];
        for (int i = 0; i < row_count; i++) {
            matrix[i] = new int[column_count];
        }
        for (int i = 0; i < row_count; i++) {
            for (int j = 0; j < column_count; j++) {
                matrix[i][j] = (row_count - i) * 10 + (column_count - j);
            }
        }
        get_random_matrix(matrix, row_count, column_count);
    }
    int verifiable_max = parallel_search_max_el(matrix, row_count, column_count);
    if (procRank == 0) {
        int referenceAlternations = matrix[0][0];
        for (int i = 0; i < row_count; i++) {
            for (int j = 0; j < column_count; j++) {
                if (matrix[i][j] > referenceAlternations)
                    referenceAlternations = matrix[i][j];
            }
        }
        for (int i = 0; i < row_count; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        ASSERT_EQ(referenceAlternations, verifiable_max);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    if (MPI_SUCCESS != MPI_Init(&argc, &argv)) {
        std::cout << "MPI_Init isn't MPI_Success" << std::endl;
        exit(0);
    }
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());
    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
