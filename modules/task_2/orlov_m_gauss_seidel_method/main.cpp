//  Copyright 2021 Maksim Orlov


//  #include <chrono>
#include <gtest/gtest.h>
#include "./gauss_seidel_method.h"
#include <gtest-mpi-listener.hpp>

TEST(MPI, small_matrix) {
    std::vector<std::vector<double>> A = { {5, 1, 3}, {1, 8, 2}, {3, 2, 9} };
    std::vector<double> b = { 9, 11, 14 };
    std::vector<double> res = gaussSeidel(A, b, 0.0001);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_NEAR(res[0], 1, 0.001);
        ASSERT_NEAR(res[1], 1, 0.001);
        ASSERT_NEAR(res[2], 1, 0.001);
    }
}

TEST(MPI, random_matrix_0) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(0);
    std::vector<std::vector<double>> A;
    std::vector<double> b;
    for (int i = 0; i < 1000; i++) {
        A.push_back({});
        double exceptDiagonal = 0;
        for (int j = 0; j < 1000; j++) {
            if (i != j) {
                A[i].push_back(static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 10);
                exceptDiagonal += A[i][j];
            } else {
                A[i].push_back(0);
            }
        }
        A[i][i] = static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 100 + exceptDiagonal;
        b.push_back(static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 10);
    }
    /*
    int t1 = MPI_Wtime();
    */
    std::vector<double> res = gaussSeidel(A, b, 0.00001);
    /*
    int t2 = MPI_Wtime();
    int time_span = t2 - t1;
    */
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    /*
    if (rank == 0)
        std::cout << static_cast<double>(time_span.count()) / 1000 << " seconds";
        */
    if (rank == 0) {
        for (int i = 0; i < 1000; i++) {
            double sum = 0;
            for (int j = 0; j < 1000; j++) {
                sum += A[i][j] * res[j];
            }
            ASSERT_NEAR(sum, b[i], 0.1);
        }
    }
    /*
    if (rank == 0) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        res = sequentialGaussSeidel(A, b, 0.00001);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        std::cout << time_span.count() << " seconds";
    }
    */
}

TEST(MPI, random_matrix_1) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(1);
    std::vector<std::vector<double>> A;
    std::vector<double> b;
    for (int i = 0; i < 1000; i++) {
        A.push_back({});
        double exceptDiagonal = 0;
        for (int j = 0; j < 1000; j++) {
            if (i != j) {
                A[i].push_back(static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 10);
                exceptDiagonal += A[i][j];
            } else {
                A[i].push_back(0);
            }
        }
        A[i][i] = static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 100 + exceptDiagonal;
        b.push_back(static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 10);
    }
    /*
    int t1 = MPI_Wtime();
    */
    std::vector<double> res = gaussSeidel(A, b, 0.00001);
    /*
    int t2 = MPI_Wtime();
    int time_span = t2 - t1;
    */
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    /*
    if (rank == 0)
        std::cout << static_cast<double>(time_span.count()) / 1000 << " seconds";
        */
    if (rank == 0) {
        for (int i = 0; i < 1000; i++) {
            double sum = 0;
            for (int j = 0; j < 1000; j++) {
                sum += A[i][j] * res[j];
            }
            ASSERT_NEAR(sum, b[i], 0.1);
        }
    }
    /*
    if (rank == 0) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        res = sequentialGaussSeidel(A, b, 0.00001);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        std::cout << time_span.count() << " seconds";
    }
    */
}

TEST(MPI, random_matrix_2) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(2);
    std::vector<std::vector<double>> A;
    std::vector<double> b;
    for (int i = 0; i < 1000; i++) {
        A.push_back({});
        double exceptDiagonal = 0;
        for (int j = 0; j < 1000; j++) {
            if (i != j) {
                A[i].push_back(static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 10);
                exceptDiagonal += A[i][j];
            } else {
                A[i].push_back(0);
            }
        }
        A[i][i] = static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 100 + exceptDiagonal;
        b.push_back(static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 10);
    }
    /*
    int t1 = MPI_Wtime();
    */
    std::vector<double> res = gaussSeidel(A, b, 0.00001);
    /*
    int t2 = MPI_Wtime();
    int time_span = t2 - t1;
    */
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    /*
    if (rank == 0)
        std::cout << static_cast<double>(time_span.count()) / 1000 << " seconds";
        */
    if (rank == 0) {
        for (int i = 0; i < 1000; i++) {
            double sum = 0;
            for (int j = 0; j < 1000; j++) {
                sum += A[i][j] * res[j];
            }
            ASSERT_NEAR(sum, b[i], 0.1);
        }
    }
    /*
    if (rank == 0) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        res = sequentialGaussSeidel(A, b, 0.00001);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        std::cout << time_span.count() << " seconds";
    }
    */
}

TEST(MPI, random_matrix_3) {
    std::random_device dev;
    std::mt19937 rng(dev());
    rng.seed(3);
    std::vector<std::vector<double>> A;
    std::vector<double> b;
    for (int i = 0; i < 1000; i++) {
        A.push_back({});
        double exceptDiagonal = 0;
        for (int j = 0; j < 1000; j++) {
            if (i != j) {
                A[i].push_back(static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 10);
                exceptDiagonal += A[i][j];
            } else {
                A[i].push_back(0);
            }
        }
        A[i][i] = static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 100 + exceptDiagonal;
        b.push_back(static_cast<double>(randNumber(&dev, &rng)) / RAND_MAX * 10);
    }
    /*
    int t1 = MPI_Wtime();
    */
    std::vector<double> res = gaussSeidel(A, b, 0.00001);
    /*
    int t2 = MPI_Wtime();
    int time_span = t2 - t1;
    */
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    /*
    if (rank == 0)
        std::cout << static_cast<double>(time_span.count()) / 1000 << " seconds";
        */
    if (rank == 0) {
        for (int i = 0; i < 1000; i++) {
            double sum = 0;
            for (int j = 0; j < 1000; j++) {
                sum += A[i][j] * res[j];
            }
            ASSERT_NEAR(sum, b[i], 0.1);
        }
    }
    /*
    if (rank == 0) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        res = sequentialGaussSeidel(A, b, 0.00001);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        std::cout << time_span.count() << " seconds";
    }
    */
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
