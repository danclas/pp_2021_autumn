// Copyright 2021 Khlyustov Ilya
#include <gtest/gtest.h>
#include "./line_vertical_image_filter.h"
#include <gtest-mpi-listener.hpp>

TEST(Line_vertical_image_filter, Rows_5_Columns_5) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int h = 5;
    int w = 5;
    vectorF kernel = getKernel();
    vectorF img;

    if (proc_rank == 0) {
        img = getRandomImg(w, h);
    } else {
        img.resize(w * h);
    }
    vectorF par = getParallel(kernel, img, w, h);
    if (proc_rank == 0) {
        vectorF seq = getSequential(kernel, img, w, h);
        ASSERT_EQ(seq, par);
    }
}

TEST(Line_vertical_image_filter, Rows_51_Columns_51) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int h = 51;
    int w = 51;
    vectorF kernel = getKernel();
    vectorF img;

    if (proc_rank == 0) {
        img = getRandomImg(w, h);
    } else {
        img.resize(w * h);
    }
    vectorF par = getParallel(kernel, img, w, h);
    if (proc_rank == 0) {
        vectorF seq = getSequential(kernel, img, w, h);
        ASSERT_EQ(seq, par);
    }
}

TEST(Line_vertical_image_filter, Rows_105_Columns_105) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int h = 105;
    int w = 105;
    vectorF kernel = getKernel();
    vectorF img;

    if (proc_rank == 0) {
        img = getRandomImg(w, h);
    } else {
        img.resize(w * h);
    }
    vectorF par = getParallel(kernel, img, w, h);
    if (proc_rank == 0) {
        vectorF seq = getSequential(kernel, img, w, h);
        ASSERT_EQ(seq, par);
    }
}

TEST(Line_vertical_image_filter, Rows_51_Columns_105) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int h = 51;
    int w = 105;
    vectorF kernel = getKernel();
    vectorF img;

    if (proc_rank == 0) {
        img = getRandomImg(w, h);
    } else {
        img.resize(w * h);
    }
    vectorF par = getParallel(kernel, img, w, h);
    if (proc_rank == 0) {
        vectorF seq = getSequential(kernel, img, w, h);
        ASSERT_EQ(seq, par);
    }
}

TEST(Line_vertical_image_filter, Rows_105_Columns_51) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int h = 105;
    int w = 51;
    vectorF kernel = getKernel();
    vectorF img;

    if (proc_rank == 0) {
        img = getRandomImg(w, h);
    } else {
        img.resize(w * h);
    }
    vectorF par = getParallel(kernel, img, w, h);
    if (proc_rank == 0) {
        vectorF seq = getSequential(kernel, img, w, h);
        ASSERT_EQ(seq, par);
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
