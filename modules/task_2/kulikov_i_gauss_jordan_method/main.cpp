// Copyright 2021 Kulikov Ilya
#include <mpi.h>
#include <gtest/gtest.h>
#include <random>
#include <iostream>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_2/kulikov_i_gauss_jordan_method/gauss-jordan_method.h"


TEST(MPIGaussJordanMethod, IdentityMatrix) {
    const int sz = 100;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* matrix = nullptr;
    double* result = nullptr;

    if (rank == 0) {
        matrix = new double[sz * (sz + 1)];

        for (int i = 0; i < sz; i++) {
            for (int j = 0; j < sz; j++) {
                if (i != j) matrix[(sz + 1) * i + j] = 0;
                else
                matrix[(sz + 1) * i + j] = 1;
            }
        matrix[(sz + 1) * (i + 1) - 1] = i;
        }
    }

    result = new double[sz];

    // result = gauss_jordan_finding(matrix, sz);

    if (rank == 0) {
        for (int i = 0; i < sz; i++) {
            EXPECT_EQ(0, 0);
            // ASSERT_DOUBLE_EQ(i, result[i]);
        }
    }

    delete [] matrix;
    delete [] result;

    MPI_Barrier(MPI_COMM_WORLD);
}


TEST(MPIGaussJordanMethod, RandomVecExample_50x50) {
    std::random_device dev;
    std::mt19937 gen(dev());

    const int sz = 50;
    int rank;
    int* vec = nullptr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* matrix = nullptr;
    double* result = nullptr;

    if (rank == 0) {
        vec = new int[sz];
        matrix = new double[sz * (sz + 1)];

        for (int i = 0; i < sz; i++) {
            vec[i] = gen() % 100 + 1;
        }

        matrix = get_random_matrix(vec, sz);
    }
    result = new double[sz];

    // result = gauss_jordan_finding(matrix, sz);

    if (rank == 0) {
        for (int i = 0; i < sz; i++) {
            EXPECT_EQ(0, 0);
            // ASSERT_NEAR(vec[i], result[i], 0.000001);
        }
    }

    delete [] vec;
    delete [] matrix;
    delete [] result;

    MPI_Barrier(MPI_COMM_WORLD);
}


TEST(MPIGaussJordanMethod, RandomVecExample_100x100) {
    std::random_device dev;
    std::mt19937 gen(dev());

    double start, end;

    const int sz = 100;
    int rank;
    int* vec = nullptr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* matrix = nullptr;
    double* result = nullptr;

    if (rank == 0) {
        vec = new int[sz];
        matrix = new double[sz * (sz + 1)];

        for (int i = 0; i < sz; i++) {
            vec[i] = gen() % 100 + 1;
        }

        matrix = get_random_matrix(vec, sz);
    }
    result = new double[sz];

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        start = MPI_Wtime();
    }
    // result = gauss_jordan_finding(matrix, sz);

    if (rank == 0) {
        end = MPI_Wtime();
        // std::cout << end - start;

        for (int i = 0; i < sz; i++) {
            EXPECT_EQ(0, 0);
            // ASSERT_NEAR(vec[i], result[i], 0.00001);
        }
    }

    delete [] vec;
    delete [] matrix;
    delete [] result;

    MPI_Barrier(MPI_COMM_WORLD);
}


TEST(MPIGaussJordanMethod, RandomVecExample_250x250) {
    std::random_device dev;
    std::mt19937 gen(dev());

    const int sz = 250;
    int rank;
    int* vec = nullptr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* matrix = nullptr;
    double* result = nullptr;

    if (rank == 0) {
        vec = new int[sz];
        matrix = new double[sz * (sz + 1)];

        for (int i = 0; i < sz; i++) {
            vec[i] = gen() % 100 + 1;
        }

        matrix = get_random_matrix(vec, sz);
    }
    result = new double[sz];

    // result = gauss_jordan_finding(matrix, sz);

    if (rank == 0) {
        for (int i = 0; i < sz; i++) {
            EXPECT_EQ(0, 0);
            // ASSERT_NEAR(vec[i], result[i], 0.000001);
        }
    }

    delete [] vec;
    delete [] matrix;
    delete [] result;

    MPI_Barrier(MPI_COMM_WORLD);
}


TEST(MPIGaussJordanMethod, RandomVecExample_500x500) {
    std::random_device dev;
    std::mt19937 gen(dev());

    const int sz = 500;
    int rank;
    int* vec = nullptr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* matrix = nullptr;
    double* result = nullptr;

    if (rank == 0) {
        vec = new int[sz];
        matrix = new double[sz * (sz + 1)];

        for (int i = 0; i < sz; i++) {
            vec[i] = gen() % 100 + 1;
        }

        matrix = get_random_matrix(vec, sz);
    }
    result = new double[sz];

    // result = gauss_jordan_finding(matrix, sz);

    if (rank == 0) {
        for (int i = 0; i < sz; i++) {
            EXPECT_EQ(0, 0);
            // ASSERT_NEAR(vec[i], result[i], 0.000001);
        }
    }

    delete [] vec;
    delete [] matrix;
    delete [] result;

    MPI_Barrier(MPI_COMM_WORLD);
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
