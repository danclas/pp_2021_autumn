// Copyright [2021] <Borisov Maxim>

#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_2/borisov_m_scatter/scatter.h"
#include <gtest-mpi-listener.hpp>


TEST(MPI_parallel, my_scatter_gather_int_test) {
    int proc_rank;
    int proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    int size = 5 * proc_count;
    std::vector<int> src(size);
    std::vector<int> dest(size);
    std::vector<int> loc_src(5);
    if (proc_rank == 0) {
        for (int i = 0; i < size; i++)
            src[i] = i;
    }
    my_scatter(src.data(), 5, MPI_INT, loc_src.data(), 5, MPI_INT,
            0, MPI_COMM_WORLD);
    MPI_Gather(loc_src.data(), 5, MPI_INT, dest.data(), 5, MPI_INT,
            0, MPI_COMM_WORLD);
    if (proc_rank == 0) {
        EXPECT_EQ(src, dest);
    }
}

TEST(MPI_parallel, my_scatter_gather_double_test) {
    int proc_rank;
    int proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    int size = 5 * proc_count;
    std::vector<double> src(size);
    std::vector<double> dest(size);
    std::vector<double> loc_src(5);
    if (proc_rank == 0) {
        for (int i = 0; i < size; i++)
            src[i] = i;
    }
    my_scatter(src.data(), 5, MPI_DOUBLE, loc_src.data(), 5, MPI_DOUBLE,
            0, MPI_COMM_WORLD);
    MPI_Gather(loc_src.data(), 5, MPI_DOUBLE, dest.data(), 5, MPI_DOUBLE,
            0, MPI_COMM_WORLD);
    if (proc_rank == 0) {
        EXPECT_EQ(src, dest);
    }
}

TEST(MPI_parallel, my_scatter_gather_float_test) {
    int proc_rank;
    int proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    int size = 5 * proc_count;
    std::vector<float> src(size);
    std::vector<float> dest(size);
    std::vector<float> loc_src(5);
    if (proc_rank == 0) {
        for (int i = 0; i < size; i++)
            src[i] = i;
    }
    my_scatter(src.data(), 5, MPI_FLOAT, loc_src.data(), 5, MPI_FLOAT,
            0, MPI_COMM_WORLD);
    MPI_Gather(loc_src.data(), 5, MPI_FLOAT, dest.data(), 5, MPI_FLOAT,
            0, MPI_COMM_WORLD);
    if (proc_rank == 0) {
        EXPECT_EQ(src, dest);
    }
}

TEST(MPI_parallel, my_scatter_error_with_different_sizes) {
    int proc_rank;
    int proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    std::vector<int> a(5);
    std::vector<int> b(3);
    EXPECT_EQ(my_scatter(a.data(), 5, MPI_INT, b.data(), 10, MPI_INT,
                0, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_parallel, my_scatter_negative_root) {
    int proc_rank;
    int proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    std::vector<int> a(5);
    std::vector<int> b(5);
    EXPECT_EQ(my_scatter(a.data(), 5, MPI_INT, b.data(), 5, MPI_INT, -1, MPI_COMM_WORLD), MPI_ERR_COUNT);
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
