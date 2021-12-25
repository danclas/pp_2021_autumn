// Copyright 2021 Bakalina Darya
#include <gtest/gtest.h>
#include <vector>
#include "./conjugate_gradient_method.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, scalar_multiply_work_correct) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 3;
    std::vector<double> v1(n);
    std::vector<double> v2(n);
    double correct_res = 46;
    v1[0] = 9;
    v1[1] = 8;
    v1[2] = 7;
    v2[0] = 1;
    v2[1] = 2;
    v2[2] = 3;
    if (ProcRank == 0) {
        double result = scalar_multiply(v1, v2);
        ASSERT_EQ(correct_res, result);
    }
}

TEST(Parallel_Operations_MPI, multiply_mtrx_to_v_work_correct) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 3;
    std::vector<double> v(n);
    std::vector<double> mtrx(n * n);
    std::vector<double> correct_res(n);
    v[0] = 1;
    v[1] = 1;
    v[2] = 1;
    mtrx[0] = 1;
    mtrx[1] = 2;
    mtrx[2] = 3;
    mtrx[3] = 4;
    mtrx[4] = 5;
    mtrx[5] = 6;
    mtrx[6] = 7;
    mtrx[7] = 8;
    mtrx[8] = 9;
    correct_res[0] = 6;
    correct_res[1] = 15;
    correct_res[2] = 24;
    if (ProcRank == 0) {
        std::vector<double> multpl_res = multiply_mtrx_to_v(mtrx, v);
        for (size_t i = 0; i < multpl_res.size(); i++)
            ASSERT_EQ(correct_res[i], multpl_res[i]);
    }
}
TEST(Parallel_Operations_MPI, lin_gradient_method_in_small_matrix) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 2;
    std::vector<double> v(n);
    std::vector<double> mtrx(n * n);
    std::vector<double> correct_res(n);
    v[0] = 3;
    v[1] = 7;
    mtrx[0] = 3;
    mtrx[1] = -1;
    mtrx[2] = -1;
    mtrx[3] = 3;
    correct_res[0] = 2;
    correct_res[1] = 3;
    if (ProcRank == 0) {
        std::vector<double> lin_res = lin_gradient_method(mtrx, v, n);
        for (size_t i = 0; i < lin_res.size(); i++)
            ASSERT_NEAR(correct_res[i], lin_res[i], 0.5);
    }
}

TEST(Parallel_Operations_MPI, parall_gradient_method_in_small_matrix) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 2;
    std::vector<double> v(n);
    std::vector<double> mtrx(n * n);
    std::vector<double> correct_res(n);
    v[0] = 3;
    v[1] = 7;
    mtrx[0] = 3;
    mtrx[1] = -1;
    mtrx[2] = -1;
    mtrx[3] = 3;
    correct_res[0] = 2;
    correct_res[1] = 3;
    std::vector<double> parl_res = parall_gradient_method(mtrx, v, n);
    if (ProcRank == 0) {
        for (size_t i = 0; i < parl_res.size(); i++)
            ASSERT_NEAR(correct_res[i], parl_res[i], 0.5);
    }
}
TEST(Parallel_Operations_MPI, linear_and_parallel_method_are_the_same_in_random_matrix_size_10) {
    int ProcRank;
    int n = 10;
    std::vector<double> v;
    std::vector<double> mtrx;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    v = create_random_vector(n);
    mtrx = create_random_matrix(n);
    std::vector<double> parl_res = parall_gradient_method(mtrx, v, n);
    if (ProcRank == 0) {
        std::vector<double> lin_res = lin_gradient_method(mtrx, v, n);
        for (size_t i = 0; i < lin_res.size(); i++) {
            ASSERT_NEAR(lin_res[i], parl_res[i], 0.5);
        }
    }
}
TEST(Parallel_Operations_MPI, linear_and_parallel_method_are_the_same_in_random_matrix) {
    int ProcRank;
    int n = 15;
    std::vector<double> v;
    std::vector<double> mtrx;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    v = create_random_vector(n);
    mtrx = create_random_matrix(n);
    std::vector<double> parl_res = parall_gradient_method(mtrx, v, n);
    if (ProcRank == 0) {
        std::vector<double> lin_res = lin_gradient_method(mtrx, v, n);
        for (size_t i = 0; i < lin_res.size(); i++) {
            ASSERT_NEAR(lin_res[i], parl_res[i], 0.5);
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
