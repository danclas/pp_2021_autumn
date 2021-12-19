// Copyright 2021 Ivanov Arkadiy
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include <cmath>
#include "./matrix_multiplication.h"
#include <gtest-mpi-listener.hpp>


TEST(generator_functions_test, rng_generator_has_both_positive_and_negative) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        const int numbersToCheck = 150;
        bool hasPositive = false;
        bool hasNegative = false;
        for (int i = 0; i < numbersToCheck; i++) {
            int value = generateRngValue(1, 100, nullptr);
            if (value > 0)
                hasPositive = true;
            else if (value < 0)
                hasNegative = true;
            if (hasPositive && hasNegative)
                break;
        }
        ASSERT_TRUE((hasPositive == true) && (hasPositive == hasNegative));
    }
}

TEST(generator_functions_test, rng_generator_generates_in_range) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        const int numbersToCheck = 150;
        const int minNum = 1;
        const int maxNum = 2530;
        bool ok = true;
        for (int i = 0; i < numbersToCheck && ok; i++) {
            int value = generateRngValue(1, 100, nullptr);
            if (abs(value) < minNum || abs(value) > maxNum)
                ok = false;
        }
        ASSERT_TRUE(ok);
    }
}

TEST(generator_functions_test, rising_generator_check) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        bool ok = true;
        int feedback = 0;
        for (int i = 0; i < 150 && ok; i++)
            if (generateRisingValue(2, 0, &feedback) != i + 2)
                ok = false;
        ASSERT_TRUE(ok);
    }
}

TEST(generator_functions_test, falling_generator_check) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        bool ok = true;
        int feedback = 0;
        for (int i = 0; i < 150 && ok; i++)
            if (generateFallingValue(0, 12, &feedback) != 12 - i)
                ok = false;
        ASSERT_TRUE(ok);
    }
}

TEST(generator_functions_test, jumping_generator_check) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        bool ok = true;
        int feedback = 0;
        const int v1 = 15, v2 = -14;
        for (int i = 0; i < 20 && ok; i++) {
            int value = generateJumpingValue(v1, v2, &feedback);
            if (i % 2 == 0) {
                if (value != v1)
                    ok = false;
            } else {
                if (value != v2)
                    ok = false;
            }
        }
        ASSERT_TRUE(ok);
    }
}

TEST(matrix_class_check, default_constructor_check) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix<int> a;
        if (a.data() != nullptr) {
            ASSERT_TRUE(false);
        }
        if ((a.getNumColums() != 0) || (a.getNumRows() != 0)) {
            ASSERT_TRUE(false);
        }
        ASSERT_TRUE(true);
    }
}

TEST(matrix_class_check, parameterized_constructor_check) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix<int> a(2, 4);
        if (a.getNumRows() != 2) {
            ASSERT_TRUE(false);
        }
        if (a.getNumColums() != 4) {
            ASSERT_TRUE(false);
        }
        ASSERT_NO_THROW({
            int* matrixData = a.data();
            for (int i = 0; i < 2 * 4; i++)
                matrixData[i] = 0; });
    }
}

TEST(matrix_class_check, matrix_fill_test) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix<int> a(2, 4);
        a.fillMatrix(generateRisingValue, 3, 0, 0);
        int* matrixData = a.data();
        bool ok = true;
        for (int i = 0; i < 2 * 4 && ok; i++)
            if (matrixData[i] != i + 3)
                ok = false;
        ASSERT_TRUE(ok);
    }
}

TEST(matrix_class_check, indexation_check) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix<int> a(3, 8);
        ASSERT_NO_THROW({
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 8; j++)
                    a[i][j] = 0; });
    }
}

TEST(matrix_class_check, copy_constructor_check) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix<int> a(4, 7);
        a.fillMatrix(generateFallingValue, 0, 10, 0);
        bool ok = true;
        matrix<int> b(a);
        for (int i = 0; i < 4 && ok; i++)
            for (int j = 0; j < 7 && ok; j++)
                if (a[i][j] != b[i][j])
                    ok = false;
        ASSERT_TRUE(ok);
    }
}

TEST(matrix_class_check, operator_is_equal_check) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix<int> a(4, 19);
        a.fillMatrix(generateRisingValue, 15, 0, 0);
        matrix<int> b(a);
        ASSERT_TRUE(a == b);
    }
}

TEST(matrix_class_check, operator_equal_check_1) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix<int> a(4, 19);
        a.fillMatrix(generateRisingValue, 15, 0, 0);
        matrix<int> b;
        b = a;
        ASSERT_TRUE(a == b);
    }
}

TEST(matrix_class_check, operator_equal_check_2) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix<int> a(4, 19);
        a.fillMatrix(generateRisingValue, 15, 0, 0);
        matrix<int> b(12, 41);
        b = a;
        ASSERT_TRUE(a == b);
    }
}

TEST(matrix_class_check, operator_multiply_check_1) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix<int> a(2, 4), b(4, 2);
        a.fillMatrix(generateRisingValue, 0, 0, 0);
        b.fillMatrix(generateFallingValue, 0, 15, 0);
        matrix<int> c = a * b;
        if ((c.getNumRows() != 2) && (c.getNumColums() != 2)) {
            ASSERT_TRUE(false);
        }
        ASSERT_TRUE(c[0][0] == 62 && c[0][1] == 56
            && c[1][0] == 254 && c[1][1] == 232);
    }
}

TEST(matrix_class_check, operator_multiply_check_2) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        matrix<int> a(2, 4), b(4, 1);
        a.fillMatrix(generateRisingValue, 0, 0, 0);
        b.fillMatrix(generateFallingValue, 0, 15, 0);
        matrix<int> c = a * b;
        if ((c.getNumRows() != 2) && (c.getNumColums() != 1)) {
            ASSERT_TRUE(false);
        }
        ASSERT_TRUE(c[0][0] == 76 && c[1][0] == 292);
    }
}

TEST(parallel_multiplication_test, can_multiply_vectors) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<int> a, b;
    if (procRank == 0) {
        a.prepareSpace(1, 11);
        b.prepareSpace(11, 1);
        a.fillMatrix(generateRisingValue, 0, 0, 0);
        b.fillMatrix(generateFallingValue, 0, 15, 0);
    }
    matrix<int> c = parallelMultiplication(&a, &b, MPI_INT);
    if (procRank == 0) {
        ASSERT_TRUE(c == a * b);
    }
}

TEST(parallel_multiplication_test, vector_on_matrix) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<int> a, b;
    if (procRank == 0) {
        a.prepareSpace(1, 11);
        b.prepareSpace(11, 15);
        a.fillMatrix(generateRngValue, 0, 496845, 0);
        b.fillMatrix(generateRngValue, 0, 912341, 0);
    }
    matrix<int> c = parallelMultiplication(&a, &b, MPI_INT);
    if (procRank == 0) {
        ASSERT_TRUE(c == a * b);
    }
}

TEST(parallel_multiplication_test, matrix2_on_matrix) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<int> a, b;
    if (procRank == 0) {
        a.prepareSpace(2, 6);
        b.prepareSpace(6, 11);
        a.fillMatrix(generateRngValue, 0, 496845, 0);
        b.fillMatrix(generateRngValue, 0, 912341, 0);
    }
    matrix<int> c = parallelMultiplication(&a, &b, MPI_INT);
    if (procRank == 0) {
        ASSERT_TRUE(c == a * b);
    }
}

TEST(parallel_multiplication_test, matrix_on_vector) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<int> a, b;
    if (procRank == 0) {
        a.prepareSpace(19, 11);
        b.prepareSpace(11, 1);
        a.fillMatrix(generateRngValue, 0, 496845, 0);
        b.fillMatrix(generateRngValue, 0, 912341, 0);
    }
    matrix<int> c = parallelMultiplication(&a, &b, MPI_INT);
    if (procRank == 0) {
        ASSERT_TRUE(c == a * b);
    }
}

TEST(parallel_multiplication_test, matrix_on_matrix) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<int> a, b;
    if (procRank == 0) {
        a.prepareSpace(34, 21);
        b.prepareSpace(21, 17);
        a.fillMatrix(generateRngValue, 0, 496845, 0);
        b.fillMatrix(generateRngValue, 0, 912341, 0);
    }
    matrix<int> c = parallelMultiplication(&a, &b, MPI_INT);
    if (procRank == 0) {
        ASSERT_TRUE(c == a * b);
    }
}

TEST(parallel_multiplication_test, can_work_with_double_matrix) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<double> a, b;
    if (procRank == 0) {
        a.prepareSpace(14, 13);
        b.prepareSpace(13, 17);
        a.fillMatrix(generateRngValue, 0, 496845, 0);
        b.fillMatrix(generateRngValue, 0, 912341, 0);
    }
    matrix<double> c = parallelMultiplication(&a, &b, MPI_DOUBLE);
    if (procRank == 0) {
        ASSERT_TRUE(c == a * b);
    }
}

TEST(parallel_multiplication_test, can_work_with_float_matrix) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<float> a, b;
    if (procRank == 0) {
        a.prepareSpace(23, 21);
        b.prepareSpace(21, 11);
        a.fillMatrix(generateRngValue, 0, 496845, 0);
        b.fillMatrix(generateRngValue, 0, 912341, 0);
    }
    matrix<float> c = parallelMultiplication(&a, &b, MPI_FLOAT);
    if (procRank == 0) {
        ASSERT_TRUE(c == a * b);
    }
}
/*
TEST(parallel_multiplication_test, matrix_on_matrix_with_time) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    matrix<int> a, b;
    if (procRank == 0) {
        a.prepareSpace(1333, 1000);
        b.prepareSpace(1000, 1444);
        a.fillMatrix(generateRngValue, 0, 32, 0);
        b.fillMatrix(generateRngValue, 0, 32, 0);
    }
    double parallel_before = MPI_Wtime();
    matrix<int> c = parallelMultiplication(&a, &b, MPI_INT);
    double parallel_after = MPI_Wtime();
    if (procRank == 0) {
        double sequential_before = MPI_Wtime();
        matrix<int> c2 = a * b;
        double sequential_after = MPI_Wtime();
        std::cout << (sequential_after - sequential_before) / (parallel_after - parallel_before) << std::endl;
        std::cout << (sequential_after - sequential_before) << " " << (parallel_after - parallel_before) << std::endl;
        ASSERT_TRUE(c == c2);
    }
}
*/

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);  // check for MPI_SUCCESS?

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);  // just comment
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
