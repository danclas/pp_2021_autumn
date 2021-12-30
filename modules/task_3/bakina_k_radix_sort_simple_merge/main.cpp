// Copyright 2021 Bakina Ksenia
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./radix_sort_simple_merge.h"

TEST(radix_sort_check, check_merge) {
    int proc_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    if (proc_rank == 0) {
        std::vector<int> result(30);

        std::vector<int> a(10);
        for (int i = 0, j = 1; i < 10; i++, j += 2) {
            a[i] = j;
            result[i] = j;
        }

        std::vector<int> b(20);
        for (int i = 0, j = 0; i < 20; i++, j += 2) {
            b[i] = j;
            result[i + 10] = j;
        }

        std::vector<int> ab = merge(a, b);
        std::vector<int> ba = merge(b, a);
        std::sort(result.begin(), result.end());

        ASSERT_EQ(ab, result);
        ASSERT_EQ(ba, result);
    }
}

TEST(radix_sort_check, check_linear_radix_sort) {
    int proc_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    if (proc_rank == 0) {
        std::vector<int> a(260);
        get_random_vector(&a);

        std::vector<int> result = a;
        linear_radix_sort(&a);

        std::sort(result.begin(), result.end());
        ASSERT_EQ(a, result);
    }
}

TEST(radix_sort_check, check_parallel_radix_sort_small_vector) {
    int proc_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    std::vector<int> vector(1500);
    std::vector<int> p_s_vector, l_s_vector;

    double linear_time;
    double parallel_time;
    if (proc_rank == 0) {
        get_random_vector(&vector);
        l_s_vector = vector;
        linear_time = MPI_Wtime();
        linear_radix_sort(&l_s_vector);
        linear_time = MPI_Wtime() - linear_time;
    }

    if (proc_rank == 0) {
        parallel_time = MPI_Wtime();
    }
    p_s_vector = parallel_radix_sort(vector);
    if (proc_rank == 0) {
        parallel_time = MPI_Wtime() - parallel_time;
    }

    if (proc_rank == 0) {
        ASSERT_EQ(l_s_vector, p_s_vector);
        std::cout << "linear_time: " << linear_time << std::endl;
        std::cout << "parallel_time: " << parallel_time << std::endl;
        std::cout << "ratio: " << 100 - parallel_time / linear_time * 100 << "%" << std::endl;
    }
}

TEST(radix_sort_check, check_parallel_radix_sort_medium_vector) {
    int proc_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    std::vector<int> vector(10000);
    std::vector<int> p_s_vector, l_s_vector;

    double linear_time;
    double parallel_time;
    if (proc_rank == 0) {
        get_random_vector(&vector);
        l_s_vector = vector;
        linear_time = MPI_Wtime();
        linear_radix_sort(&l_s_vector);
        linear_time = MPI_Wtime() - linear_time;
    }

    if (proc_rank == 0) {
        parallel_time = MPI_Wtime();
    }
    p_s_vector = parallel_radix_sort(vector);
    if (proc_rank == 0) {
        parallel_time = MPI_Wtime() - parallel_time;
    }

    if (proc_rank == 0) {
        ASSERT_EQ(l_s_vector, p_s_vector);
        std::cout << "linear_time: " << linear_time << std::endl;
        std::cout << "parallel_time: " << parallel_time << std::endl;
        std::cout << "ratio: " << 100 - parallel_time / linear_time * 100 << "%" << std::endl;
    }
}

TEST(radix_sort_check, check_parallel_radix_sort_big_vector) {
    int proc_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    std::vector<int> vector(1000000);
    std::vector<int> p_s_vector, l_s_vector;

    double linear_time;
    double parallel_time;
    if (proc_rank == 0) {
        get_random_vector(&vector);
        l_s_vector = vector;
        linear_time = MPI_Wtime();
        linear_radix_sort(&l_s_vector);
        linear_time = MPI_Wtime() - linear_time;
    }

    if (proc_rank == 0) {
        parallel_time = MPI_Wtime();
    }
    p_s_vector = parallel_radix_sort(vector);
    if (proc_rank == 0) {
        parallel_time = MPI_Wtime() - parallel_time;
    }

    if (proc_rank == 0) {
        ASSERT_EQ(l_s_vector, p_s_vector);
        std::cout << "linear_time: " << linear_time<< std::endl;
        std::cout << "parallel_time: " << parallel_time<< std::endl;
        std::cout << "ratio: " << 100 - parallel_time / linear_time * 100 << "%" << std::endl;
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
