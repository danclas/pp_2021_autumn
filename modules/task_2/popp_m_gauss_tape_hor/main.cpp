// Copyright 2021 Popp Maksim
#include <gtest/gtest.h>
#include <vector>
#include "./gauss_tape_hor.h"
#include <gtest-mpi-listener.hpp>


TEST(MATRIX_GENERATE, generate_random_sle) {
    std::vector<double> matrix (10 * 10);
    std::vector<double> vec (10);
    ASSERT_NO_THROW(sleGenerate(&matrix, &vec, 10));
}

TEST(SEQUENTIAL, run_sequential_gauss) {
    std::vector<double> matrix = { 1, 2, 3,
                                   5, 6, 7,
                                   9, 1, 11 };
    std::vector<double> vec{ 1, 2, 4 };
    ASSERT_NO_THROW(sequential(matrix, vec, 3));
}

TEST(SEQUENTIAL, sequential_works) {
    std::vector<double> matrix = { 1, 2, 3,
                                   5, 6, 7,
                                   9, 1, 11 };
    std::vector<double> vec{ 1, 2, 4 };

    auto solution = sequential(matrix, vec, 3);
    std::vector<double> expected_sln = { -5. / 72., -1. / 9., 31. / 72. };

    for (std::size_t i = 0; i < solution.size(); i++) {
        ASSERT_NEAR(solution[i], expected_sln[i], 0.001);
    }
}

TEST(SEQUENTIA, sequential_works_with_random_sle) {
    const std::size_t num = 17;
    std::vector<double> matrix(num * num);
    std::vector<double> vec(num);

    auto expected_sln = sleGenerate(&matrix, &vec, num);
    auto sln = sequential(matrix, vec, num);

    for (std::size_t i = 0; i < sln.size(); i++) {
        ASSERT_NEAR(sln[i], expected_sln[i], 0.001);
    }
}

TEST(PARALLEL, run_parallel) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const std::size_t num = 9;
    std::vector<double> matrix(num * num);
    std::vector<double> vec(num);

    if (rank == 0) {
        sleGenerate(&matrix, &vec, num);
    }

    ASSERT_NO_THROW(parallel(matrix, vec, num));
}

TEST(PARALLEL, parallel__work_matrix_even_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const std::size_t num = 20;
    std::vector<double> matrix(num * num);
    std::vector<double> vec(num);

    if (rank == 0) {
        sleGenerate(&matrix, &vec, num);
    }

    auto parallel_sln = parallel(matrix, vec, num);

    if (rank == 0) {
        auto sequential_sln = sequential(matrix, vec, num);
        for (std::size_t i = 0; i < num; i++)
            ASSERT_NEAR(parallel_sln[i], sequential_sln[i], 0.001);
    }
}

TEST(PARALLEL, parallel_work_matrix_uneven_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const std::size_t num = 19;
    std::vector<double> matrix(num * num);
    std::vector<double> vec(num);

    if (rank == 0) {
        sleGenerate(&matrix, &vec, num);
    }

    auto parallel_sln = parallel(matrix, vec, num);

    if (rank == 0) {
        auto sequential_sln = sequential(matrix, vec, num);
        for (std::size_t i = 0; i < num; i++)
            ASSERT_NEAR(parallel_sln[i], sequential_sln[i], 0.001);
    }
}

/*TEST(PARALLEL, parallel_boost_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const std::size_t num = 1000;
    std::vector<double> matrix(num * num);
    std::vector<double> vec(num);
    if (rank == 0) {
        sleGenerate(&matrix, &vec, num);
    }
    auto launch_p = MPI_Wtime();
    auto parallel_sln = parallel(matrix, vec, num);
    auto finish_p = MPI_Wtime();
    if (rank == 0) {
        auto launch_s = MPI_Wtime();
        auto sequential_sln = sequential(matrix, vec, num);
        auto finish_s = MPI_Wtime();
        auto parallel_time = finish_p - launch_p;
        auto sequential_time = finish_s - launch_s;
        auto boost = sequential_time / parallel_time;
        printf("\nSequential Time=%f\nParallel Time=%f\nBoost=%f\n", sequential_time, parallel_time, boost);
        for (std::size_t i = 0; i < num; i++)
            ASSERT_NEAR(parallel_sln[i], sequential_sln[i], 0.001);
    }
}*/

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
