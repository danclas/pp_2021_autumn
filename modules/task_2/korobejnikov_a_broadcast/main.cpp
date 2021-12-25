// Copyright 2021 Korobejnikov Alexander

#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "./broadcast.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_My_Bcast_MPI_FLOAT) {
    std::vector<float> vec = {1.1234, 4.5234, 6.6789, 5.3334, 6.1233};
    int n = vec.size();
    int rank, size, root;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % size;
    }

    My_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<float> local_vec;
    if (rank == root) {
        local_vec = std::vector<float>(vec.begin(), vec.end());
    } else {
        local_vec.resize(n);
    }
    MPI_Bcast(local_vec.data(), n, MPI_FLOAT, root, MPI_COMM_WORLD);
    float eps = 1e-4;
    for (int i = 0; i < n; ++i) {
        if (!(local_vec[i] < vec[i] + eps && local_vec[i] > vec[i] - eps)) {
            FAIL();
        }
    }
}

TEST(Parallel_Operations_MPI, Test_My_Bcast_MPI_INT_1) {
    std::vector<int> vec = {3, 33, 5, 63, 87};
    int n = vec.size();
    int rank, size, root;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % size;
    }

    My_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> local_vec;
    if (rank == root) {
        local_vec = std::vector<int>(vec.begin(), vec.end());
    } else {
        local_vec.resize(n);
    }
    My_Bcast(local_vec.data(), n, MPI_INT, root, MPI_COMM_WORLD);
    for (int i = 0; i < n; ++i) {
        if (local_vec[i] != vec[i]) {
            FAIL();
        }
    }
}

TEST(Parallel_Operations_MPI, Test_My_Bcast_MPI_INT_2) {
    int n = 33;
    int rank, size, root;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % size;
    }

    My_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_n;
    if (rank == root) {
        local_n = n;
    }
    My_Bcast(&local_n, 1, MPI_INT, root, MPI_COMM_WORLD);
    if (local_n != n) {
        FAIL();
    }
}

TEST(Parallel_Operations_MPI, Test_My_Bcast_MPI_INT_3) {
    std::vector<int> vec = { 3 };
    int n = vec.size();
    int rank, size, root;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % size;
    }

    My_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> local_vec;
    if (rank == root) {
        local_vec = std::vector<int>(vec.begin(), vec.end());
    } else {
        local_vec.resize(n);
    }
    My_Bcast(local_vec.data(), n, MPI_INT, root, MPI_COMM_WORLD);
    for (int i = 0; i < n; ++i) {
        if (local_vec[i] != vec[i]) {
            FAIL();
        }
    }
}

TEST(Parallel_Operations_MPI, Test_My_Bcast_MPI_DOUBLE) {
    std::vector<double> vec =
        {1.1234567898, 3.9087654125, 6.456732430, 2.213478613, 2.1283712983};
    int n = vec.size();
    int rank, size, root;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::random_device dev;
        std::mt19937 gen(dev());
        root = gen() % size;
    }

    My_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<double> local_vec;
    if (rank == root) {
        local_vec = std::vector<double>(vec.begin(), vec.end());
    } else {
        local_vec.resize(n);
    }
    My_Bcast(local_vec.data(), n, MPI_DOUBLE, root, MPI_COMM_WORLD);
    double eps = 1e-10;
    for (int i = 0; i < n; ++i) {
        if (!(local_vec[i] < vec[i] + eps && local_vec[i] > vec[i] - eps)) {
            FAIL();
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing
        ::UnitTest::GetInstance()->listeners();
    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());
    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
