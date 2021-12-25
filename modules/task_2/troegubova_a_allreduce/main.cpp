// Copyright 2021 Troegubova Alexandra
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./allreduce.h"
#include <gtest-mpi-listener.hpp>


TEST(MPI_Allreduce_test, correct_finding_of_the_maximum_among_numbers_of_different_types) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float send_float = static_cast<float>(rank) + 0.5;
    double send_double = static_cast<double>(rank) + 1.e-4;

    int control_max_int;
    float control_max_float;
    double control_max_double;

    // double time_1 = MPI_Wtime();
    ALLreduce(&rank, &control_max_int, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    // double time_2 = MPI_Wtime();
    // std::cout << "The time spent on the execution of the test function: "
    //<< (time_2 - time_1) << " process rank: " << rank << "\n";

    ALLreduce(&send_float, &control_max_float, 1, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);
    ALLreduce(&send_double, &control_max_double, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    int exemp_int;
    float exemp_float;
    double exemp_double;

    // time_1 = MPI_Wtime();
    MPI_Allreduce(&rank, &exemp_int, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    // time_2 = MPI_Wtime();
    // std::cout << "Time spent on MPI_Allreduce execution " << (time_2 - time_1) << " process rank: " << rank << "\n";

    MPI_Allreduce(&send_float, &exemp_float, 1, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&send_double, &exemp_double, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        int control = 1;
        if ((control_max_int != exemp_int) ||
            (control_max_float != exemp_float) || (control_max_double != exemp_double))
            control = 0;
        ASSERT_EQ(control, 1);
    }
}

TEST(MPI_Allreduce_test, correct_finding_of_the_minimum_among_numbers_of_different_types) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float send_float = static_cast<float>(rank) + 2.5;
    double send_double = static_cast<double>(rank) + 2.6e-4;

    int control_min_int;
    float control_min_float;
    double control_min_double;

    ALLreduce(&rank, &control_min_int, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    ALLreduce(&send_float, &control_min_float, 1, MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD);
    ALLreduce(&send_double, &control_min_double, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    int exemp_int;
    float exemp_float;
    double exemp_double;

    MPI_Allreduce(&rank, &exemp_int, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&send_float, &exemp_float, 1, MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&send_double, &exemp_double, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        int control = 1;
        if ((control_min_int != exemp_int) ||
            (control_min_float != exemp_float) || (control_min_double != exemp_double))
            control = 0;
        ASSERT_EQ(control, 1);
    }
}

TEST(MPI_Allreduce_test, correct_finding_of_the_sum_of_numbers_of_different_types) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float send_float = static_cast<float>(rank) + 3.5;
    double send_double = static_cast<double>(rank) + 2.5;

    int control_sum_int;
    float control_sum_float;
    double control_sum_double;

    ALLreduce(&rank, &control_sum_int, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    ALLreduce(&send_float, &control_sum_float, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    ALLreduce(&send_double, &control_sum_double, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    int exemp_int;
    float exemp_float;
    double exemp_double;

    MPI_Allreduce(&rank, &exemp_int, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(&send_float, &exemp_float, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(&send_double, &exemp_double, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        int control = 1;
        if ((control_sum_int != exemp_int) ||
            (control_sum_float != exemp_float) || (control_sum_double != exemp_double))
            control = 0;
        ASSERT_EQ(control, 1);
    }
}
TEST(MPI_Allreduce_test, correct_finding_of_the_product_of_numbers_of_different_types) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float send_float = static_cast<float>(rank) / 6;
    double send_double = static_cast<double>(rank) / 3;

    int control_prod_int;
    float control_prod_float;
    double control_prod_double;

    ALLreduce(&rank, &control_prod_int, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);
    ALLreduce(&send_float, &control_prod_float, 1, MPI_FLOAT, MPI_PROD, MPI_COMM_WORLD);
    ALLreduce(&send_double, &control_prod_double, 1, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);

    int exemp_int;
    float exemp_float;
    double exemp_double;

    MPI_Allreduce(&rank, &exemp_int, 1, MPI_INT, MPI_PROD, MPI_COMM_WORLD);
    MPI_Allreduce(&send_float, &exemp_float, 1, MPI_FLOAT, MPI_PROD, MPI_COMM_WORLD);
    MPI_Allreduce(&send_double, &exemp_double, 1, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        int control = 1;
        if ((control_prod_int != exemp_int) ||
            (control_prod_float != exemp_float) || (control_prod_double != exemp_double))
            control = 0;
        ASSERT_EQ(control, 1);
    }
}

TEST(MPI_Allreduce_test, correct_finding_of_the_maximum_among_vectors_of_different_types) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 10;
    std::vector<int> send_int = getRandomVec<int>(size);
    std::vector<float> send_float = getRandomVec<float>(size);
    std::vector<double> send_double = getRandomVec<double>(size);

    std::vector<int> control_max_int(size);
    std::vector<float> control_max_float(size);
    std::vector<double> control_max_double(size);

    ALLreduce(send_int.data(), control_max_int.data(), size, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    ALLreduce(send_float.data(), control_max_float.data(), size, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);
    ALLreduce(send_double.data(), control_max_double.data(), size, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    std::vector<int> exemp_int(size);
    std::vector<float> exemp_float(size);
    std::vector<double> exemp_double(size);

    MPI_Allreduce(send_int.data(), exemp_int.data(), size, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(send_float.data(), exemp_float.data(), size, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(send_double.data(), exemp_double.data(), size, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        int control = 1;
        if ((control_max_int != exemp_int) ||
            (control_max_float != exemp_float) || (control_max_double != exemp_double))
            control = 0;
        ASSERT_EQ(control, 1);
    }
}

TEST(MPI_Allreduce_test, correct_finding_of_the_minimum_among_vectors_of_different_types) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 15;
    std::vector<int> send_int = getRandomVec<int>(size);
    std::vector<float> send_float = getRandomVec<float>(size);
    std::vector<double> send_double = getRandomVec<double>(size);

    std::vector<int> control_min_int(size);
    std::vector<float> control_min_float(size);
    std::vector<double> control_min_double(size);

    ALLreduce(send_int.data(), control_min_int.data(), size, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    ALLreduce(send_float.data(), control_min_float.data(), size, MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD);
    ALLreduce(send_double.data(), control_min_double.data(), size, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    std::vector<int> exemp_int(size);
    std::vector<float> exemp_float(size);
    std::vector<double> exemp_double(size);

    MPI_Allreduce(send_int.data(), exemp_int.data(), size, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(send_float.data(), exemp_float.data(), size, MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(send_double.data(), exemp_double.data(), size, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        int control = 1;
        if ((control_min_int != exemp_int) ||
            (control_min_float != exemp_float) || (control_min_double != exemp_double))
            control = 0;
        ASSERT_EQ(control, 1);
    }
}

TEST(MPI_Allreduce_test, correct_finding_of_the_sum_of_vectors_of_different_types) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 3;
    std::vector<int> send_int = getRandomVec<int>(size);
    std::vector<float> send_float = getRandomVec<float>(size);
    std::vector<double> send_double = getRandomVec<double>(size);

    std::vector<int> control_sum_int(size);
    std::vector<float> control_sum_float(size);
    std::vector<double> control_sum_double(size);

    ALLreduce(send_int.data(), control_sum_int.data(), size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    ALLreduce(send_float.data(), control_sum_float.data(), size, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    ALLreduce(send_double.data(), control_sum_double.data(), size, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    std::vector<int> exemp_int(size);
    std::vector<float> exemp_float(size);
    std::vector<double> exemp_double(size);

    MPI_Allreduce(send_int.data(), exemp_int.data(), size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(send_float.data(), exemp_float.data(), size, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(send_double.data(), exemp_double.data(), size, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        int control = 1;
        if ((control_sum_int != exemp_int) ||
            (control_sum_float != exemp_float) || (control_sum_double != exemp_double))
            control = 0;
        ASSERT_EQ(control, 1);
    }
}

TEST(MPI_Allreduce_test, correct_finding_of_the_product_of_vectors_of_different_types) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 3;
    std::vector<int> send_int = getRandomVec<int>(size);
    std::vector<float> send_float = getRandomVec<float>(size);
    std::vector<double> send_double = getRandomVec<double>(size);

    std::vector<int> control_prod_int(size);
    std::vector<float> control_prod_float(size);
    std::vector<double> control_prod_double(size);

    ALLreduce(send_int.data(), control_prod_int.data(), size, MPI_INT, MPI_PROD, MPI_COMM_WORLD);
    ALLreduce(send_float.data(), control_prod_float.data(), size, MPI_FLOAT, MPI_PROD, MPI_COMM_WORLD);
    ALLreduce(send_double.data(), control_prod_double.data(), size, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);

    std::vector<int> exemp_int(size);
    std::vector<float> exemp_float(size);
    std::vector<double> exemp_double(size);

    MPI_Allreduce(send_int.data(), exemp_int.data(), size, MPI_INT, MPI_PROD, MPI_COMM_WORLD);
    MPI_Allreduce(send_float.data(), exemp_float.data(), size, MPI_FLOAT, MPI_PROD, MPI_COMM_WORLD);
    MPI_Allreduce(send_double.data(), exemp_double.data(), size, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        int control = 1;
        if ((control_prod_int != exemp_int) || (control_prod_float != exemp_float)
            || (control_prod_double != exemp_double))
            control = 0;
        ASSERT_EQ(control, 1);
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
