// Copyright 2021 Remizova Antonina
#include <gtest/gtest.h>
#include "../../../modules/task_1/remizova_a_min_value_matrix/min_value_matrix.h"
#include <gtest-mpi-listener.hpp>

TEST(parallel_min_elem_MPI, search_min_elem_in_msize_3x3) {
    int size = 0, rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> m;
    int c = 3;
    int r = 3;

    if (rank == 0) {
        m = getRandMatrix(c, r);
    }

    if ((c * r) < size) {
        EXPECT_ANY_THROW(minElemParallelOperations(m, c, r));
    } else {
        int glob_min = minElemParallelOperations(m, c, r);

        if (rank == 0) {
            int loc_min = minElemSequentialOperations(m, c, r);
            ASSERT_EQ(loc_min, glob_min);
        }
    }
}

TEST(parallel_min_elem_MPI, search_min_elem_in_msize_7x10) {
    int size = 0, rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> m;
    int c = 7;
    int r = 10;

    if (rank == 0) {
        m = getRandMatrix(c, r);
    }

    if ((c * r) < size) {
        EXPECT_ANY_THROW(minElemParallelOperations(m, c, r));
    } else {
        int glob_min = minElemParallelOperations(m, c, r);

        if (rank == 0) {
            int loc_min = minElemSequentialOperations(m, c, r);
            ASSERT_EQ(loc_min, glob_min);
        }
    }
}

TEST(parallel_min_elem_MPI, search_min_elem_in_msize_61x25) {
    int size = 0, rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> m;
    int c = 61;
    int r = 25;

    if (rank == 0) {
        m = getRandMatrix(c, r);
    }

    if ((c * r) < size) {
        EXPECT_ANY_THROW(minElemParallelOperations(m, c, r));
    } else {
        int glob_min = minElemParallelOperations(m, c, r);

        if (rank == 0) {
            int loc_min = minElemSequentialOperations(m, c, r);
            ASSERT_EQ(loc_min, glob_min);
        }
    }
}

TEST(parallel_min_elem_MPI, search_min_elem_in_msize_1x507) {
    int size = 0, rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> m;
    int c = 1;
    int r = 507;

    if (rank == 0) {
        m = getRandMatrix(c, r);
    }

    if ((c * r) < size) {
        EXPECT_ANY_THROW(minElemParallelOperations(m, c, r));
    } else {
        int glob_min = minElemParallelOperations(m, c, r);

        if (rank == 0) {
            int loc_min = minElemSequentialOperations(m, c, r);
            ASSERT_EQ(loc_min, glob_min);
        }
    }
}

TEST(parallel_min_elem_MPI, search_min_elem_in_msize_394x1) {
    int size = 0, rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> m;
    int c = 394;
    int r = 1;

    if (rank == 0) {
        m = getRandMatrix(c, r);
    }

    if ((c * r) < size) {
        EXPECT_ANY_THROW(minElemParallelOperations(m, c, r));
    } else {
        int glob_min = minElemParallelOperations(m, c, r);

        if (rank == 0) {
            int loc_min = minElemSequentialOperations(m, c, r);
            ASSERT_EQ(loc_min, glob_min);
        }
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
