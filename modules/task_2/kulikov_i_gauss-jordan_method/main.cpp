// Copyright 2021 Kulikov Ilya
#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_2/kulikov_i_gauss-jordan_method/gauss-jordan_method.h"

int sz = 100;

TEST(MPIGaussJordanMethod, IdentityMatrix) {
    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        double* matrix = new double[sz * (sz + 1)];

        for (int i = 0; i < sz; i ++) {
            matrix[(sz + 1) * i + i] = 1;
            matrix[(sz + 1) * (i + 1) - 1] = i;
        }

        double* result = gauss_jordan_finding(matrix, sz);

        int sum = 0;

        for (int i = 0; i < sz; i++) {
            result[i] -= i;
            sum += result[i];
        }

        EXPECT_EQ(sum, 0);
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
