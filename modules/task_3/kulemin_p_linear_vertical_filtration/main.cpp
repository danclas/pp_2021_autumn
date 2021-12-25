// Copyright 2021 Zaytsev Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./linear_vectrical_filtration.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Matrix_Multiplacition, mRows_Eq_mColumns_50) {
    int currentProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

    std::vector<float> matrix, img;
    int height = 50;
    int weight = 50;
    getKernell(&matrix);

    if (currentProcess == 0) {
        getRandomImg(&img, weight, height);
    }
    std::vector<float> globalMatrix = getParallelOperations(matrix, img,
   weight, height);
    if (currentProcess == 0) {
        std::vector<float> referenceMatrix = getSequentialOperations(matrix,
        img, weight, height);
        ASSERT_EQ(globalMatrix, referenceMatrix);
    }
}

TEST(Parallel_Matrix_Multiplacition, mRows_Eq_mColumns_5) {
    int currentProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

    std::vector<float> matrix, img;
    int height = 5;
    int weight = 5;
    getKernell(&matrix);

    if (currentProcess == 0) {
        getRandomImg(&img, weight, height);
    }

    std::vector<float> globalMatrix = getParallelOperations(matrix,
    img, weight, height);

    if (currentProcess == 0) {
        std::vector<float> referenceMatrix = getSequentialOperations(matrix,
        img, weight, height);
        ASSERT_EQ(globalMatrix, referenceMatrix);
    }
}

TEST(Parallel_Matrix_Multiplacition, mRows_Eq_mColumns_211) {
    int currentProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

    std::vector<float> matrix, img;
    int height = 211;
    int weight = 211;
    getKernell(&matrix);

    if (currentProcess == 0) {
        getRandomImg(&img, weight, height);
    }

    std::vector<float> globalMatrix = getParallelOperations(matrix,
    img, weight, height);

    if (currentProcess == 0) {
        std::vector<float> referenceMatrix = getSequentialOperations(matrix,
        img, weight, height);
        ASSERT_EQ(globalMatrix, referenceMatrix);
    }
}

TEST(Parallel_Matrix_Multiplacition, mRows_Gr_mColumns_150_100) {
    int currentProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
    std::vector<float> matrix, img;
    int height = 150;
    int weight = 100;
    getKernell(&matrix);
    if (currentProcess == 0) {
        getRandomImg(&img, weight, height);
    }

    std::vector<float> globalMatrix = getParallelOperations(matrix,
    img, weight, height);

    if (currentProcess == 0) {
        std::vector<float> referenceMatrix = getSequentialOperations(matrix,
        img, weight, height);
        ASSERT_EQ(globalMatrix, referenceMatrix);
    }
}

TEST(Parallel_Matrix_Multiplacition, mRows_Le_mColumns_100_150) {
    int currentProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);

    std::vector<float> matrix, img;
    int height = 100;
    int weight = 150;
    getKernell(&matrix);
    if (currentProcess == 0) {
        getRandomImg(&img, weight, height);
    }

    std::vector<float> globalMatrix = getParallelOperations(matrix,
    img, weight, height);

    if (currentProcess == 0) {
        std::vector<float> referenceMatrix = getSequentialOperations(matrix,
        img, weight, height);
        ASSERT_EQ(globalMatrix, referenceMatrix);
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
