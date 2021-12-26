// Copyright 2021 Lakhov Kirill

#include <gtest/gtest.h>
#include <iostream>
#include "./optimization_params.h"
#include <gtest-mpi-listener.hpp>

TEST(Global_Params_Optimization_MPI, Test_Func1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double left_x = -1;
    double right_x = 1;
    double left_y = -2;
    double right_y = 2;
    double(*func1)(double, double) = f1;
    double time1, time2, delta1, delta2;
    if (rank == 0) {
        time1 = MPI_Wtime();
    }
    Point parallel_result = parralelCalc(left_x, right_x,
                                                 left_y, right_y, func1);
    if (rank == 0) {
        time2 = MPI_Wtime();
        delta1 = time2 - time1;
        std::cout << "Parallel time " << delta1 << std::endl;
        time1 = MPI_Wtime();
        Point seq_result = sequentialCalc(left_x, right_x,
                                                  left_y, right_y, func1);
        time2 = MPI_Wtime();
        delta2 = time2 - time1;
        std::cout << "Sequental time " << delta2 << std::endl;
        std::cout << "sequental/parallel ratio " << delta2/delta1 << std::endl;
        bool r = comparePoints(parallel_result, seq_result);
        EXPECT_TRUE(r);
    }
}

TEST(Global_Params_Optimization_MPI, Test_Func2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double left_x = -1;
    double right_x = 1;
    double left_y = -2;
    double right_y = 2;
    double(*func2)(double, double) = f2;
    double time1, time2, delta1, delta2;
    if (rank == 0) {
        time1 = MPI_Wtime();
    }
    Point parallel_result = parralelCalc(left_x, right_x,
                                                 left_y, right_y, func2);
    if (rank == 0) {
        time2 = MPI_Wtime();
        delta1 = time2 - time1;
        std::cout << "Parallel time " << delta1 << std::endl;
        time1 = MPI_Wtime();
        Point seq_result = sequentialCalc(left_x, right_x,
                                                  left_y, right_y, func2);
        time2 = MPI_Wtime();
        delta2 = time2 - time1;
        std::cout << "Sequental time " << delta2 <<std::endl;
        std::cout << "sequental/parallel ratio " << delta2/delta1 <<std::endl;
        bool r = comparePoints(parallel_result, seq_result);
        EXPECT_TRUE(r);
    }
}

TEST(Global_Params_Optimization_MPI, Test_Func3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double left_x = 0.5;
    double right_x = 2;
    double left_y = 0.5;
    double right_y = 2;
    double(*func3)(double, double) = f3;
    double time1, time2, delta1, delta2;
    if (rank == 0) {
        time1 = MPI_Wtime();
    }
    Point parallel_result = parralelCalc(left_x, right_x,
                                                 left_y, right_y, func3);
    if (rank == 0) {
        time2 = MPI_Wtime();
        delta1 = time2 - time1;
        std::cout << "Parallel time " << delta1 << std::endl;
        time1 = MPI_Wtime();
        Point seq_result = sequentialCalc(left_x, right_x,
                                                  left_y, right_y, func3);
        time2 = MPI_Wtime();
        delta2 = time2 - time1;
        std::cout << "Sequental time " << delta2 << std::endl;
        std::cout << "sequental/parallel ratio " << delta2/delta1 << std::endl;
        bool r = comparePoints(parallel_result, seq_result);
        EXPECT_TRUE(r);
    }
}

TEST(Global_Params_Optimization_MPI, Test_Func4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double left_x = 0;
    double right_x = 2;
    double left_y = 0;
    double right_y = 2;
    double time1, time2, delta1, delta2;
    double(*func4)(double, double) = f1;
    if (rank == 0) {
        time1 = MPI_Wtime();
    }
    Point parallel_result = parralelCalc(left_x, right_x,
                                                 left_y, right_y, func4);
    if (rank == 0) {
        time2 = MPI_Wtime();
        delta1 = time2 - time1;
        std::cout << "Parallel time "<< delta1 << std::endl;
        time1 = MPI_Wtime();
        Point seq_result = sequentialCalc(left_x, right_x,
                                                  left_y, right_y, func4);
        time2 = MPI_Wtime();
        delta2 = time2 - time1;
        std::cout << "Sequental time " << delta2 << std::endl;
        std::cout << "sequental/parallel ratio " << delta2/delta1 << std::endl;
        bool r = comparePoints(parallel_result, seq_result);
        EXPECT_TRUE(r);
    }
}

TEST(Global_Params_Optimization_MPI, Test_Func5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double left_x = -1.5;
    double right_x = 1;
    double left_y = 0;
    double right_y = 1.5;
    double time1, time2, delta1, delta2;
    double(*func5)(double, double) = f1;
    if (rank == 0) {
        time1 = MPI_Wtime();
    }
    Point parallel_result = parralelCalc(left_x, right_x,
                                                 left_y, right_y, func5);
    if (rank == 0) {
        time2 = MPI_Wtime();
        delta1 = time2 - time1;
        std::cout << "Parallel time " << delta1 << std::endl;
        time1 = MPI_Wtime();
        Point seq_result = sequentialCalc(left_x, right_x,
                                                  left_y, right_y, func5);
        time2 = MPI_Wtime();
        delta2 = time2 - time1;
        std::cout << "Sequental time " << delta2 << std::endl;
        std::cout << "sequental/parallel ratio " << delta2/delta1 << std::endl;
        bool r = comparePoints(parallel_result, seq_result);
        EXPECT_TRUE(r);
    }
}

// Test for time measure
// TEST(Global_Params_Optimization_MPI, Test_Func6) {
//     int rank;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     double left_x = -1;
//     double right_x = 2;
//     double left_y = -2;
//     double right_y = 2;
//     double(*func1)(double, double) = f1;
//     double time1, time2, delta1, delta2;
//     if (rank == 0) {
//         time1 = MPI_Wtime();
//     }
//     Point parallel_result = parralelCalc(left_x, right_x,
//                                                  left_y, right_y, func1);
//     if (rank == 0) {
//         time2 = MPI_Wtime();
//         delta1 = time2 - time1;
//         std::cout << "Parallel time " << delta1 << std::endl;
//         time1 = MPI_Wtime();
//         Point seq_result = sequentialCalc(left_x, right_x,
//                                                   left_y, right_y, func1);
//         time2 = MPI_Wtime();
//         delta2 = time2 - time1;
//         std::cout << "Sequental time " << delta2 << std::endl;
//         std::cout << "sequental/parallel ratio "<< delta2/delta1
// << std::endl;
//         bool r = comparePoints(parallel_result, seq_result);
//         EXPECT_TRUE(r);
//     }
// }

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
