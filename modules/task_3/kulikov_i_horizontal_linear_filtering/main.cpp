// Copyright 2021 Kulikov Ilya
#include <mpi.h>
#include <gtest/gtest.h>
#include <random>
#include <iostream>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_3/kulikov_i_horizontal_linear_filtering/linear_filtering_gauss_kernel.h"


TEST(MPIHorisontalLinearFilterGauss, Sigma1) {
    const int sz = 100;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* matrix = nullptr;
    double* img = nullptr;
    double* out_img = nullptr;

    if (rank == 0) {
        matrix = get_matrix(1.0);

        img = get_random_img(sz);

        EXPECT_EQ(0, 0);
    } else {
        matrix = new double[9];
    }

    out_img = horizontal_linear_filter_gauss(img, matrix, sz);

    delete[] img;
    delete[] out_img;
}

TEST(MPIHorisontalLinearFilterGauss, Sigma0_5) {
    const int sz = 100;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* matrix = nullptr;
    double* img = nullptr;
    double* out_img = nullptr;

    if (rank == 0) {
        matrix = get_matrix(0.5);

        img = get_random_img(sz);

        EXPECT_EQ(0, 0);
    } else {
        matrix = new double[9];
    }

    out_img = horizontal_linear_filter_gauss(img, matrix, sz);

    delete[] img;
    delete[] out_img;
}

TEST(MPIHorisontalLinearFilterGauss, Sigma2) {
    const int sz = 100;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* matrix = nullptr;
    double* img = nullptr;
    double* out_img = nullptr;

    if (rank == 0) {
        matrix = get_matrix(2.0);

        img = get_random_img(sz);

        EXPECT_EQ(0, 0);
    } else {
        matrix = new double[9];
    }

    out_img = horizontal_linear_filter_gauss(img, matrix, sz);

    delete[] img;
    delete[] out_img;
}

TEST(MPIHorisontalLinearFilterGauss, Img250) {
    const int sz = 250;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* matrix = nullptr;
    double* img = nullptr;
    double* out_img = nullptr;

    if (rank == 0) {
        matrix = get_matrix(1.0);

        img = get_random_img(sz);

        EXPECT_EQ(0, 0);
    } else {
        matrix = new double[9];
    }

    out_img = horizontal_linear_filter_gauss(img, matrix, sz);

    delete[] img;
    delete[] out_img;
}

TEST(MPIHorisontalLinearFilterGauss, Img250Sigma2) {
    const int sz = 250;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* matrix = nullptr;
    double* img = nullptr;
    double* out_img = nullptr;

    if (rank == 0) {
        matrix = get_matrix(2.0);

        img = get_random_img(sz);

        EXPECT_EQ(0, 0);
    } else {
        matrix = new double[9];
    }

    out_img = horizontal_linear_filter_gauss(img, matrix, sz);

    delete[] img;
    delete[] out_img;
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
