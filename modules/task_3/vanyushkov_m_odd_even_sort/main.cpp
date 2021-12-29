// Copyright 2021 Vanyushkov Maxim
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./odd_even_sort.h"

bool checkEQ(int* parallel, int* sequence, int len) {
    for (int i = 0; i < len; i++) {
        // std::cout << parallel[i] << ' ' << sequence[i] << '\n';
        if (parallel[i] != sequence[i]) {
            std::cout << parallel[i] << ' ' << sequence[i] << '\n';
            return false;
        }
    }
    return true;
}

TEST(Odd_Even_Sort, size_100) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int len = 100;
    int* parallel = new int[len];
    int* sequence = new int[len];
    if (procRank == 0) {
        vec_gen(parallel, len);
        copy_vec(sequence, parallel, len);
    }

    odd_even_sort(parallel, len);
    if (procRank == 0) {
        q_sort(sequence, 0, len - 1);
        ASSERT_TRUE(checkEQ(parallel, sequence, len));
        delete[] parallel;
        delete[] sequence;
    }
}

TEST(Odd_Even_Sort, size_5000) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int len = 5000;
    int* parallel = new int[len];
    int* sequence = new int[len];
    if (procRank == 0) {
        vec_gen(parallel, len);
        copy_vec(sequence, parallel, len);
    }

    odd_even_sort(parallel, len);
    if (procRank == 0) {
        q_sort(sequence, 0, len - 1);
        ASSERT_TRUE(checkEQ(parallel, sequence, len));
        delete[] parallel;
        delete[] sequence;
    }
}

TEST(Odd_Even_Sort, size_30001) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int len = 30001;
    int* parallel = new int[len];
    int* sequence = new int[len];
    if (procRank == 0) {
        vec_gen(parallel, len);
        copy_vec(sequence, parallel, len);
    }

    odd_even_sort(parallel, len);
    if (procRank == 0) {
        q_sort(sequence, 0, len - 1);
        ASSERT_TRUE(checkEQ(parallel, sequence, len));
        delete[] parallel;
        delete[] sequence;
    }
}

TEST(Odd_Even_Sort, size_100007) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int len = 100007;
    int* parallel = new int[len];
    int* sequence = new int[len];
    if (procRank == 0) {
        vec_gen(parallel, len);
        copy_vec(sequence, parallel, len);
    }

    odd_even_sort(parallel, len);
    if (procRank == 0) {
        q_sort(sequence, 0, len - 1);
        ASSERT_TRUE(checkEQ(parallel, sequence, len));
        delete[] parallel;
        delete[] sequence;
    }
}

TEST(Odd_Even_Sort, size_2005827) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int len = 2005827;
    int* parallel = new int[len];
    int* sequence = new int[len];
    if (procRank == 0) {
        vec_gen(parallel, len);
        copy_vec(sequence, parallel, len);
    }

    odd_even_sort(parallel, len);
    if (procRank == 0) {
        q_sort(sequence, 0, len - 1);
        ASSERT_TRUE(checkEQ(parallel, sequence, len));
        delete[] parallel;
        delete[] sequence;
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
