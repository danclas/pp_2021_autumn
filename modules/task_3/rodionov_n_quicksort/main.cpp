// Copyright 2021 TexHik620953
#include <gtest/gtest.h>
#include <time.h>
#include <cstring>
#include <iostream>
#include "./quicksort.h"
#include <gtest-mpi-listener.hpp>
int length = 5000;

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);  // check for MPI_SUCCESS?

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}



TEST(QuickSort, TimeComparsion) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* arr1 = nullptr;
    int* arr2 = nullptr;
    double mpi_start;
    double mpi_end;

    clock_t seq_start;
    clock_t seq_end;
    if (rank == 0) {
        arr1 = new int[length];
        arr2 = new int[length];
        RandomArray(arr1, length);
        memcpy(arr2, arr1, sizeof(int) * length);
        mpi_start = MPI_Wtime();
    }

    QuickSortMpi(arr1, length);

    if (rank == 0) {
        mpi_end = MPI_Wtime();
        double mpi_elapsed_seconds = (mpi_end - mpi_start);
        std::cout << "Mpi time elapsed: " << mpi_elapsed_seconds << " ms\n";
        seq_start = clock();
        quickSort(arr2, length);
        seq_end = clock();
        double seq_elapsed_seconds =
            static_cast<double>(seq_end - seq_start) / CLOCKS_PER_SEC;
        std::cout << "Sequential time elapsed: "
            << seq_elapsed_seconds << " ms\n";
        delete[] arr1;
        delete[] arr2;
    }
}

TEST(QuickSort, SequentialSortCheck) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int* arr = new int[length];
        RandomArray(arr, length);
        quickSort(arr, length);
        EXPECT_TRUE(CheckOrdering(arr, length));
        delete[] arr;
    }
}

TEST(QuickSort, CheckOrderingPositiveTest) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int* arr = new int[length];
        for (int i = 0; i < length; i++) {
            arr[i] = i;
        }
        EXPECT_TRUE(CheckOrdering(arr, length));
        delete[] arr;
    }
}

TEST(QuickSort, CheckOrderingNegativeTest) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int* arr = new int[length];
        for (int i = 0; i < length; i++) {
            arr[i] = i;
        }
        arr[length - 1] = 0;
        arr[length - 2] = 1;
        EXPECT_FALSE(CheckOrdering(arr, length));
        delete[] arr;
    }
}



TEST(QuickSort, SortsEqualsTest) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* arr = nullptr;
    int* arr2 = nullptr;
    if (rank == 0) {
        arr = new int[length];
        arr2 = new int[length];
        RandomArray(arr, length);
        memcpy(arr2, arr, sizeof(int) * length);
    }

    QuickSortMpi(arr, length);

    if (rank == 0) {
        quickSort(arr2, length);
        for (int i = 0; i < length; i++) {
            EXPECT_EQ(arr[i], arr2[i]);
        }
        delete[] arr;
        delete[] arr2;
    }
}


TEST(QuickSort, MpiSort) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* arr = nullptr;
    if (rank == 0) {
        arr = new int[length];
        RandomArray(arr, length);
    }

    QuickSortMpi(arr, length);

    if (rank == 0) {
        EXPECT_TRUE(CheckOrdering(arr, length));
        delete[] arr;
    }
}
