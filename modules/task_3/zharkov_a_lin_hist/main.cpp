// Copyright 2021 Zharkov Andrey
#include <gtest/gtest.h>
#include <vector>
#include "./lin_hist.h"
#include <gtest-mpi-listener.hpp>


TEST(Parallel_Operations_MPI, 3_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int width = 5;
    int height = 3;
    int* img = nullptr;
    if (rank == 0) {
        img = getRandomArr(width, height);
    }
    int *new_img_p = get_Parallel_lin_hist(img, width, height);
    if (rank == 0) {
        int min = get_local_min(img, width, height);
        int max = get_local_max(img, width, height);
        int* new_img_s = get_Sequential_lin_hist(img, width, height, min, max);
        for (int i = 0; i < width * height; i++) {
            ASSERT_EQ(new_img_p[i], new_img_s[i]);
        }
    }
}
TEST(Parallel_Operations_MPI, 7_9) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int width = 9;
    int height = 7;
    int* img = nullptr;
    if (rank == 0) {
        img = getRandomArr(width, height);
    }
    int* new_img_p = get_Parallel_lin_hist(img, width, height);
    if (rank == 0) {
        int min = get_local_min(img, width, height);
        int max = get_local_max(img, width, height);

        int* new_img_s = get_Sequential_lin_hist(img, width, height, min, max);
        for (int i = 0; i < width * height; i++) {
            ASSERT_EQ(new_img_p[i], new_img_s[i]);
        }
    }
}
TEST(Parallel_Operations_MPI, 20_30) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int width = 12;
    int height = 11;
    int* img = nullptr;
    if (rank == 0) {
        img = getRandomArr(width, height);
    }
    int* new_img_p = get_Parallel_lin_hist(img, width, height);
    if (rank == 0) {
        int min = get_local_min(img, width, height);
        int max = get_local_max(img, width, height);

        int* new_img_s = get_Sequential_lin_hist(img, width, height, min, max);
        for (int i = 0; i < width * height; i++) {
            ASSERT_EQ(new_img_p[i], new_img_s[i]);
        }
    }
}
TEST(Parallel_Operations_MPI, 45_110) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int width = 12;
    int height = 11;
    int* img = nullptr;
    if (rank == 0) {
        img = getRandomArr(width, height);
    }
    int* new_img_p = get_Parallel_lin_hist(img, width, height);
    if (rank == 0) {
        int min = get_local_min(img, width, height);
        int max = get_local_max(img, width, height);

        int* new_img_s = get_Sequential_lin_hist(img, width, height, min, max);
        for (int i = 0; i < width * height; i++) {
            ASSERT_EQ(new_img_p[i], new_img_s[i]);
        }
    }
}
TEST(Parallel_Operations_MPI, 1_40) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int width = 12;
    int height = 11;
    int* img = nullptr;
    if (rank == 0) {
        img = getRandomArr(width, height);
    }
    int* new_img_p = get_Parallel_lin_hist(img, width, height);
    if (rank == 0) {
        int min = get_local_min(img, width, height);
        int max = get_local_max(img, width, height);

        int* new_img_s = get_Sequential_lin_hist(img, width, height, min, max);
        for (int i = 0; i < width * height; i++) {
            ASSERT_EQ(new_img_p[i], new_img_s[i]);
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
