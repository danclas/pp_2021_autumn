// Copyright 2021 Borisov Maxim

#include <gtest/gtest.h>
#include <cmath>
#include "../../../modules/task_3/borisov_m_monte_carlo_integration/monte_carlo_integration.h"
#include <gtest-mpi-listener.hpp>

double second_integral(std::vector<double> f) {
    double tmp = 2 * f[0] * f[1] + (f[1 * f[1]]) / 2;
    return tmp;
}

double third_integral(std::vector<double> f) {
    double tmp = 8 * std::pow(f[1], 2) * std::pow(f[2], f[0] * f[1]);
    return tmp;
}

double fourth_integral(std::vector<double> f) {
    double tmp = f[0] * f[1] / (f[3] * sin(f[2] + f[1]));
    return tmp;
}

double fifth_integral(std::vector<double> f) {
    double tmp = (f[0] * f[1] + f[2] * f[3]) / cos(f[4]);
    return tmp;
}

TEST(MPI_Parallel, d_2_points_size_100001) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    std::vector<double> up = { 3.0, 2.0};
    std::vector<double> lo = { 0.0, 0.0};
    int points_size = 900001;
    double res_seq;
    double res_par;
    double error;
    double start_of_par;
    double end_of_par;
    double start_of_seq;
    double end_of_seq;
    start_of_par = MPI_Wtime();
    res_par = parallel_monte_carlo(up, lo, points_size, second_integral);
    end_of_par = MPI_Wtime();
    if (proc_rank == 0) {
        start_of_seq = MPI_Wtime();
        res_seq = sequential_monte_carlo(up, lo, points_size, second_integral);
        end_of_seq = MPI_Wtime();
        std::cout << "res_seq: " << res_seq << "\n";
        std::cout << "res_par: " << res_par << "\n";
        std::cout << "par_time: " << end_of_par - start_of_par << "\n";
        std::cout << "seq_time: " << end_of_seq - start_of_seq << "\n";
        error = abs(res_seq - res_par);
        ASSERT_NEAR(res_par, res_seq, error);
    }
}

TEST(MPI_Parallel, d_3_points_size_10003) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    std::vector<double> up = { 1.0, 2.0, 4.3 };
    std::vector<double> lo = { 0.5, 1.7, 2.8 };
    int points_size = 10003;
    double res_seq;
    double res_par;
    double error;
    res_par = parallel_monte_carlo(up, lo, points_size, third_integral);
    if (proc_rank == 0) {
        res_seq = sequential_monte_carlo(up, lo, points_size, third_integral);
        error = abs(res_seq - res_par);
        ASSERT_NEAR(res_par, res_seq, error);
    }
}

TEST(MPI_Parallel, d_4_points_size_10005) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    std::vector<double> up = {0.0, 1.6, -0.5, 1.7};
    std::vector<double> lo = {-1.9, -3.1, -2.0, 0.0};
    int points_size = 10005;
    double res_seq;
    double res_par;
    double error;
    res_par = parallel_monte_carlo(up, lo, points_size, fourth_integral);
    if (proc_rank == 0) {
        res_seq = sequential_monte_carlo(up, lo, points_size, fourth_integral);
        error = abs(res_seq - res_par);
        ASSERT_NEAR(res_par, res_seq, error);
    }
}

TEST(MPI_Parallel, d_4_points_size_10009) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    std::vector<double> up = {0.0, 1.6, -0.5, 1.7};
    std::vector<double> lo = {-1.9, -3.1, -2.0, 0.0};
    int points_size = 10009;
    double res_seq;
    double res_par;
    double error;
    res_par = parallel_monte_carlo(up, lo, points_size, fourth_integral);
    if (proc_rank == 0) {
        res_seq = sequential_monte_carlo(up, lo, points_size, fourth_integral);
        error = abs(res_seq - res_par);
        ASSERT_NEAR(res_par, res_seq, error);
    }
}

TEST(MPI_Parallel, d_5_points_size_100008) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    std::vector<double> up = {1.0, 1.7, 3.2, 5.5, 2.0};
    std::vector<double> lo = {0.0, 1.5, 1.1, 3.7, 0.0};
    int points_size = 100008;
    double res_seq;
    double res_par;
    double error;
    res_par = parallel_monte_carlo(up, lo, points_size, fifth_integral);
    if (proc_rank == 0) {
        res_seq = sequential_monte_carlo(up, lo, points_size, fifth_integral);
        error = abs(res_seq - res_par);
        ASSERT_NEAR(res_par, res_seq, error);
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
