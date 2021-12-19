//  Copyright 2021 Maksim Orlov


//  #include <chrono>
#include <gtest/gtest.h>
#include "./horizontal_gaussian_filter.h"
#include <gtest-mpi-listener.hpp>
#define MATRIX_SIZE 128


TEST(MPI, small_random_image) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(0);
    std::vector<std::vector<char>> image;
    for (int i = 0; i < 16; i++) {
        image.push_back({});
        for (int j = 0; j < 16; j++) {
            if (i != j) {
                image[i].push_back(static_cast<char>(randNumber(&dev, &rng) % 256));
            }
        }
    }
    //  std::cout << "1" << std::endl;
    std::vector<std::vector<char>> res1 = gaussianFilter(image, 0.5);
    //  std::cout << "2" << std::endl;
    std::vector<std::vector<char>> res2 = sequentialGaussianFilter(image, 0.5);
    //  std::cout << "3" << std::endl;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

TEST(MPI, random_image_1) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(1);
    std::vector<std::vector<char>> image;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        image.push_back({});
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (i != j) {
                image[i].push_back(static_cast<char>(randNumber(&dev, &rng) % 256));
            }
        }
    }
    std::vector<std::vector<char>> res1 = gaussianFilter(image, 0.5);
    std::vector<std::vector<char>> res2 = sequentialGaussianFilter(image, 0.5);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

TEST(MPI, random_image_2) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(2);
    std::vector<std::vector<char>> image;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        image.push_back({});
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (i != j) {
                image[i].push_back(static_cast<char>(randNumber(&dev, &rng) % 256));
            }
        }
    }
    std::vector<std::vector<char>> res1 = gaussianFilter(image, 0.5);
    std::vector<std::vector<char>> res2 = sequentialGaussianFilter(image, 0.5);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

TEST(MPI, sigma_0) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(3);
    std::vector<std::vector<char>> image;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        image.push_back({});
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (i != j) {
                image[i].push_back(static_cast<char>(randNumber(&dev, &rng) % 256));
            }
        }
    }
    std::vector<std::vector<char>> res1 = gaussianFilter(image, 0);
    std::vector<std::vector<char>> res2 = sequentialGaussianFilter(image, 0);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

TEST(MPI, sigma_large) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(4);
    std::vector<std::vector<char>> image;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        image.push_back({});
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (i != j) {
                image[i].push_back(static_cast<char>(randNumber(&dev, &rng) % 256));
            }
        }
    }
    std::vector<std::vector<char>> res1 = gaussianFilter(image, 10);
    std::vector<std::vector<char>> res2 = sequentialGaussianFilter(image, 10);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

/*
TEST(MPI, random_matrix_3) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(3);
    std::vector<std::vector<double>> A;
    std::vector<double> b;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        A.push_back({});
        double exceptDiagonal = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (i != j) {
                A[i].push_back(static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 10);
                exceptDiagonal += A[i][j];
            }
            else {
                A[i].push_back(0);
            }
        }
        A[i][i] = static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 100 + exceptDiagonal;
        b.push_back(static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 10);
    }

    double t1 = MPI_Wtime();

    std::vector<double> res = gaussSeidel(A, b, 0.00001);
    double t2 = MPI_Wtime();
    double time_span1 = t2 - t1;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        std::cout << "Parallel: " << time_span1 << " seconds\n";

    if (rank == 0) {
        for (int i = 0; i < MATRIX_SIZE; i++) {
            double sum = 0;
            for (int j = 0; j < MATRIX_SIZE; j++) {
                sum += A[i][j] * res[j];
            }
            ASSERT_NEAR(sum, b[i], 0.1);
        }
    }


    t1 = MPI_Wtime();
    if (rank == 0)
        res = sequentialGaussSeidel(A, b, 0.00001);
    t2 = MPI_Wtime();
    double time_span2 = t2 - t1;
    if (rank == 0) {
        std::cout << "Sequential: " << time_span2 << " seconds\n";
        std::cout << "Ratio: " << time_span2 / time_span1;
    }


}
*/

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
