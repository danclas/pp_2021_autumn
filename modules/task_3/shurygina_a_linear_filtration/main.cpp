// Copyright 2021 Shurygina A
#include <mpi.h>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <gtest-mpi-listener.hpp>
#include "./linefilter_block.h"


TEST(Parallel_Operation_MPI, Test_Image_50x50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int height = 50;
    int width = 50;
    int pixes = height * width;
    int radius = 1;
    double sigma = 5.0;
    std::vector<int> img(pixes);
    std::vector<int> simpleGauss(pixes);
    std::vector<int> parallelGauss(pixes);
    std::vector<double> kernel = createGaussianKernel(radius, sigma);
    img = createRandomImg(height, width);
    parallelGauss =
        parallelGaussianFilter(img, height, width, radius,
            sigma, kernel);
    if (rank == 0) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                simpleGauss[i * width + j] =
                    simpleGaussianFilter(img, height, width, radius,
                        sigma, kernel, i, j);
            }
        }
        ASSERT_EQ(parallelGauss, simpleGauss);
    }
}

TEST(Parallel_Operation_MPI, Test_Image_100x100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int height = 100;
    int width = 100;
    int pixes = height * width;
    int radius = 1;
    double sigma = 5.0;
    std::vector<int> img(pixes);
    std::vector<int> simpleGauss(pixes);
    std::vector<int> parallelGauss(pixes);
    std::vector<double> kernel = createGaussianKernel(radius, sigma);
    img = createRandomImg(height, width);
    parallelGauss =
        parallelGaussianFilter(img, height, width, radius,
            sigma, kernel);
    if (rank == 0) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                simpleGauss[i * width + j] =
                    simpleGaussianFilter(img, height, width, radius,
                        sigma, kernel, i, j);
            }
        }
        ASSERT_EQ(parallelGauss, simpleGauss);
    }
}
TEST(Parallel_Operation_MPI, Test_Image_500x500) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int height = 500;
    int width = 500;
    int pixes = height * width;
    int radius = 1;
    double sigma = 5.0;
    std::vector<int> img(pixes);
    std::vector<int> simpleGauss(pixes);
    std::vector<int> parallelGauss(pixes);
    std::vector<double> kernel =
        createGaussianKernel(radius, sigma);
    img = createRandomImg(height, width);
    parallelGauss =
        parallelGaussianFilter(img, height, width, radius,
            sigma, kernel);
    if (rank == 0) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                simpleGauss[i * width + j] =
                    simpleGaussianFilter(img, height, width, radius,
                        sigma, kernel, i, j);
            }
        }
        ASSERT_EQ(parallelGauss, simpleGauss);
    }
}
TEST(Parallel_Operation_MPI, Test_Image_255x255) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int height = 255;
    int width = 255;
    int pixes = height * width;
    int radius = 1;
    double sigma = 5.0;
    std::vector<int> img(pixes);
    std::vector<int> simpleGauss(pixes);
    std::vector<int> parallelGauss(pixes);
    std::vector<double> kernel =
        createGaussianKernel(radius, sigma);
    img = createRandomImg(height, width);
    parallelGauss =
        parallelGaussianFilter(img, height, width, radius, sigma, kernel);
    if (rank == 0) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                simpleGauss[i * width + j] =
                    simpleGaussianFilter(img, height, width, radius,
                        sigma, kernel, i, j);
            }
        }
        ASSERT_EQ(parallelGauss, simpleGauss);
    }
}
TEST(Parallel_Operation_MPI, Test_Image_500x1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int height = 50;
    int width = 1000;
    int pixes = height * width;
    int radius = 1;
    double sigma = 5.0;
    std::vector<int> img(pixes);
    std::vector<int> simpleGauss(pixes);
    std::vector<int> parallelGauss(pixes);
    std::vector<double> kernel =
        createGaussianKernel(radius, sigma);
    img = createRandomImg(height, width);
    parallelGauss =
        parallelGaussianFilter(img, height, width, radius,
            sigma, kernel);
    if (rank == 0) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                simpleGauss[i * width + j] =
                    simpleGaussianFilter(img, height, width, radius,
                        sigma, kernel, i, j);
            }
        }
        ASSERT_EQ(parallelGauss, simpleGauss);
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
