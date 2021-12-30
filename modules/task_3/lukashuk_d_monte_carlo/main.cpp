// Copyright 2021 Lukashuk Diana
#include <gtest/gtest.h>
#include "./monte_carlo.h"
#include <gtest-mpi-listener.hpp>

double func_1(std::vector<double> v) {
  return 7 * v[0] * v[0] + 6 * v[1] * v[1] * v[1];
}

double func_2(std::vector<double> v) {
  return atan(v[0]) + 7 * sin(v[1]) + 3 * v[2] * v[2];
}

double func_3(std::vector<double> v) {
  return v[0] + 8 * v[1] + v[2] * v[2] * v[2] - cos(v[3]);
}

double func_4(std::vector<double> v) {
  return 5 * v[0] * v[0] * v[0] - 2 * v[1] + sin(v[2] * v[3] * v[4]);
}

TEST(Monte_Carlo_MPI, dimension_is_2) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

  // double start, end;
  // double seq_time, par_time;

  int points = 100000;
  std::vector<double> lower = {0.0, 1.0};
  std::vector<double> upper = {1.0, 2.0};

  std::vector<double> node = getRandomNode(lower, upper, points);
  std::vector<double> upper_lower = upperLower(lower, upper);

  // start = MPI_Wtime();
  double par = getParallel(upper_lower, node, points, func_1);

  if (proc_rank == 0) {
    // end = MPI_Wtime();
    // par_time = end - start;

    // start = MPI_Wtime();
    double seq = getSequential(upper_lower, node, points, func_1);
    // end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    ASSERT_NEAR(par, seq, 1);
  }
}

TEST(Monte_Carlo_MPI, dimension_is_3) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

  // double start, end;
  // double seq_time, par_time;

  int points = 100000;
  std::vector<double> lower = {0.5, 1.5, 3.0};
  std::vector<double> upper = {2.0, 3.0, 5.5};

  std::vector<double> node = getRandomNode(lower, upper, points);
  std::vector<double> upper_lower = upperLower(lower, upper);

  // start = MPI_Wtime();
  double par = getParallel(upper_lower, node, points, func_2);

  if (proc_rank == 0) {
    // end = MPI_Wtime();
    // par_time = end - start;

    // start = MPI_Wtime();
    double seq = getSequential(upper_lower, node, points, func_2);
    // end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    ASSERT_NEAR(par, seq, 1);
  }
}

TEST(Monte_Carlo_MPI, dimension_is_4) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

  // double start, end;
  // double seq_time, par_time;

  int points = 100000;
  std::vector<double> lower = {-2.0, 1.3, -4.0, 0.0};
  std::vector<double> upper = {0.0, 3.6, -1.0, 2.0};

  std::vector<double> node = getRandomNode(lower, upper, points);
  std::vector<double> upper_lower = upperLower(lower, upper);

  // start = MPI_Wtime();
  double par = getParallel(upper_lower, node, points, func_3);

  if (proc_rank == 0) {
    // end = MPI_Wtime();
    // par_time = end - start;

    // start = MPI_Wtime();
    double seq = getSequential(upper_lower, node, points, func_3);
    // end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    ASSERT_NEAR(par, seq, 1);
  }
}

TEST(Monte_Carlo_MPI, dimension_is_4_with_more_points) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

  // double start, end;
  // double seq_time, par_time;

  int points = 1000000;
  std::vector<double> lower = {-2.0, 1.3, -4.0, 0.0};
  std::vector<double> upper = {0.0, 3.6, -1.0, 2.0};

  std::vector<double> node = getRandomNode(lower, upper, points);
  std::vector<double> upper_lower = upperLower(lower, upper);

  // start = MPI_Wtime();
  double par = getParallel(upper_lower, node, points, func_3);

  if (proc_rank == 0) {
    // end = MPI_Wtime();
    // par_time = end - start;

    // start = MPI_Wtime();
    double seq = getSequential(upper_lower, node, points, func_3);
    // end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    ASSERT_NEAR(par, seq, 1);
  }
}

TEST(Monte_Carlo_MPI, dimension_is_5) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

  // double start, end;
  // double seq_time, par_time;

  int points = 100000;
  std::vector<double> lower = {0.0, 1.5, 1.0, 3.0, 2.0};
  std::vector<double> upper = {4.0, 2.5, 5.0, 6.0, 3.0};

  std::vector<double> node = getRandomNode(lower, upper, points);
  std::vector<double> upper_lower = upperLower(lower, upper);

  // start = MPI_Wtime();
  double par = getParallel(upper_lower, node, points, func_4);

  if (proc_rank == 0) {
    // end = MPI_Wtime();
    // par_time = end - start;

    // start = MPI_Wtime();
    double seq = getSequential(upper_lower, node, points, func_4);
    // end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    ASSERT_NEAR(par, seq, 1);
  }
}

TEST(Monte_Carlo_MPI, dimension_is_5_with_more_points) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

  // double start, end;
  // double seq_time, par_time;

  int points = 1000000;
  std::vector<double> lower = {0.0, 1.5, 1.0, 3.0, 2.0};
  std::vector<double> upper = {4.0, 2.5, 5.0, 6.0, 3.0};

  std::vector<double> node = getRandomNode(lower, upper, points);
  std::vector<double> upper_lower = upperLower(lower, upper);

  // start = MPI_Wtime();
  double par = getParallel(upper_lower, node, points, func_4);

  if (proc_rank == 0) {
    // end = MPI_Wtime();
    // par_time = end - start;

    // start = MPI_Wtime();
    double seq = getSequential(upper_lower, node, points, func_4);
    // end = MPI_Wtime();
    // seq_time = end - start;

    // std::cout << "Par: " << par_time << "\nSeq: " << seq_time << '\n';

    ASSERT_NEAR(par, seq, 1);
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
