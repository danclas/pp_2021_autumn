// Copyright 2021 Trevogin Kirill
#include "./canon.h"

TEST(cannons_mult_test, test_1) {
    const int size = 6;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    matrix<double> mat, mat1;
    if (rank == 0) {
        mat.prepareSpace(size, size);
        mat1.prepareSpace(size, size);
        mat.fillMatrix(generateRngValue, 0, 15, 0);
        mat1.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> mat_par = cannonsMultiplication(&mat, &mat1);
    if (rank == 0) {
        ASSERT_TRUE(mat_par == mat * mat1);
    }
}

TEST(cannons_mult_test, test_2) {
    const int size = 10;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    matrix<double> mat, mat1;
    if (rank == 0) {
        mat.prepareSpace(size, size);
        mat1.prepareSpace(size, size);
        mat.fillMatrix(generateRngValue, 0, 15, 0);
        mat1.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> mat_par = cannonsMultiplication(&mat, &mat1);
    if (rank == 0) {
        ASSERT_TRUE(mat_par == mat * mat1);
    }
}

TEST(cannons_mult_test, test_3) {
    const int size = 20;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    matrix<double> mat, mat1;
    if (rank == 0) {
        mat.prepareSpace(size, size);
        mat1.prepareSpace(size, size);
        mat.fillMatrix(generateRngValue, 0, 15, 0);
        mat1.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> mat_par = cannonsMultiplication(&mat, &mat1);
    if (rank == 0) {
        ASSERT_TRUE(mat_par == mat * mat1);
    }
}

TEST(cannons_mult_test, test_4) {
    const int size = 40;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    matrix<double> mat, mat1;
    if (rank == 0) {
        mat.prepareSpace(size, size);
        mat1.prepareSpace(size, size);
        mat.fillMatrix(generateRngValue, 0, 15, 0);
        mat1.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> mat_par = cannonsMultiplication(&mat, &mat1);
    if (rank == 0) {
        ASSERT_TRUE(mat_par == mat * mat1);
    }
}

TEST(cannons_mult_test, test_5) {
    const int size = 100;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    matrix<double> mat, mat1;
    if (rank == 0) {
        mat.prepareSpace(size, size);
        mat1.prepareSpace(size, size);
        mat.fillMatrix(generateRngValue, 0, 15, 0);
        mat1.fillMatrix(generateRngValue, 0, 15, 0);
    }
    matrix<double> mat_par = cannonsMultiplication(&mat, &mat1);
    if (rank == 0) {
        ASSERT_TRUE(mat_par == mat * mat1);
    }
}
TEST(cannons_mult_test, sravn_test) {
    const int size = 1000;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    matrix<double> mat, mat1;
    if (rank == 0) {
        mat.prepareSpace(size, size);
        mat1.prepareSpace(size, size);
        mat.fillMatrix(generateRngValue, 0, 15, 0);
        mat1.fillMatrix(generateRngValue, 0, 15, 0);
    }
    double t1 = MPI_Wtime();
    matrix<double> mat_par = cannonsMultiplication(&mat, &mat1);
    double t2 = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel " << (t2 - t1) << std::endl;
        t1 = MPI_Wtime();
        matrix<double> mat_seq = mat * mat1;
        t2 = MPI_Wtime();
        std::cout << "Sequential " << (t2 - t1) << std::endl;
        ASSERT_TRUE(mat_par == mat_seq);
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
