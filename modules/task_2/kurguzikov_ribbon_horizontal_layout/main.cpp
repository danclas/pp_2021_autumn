  // Copyright 2021 Kurguzikov Kirill
#include <gtest/gtest.h>
#include "./kurguzikov_ribbon_horizontal_layout.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Matrix_Multiplication_MPI, A_2x2_B_2x2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* A = nullptr, * B = nullptr, * C = nullptr, * Cs = nullptr;
    int Astring = 2, Acolumn = 2, Bstring = 2, Bcolumn = 2;
    if (rank == 0) {
        A = getRandomMatrix(Astring, Acolumn);
        B = getRandomMatrix(Bstring, Bcolumn);
        C = initEmptyMatrix(Astring, Bcolumn);
        Cs = initEmptyMatrix(Astring, Bcolumn);
    }

    ParallelMatrixMultiplication(A, Astring, Acolumn, B, Bstring, Bcolumn, C);

    if (rank == 0) {
        SequentialMatrixMultiplication(A, Astring, Acolumn, B, Bstring, Bcolumn, Cs);
        bool flag = true;

        for (int i = 0; i < Astring * Bcolumn; i++) {
            if (C[i] != Cs[i]) {
                flag = false;
            }
        }
        ASSERT_EQ(flag, true);
    }
}

TEST(Parallel_Matrix_Multiplication_MPI, A_4x3_B_3x2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* A = nullptr, * B = nullptr, * C = nullptr, * Cs = nullptr;
    int Astring = 4, Acolumn = 3, Bstring = 3, Bcolumn = 2;
    if (rank == 0) {
        A = getRandomMatrix(Astring, Acolumn);
        B = getRandomMatrix(Bstring, Bcolumn);
        C = initEmptyMatrix(Astring, Bcolumn);
        Cs = initEmptyMatrix(Astring, Bcolumn);
    }

    ParallelMatrixMultiplication(A, Astring, Acolumn, B, Bstring, Bcolumn, C);

    if (rank == 0) {
        SequentialMatrixMultiplication(A, Astring, Acolumn, B, Bstring, Bcolumn, Cs);
        bool flag = true;
        for (int i = 0; i < Astring; i++) {
            for (int j = 0; j < Bcolumn; j++) {
                if (C[Bcolumn * i + j] != Cs[Bcolumn * i + j]) {
                    flag = false;
                }
            }
        }
        ASSERT_EQ(flag, true);
    }
}

TEST(Parallel_Matrix_Multiplication_MPI, A_5x10_B_10x2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* A = nullptr, * B = nullptr, * C = nullptr, * Cs = nullptr;
    int Astring = 5, Acolumn = 10, Bstring = 10, Bcolumn = 2;
    if (rank == 0) {
        A = getRandomMatrix(Astring, Acolumn);
        B = getRandomMatrix(Bstring, Bcolumn);
        C = initEmptyMatrix(Astring, Bcolumn);
        Cs = initEmptyMatrix(Astring, Bcolumn);
    }

    ParallelMatrixMultiplication(A, Astring, Acolumn, B, Bstring, Bcolumn, C);

    if (rank == 0) {
        SequentialMatrixMultiplication(A, Astring, Acolumn, B, Bstring, Bcolumn, Cs);

        bool flag = true;

        for (int i = 0; i < Astring * Bcolumn; i++) {
            if (C[i] != Cs[i]) {
                flag = false;
            }
        }
        ASSERT_EQ(flag, true);
    }
}

TEST(Parallel_Matrix_Multiplication_MPI, A_15x3_B_3x20) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* A = nullptr, * B = nullptr, * C = nullptr, * Cs = nullptr;
    int Astring = 15, Acolumn = 3, Bstring = 3, Bcolumn = 20;
    if (rank == 0) {
        A = getRandomMatrix(Astring, Acolumn);
        B = getRandomMatrix(Bstring, Bcolumn);
        C = initEmptyMatrix(Astring, Bcolumn);
        Cs = initEmptyMatrix(Astring, Bcolumn);
    }

    ParallelMatrixMultiplication(A, Astring, Acolumn, B, Bstring, Bcolumn, C);

    if (rank == 0) {
        SequentialMatrixMultiplication(A, Astring, Acolumn, B, Bstring, Bcolumn, Cs);
        bool flag = true;

        for (int i = 0; i < Astring * Bcolumn; i++) {
            if (C[i] != Cs[i]) {
                flag = false;
            }
        }
        ASSERT_EQ(flag, true);
    }
}

TEST(Parallel_Matrix_Multiplication_MPI, A_500x100_B_100x300) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* A = nullptr, * B = nullptr, * C = nullptr, * Cs = nullptr;
    int Astring = 500, Acolumn = 100, Bstring = 100, Bcolumn = 300;
    if (rank == 0) {
        A = getRandomMatrix(Astring, Acolumn);
        B = getRandomMatrix(Bstring, Bcolumn);
        C = initEmptyMatrix(Astring, Bcolumn);
        Cs = initEmptyMatrix(Astring, Bcolumn);
    }
    // double start_p = MPI_Wtime();
    ParallelMatrixMultiplication(A, Astring, Acolumn, B, Bstring, Bcolumn, C);
    // double end_p = MPI_Wtime();
    if (rank == 0) {
        // double start_s = MPI_Wtime();
        SequentialMatrixMultiplication(A, Astring, Acolumn, B, Bstring, Bcolumn, Cs);
        // double end_s = MPI_Wtime();
        // std::cout << "Parallel time: " << end_p-start_p << std::endl;
        // std::cout << "Serial time: " << end_s - start_s << std::endl;
        bool flag = true;

        for (int i = 0; i < Astring * Bcolumn; i++) {
            if (C[i] != Cs[i]) {
                flag = false;
            }
        }
        ASSERT_EQ(flag, true);
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
