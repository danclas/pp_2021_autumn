// Copyright 2021 Vershinin Daniil
#include <gtest/gtest.h>

#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./mulit_step_optimization_search_area.h"

double F1(double x, double y) { return x * x + y * y; }
double F2(double x, double y) {
  return (1.5 - x + x * y) * (1.5 - x + x * y) +
         (2.25 - x + x * y * y) * (2.25 - x + x * y * y) +
         (2.625 - x + x * y * y * y) * (2.625 - x + x * y * y * y);
}
double F3(double x, double y) {
  return (x + 2 * y - 7) * (x + 2 * y - 7) + (2 * x + y - 5) * (2 * x + y - 5);
}

double F4(double x, double y) { return 0.26 * (x * x + y * y) - 0.48 * x * y; }

TEST(Sequential_Operations, Test_With_F1) {
  Point3d res = getSequentialOperations(-1, 1, -1, 1, F1);
  ASSERT_NEAR(res.x, 0, 0.001);
  ASSERT_NEAR(res.y, 0, 0.001);
}

TEST(Sequential_Operations, Test_With_F2) {
  Point3d res = getSequentialOperations(-4.5, 4.5, -4.5, 4.5, F2);
  ASSERT_NEAR(res.x, 3, 0.01);
  ASSERT_NEAR(res.y, 0.5, 0.01);
}

TEST(Sequential_Operations, Test_With_F3) {
  Point3d res = getSequentialOperations(-10, 10, -10, 10, F3);
  ASSERT_NEAR(res.x, 1, 0.001);
  ASSERT_NEAR(res.y, 3, 0.001);
}

TEST(Sequential_Operations, Test_With_F4) {
  Point3d res = getSequentialOperations(-10, 10, -10, 10, F4);
  ASSERT_NEAR(res.x, 0, 0.0015);
  ASSERT_NEAR(res.y, 0, 0.0015);
}

TEST(Parallel_Operations_MPI, Test_With_F1) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Point3d res;
  if (size > 1) {
    res = getParallelOperations(-1, 1, -1, 1, F1);
  } else {
    res = getSequentialOperations(-1, 1, -1, 1, F1);
  }

  if (rank == 0) {
    ASSERT_NEAR(res.x, 0, 0.001);
    ASSERT_NEAR(res.y, 0, 0.001);
  }
}

TEST(Parallel_Operations_MPI, Test_With_F2) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Point3d res;
  if (size > 1) {
    res = getParallelOperations(-4.5, 4.5, -4.5, 4.5, F2);
  } else {
    res = getSequentialOperations(-4.5, 4.5, -4.5, 4.5, F2);
  }

  if (rank == 0) {
    ASSERT_NEAR(res.x, 3, 0.01);
    ASSERT_NEAR(res.y, 0.5, 0.01);
  }
}

TEST(Parallel_Operations_MPI, Test_With_F3) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Point3d res;
  if (size > 1) {
    res = getParallelOperations(-10, 10, -10, 10, F3);
  } else {
    res = getSequentialOperations(-10, 10, -10, 10, F3);
  }

  if (rank == 0) {
    ASSERT_NEAR(res.x, 1, 0.001);
    ASSERT_NEAR(res.y, 3, 0.001);
  }
}
TEST(Parallel_Operations_MPI, Test_With_F4) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  Point3d res;
  if (size > 1) {
    res = getParallelOperations(-10, 10, -10, 10, F4);
  } else {
    res = getSequentialOperations(-10, 10, -10, 10, F4);
  }

  if (rank == 0) {
    ASSERT_NEAR(res.x, 0, 0.0015);
    ASSERT_NEAR(res.y, 0, 0.0015);
  }
}

// Test time
// TEST(Parallel_Operations_MPI, Test_With_F1) {
//   int rank, size;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   MPI_Comm_size(MPI_COMM_WORLD, &size);
//   double start = 0, end = 0.001;
//   Point3d res;
//   if (size > 1) {
//     start = MPI_Wtime();
//     res = getParallelOperations(-1, 1, -1, 1, F1);
//     end = MPI_Wtime();
//   }
//
//   if (rank == 0) {
//     double t1 = end - start;
//     start = MPI_Wtime();
//     Point3d check_res = getSequentialOperations(-1, 1, -1, 1, F1);
//     end = MPI_Wtime();
//     double t2 = end - start;
//
//     if (size == 1) {
//       res = check_res;
//     }
//     // printf("t2 / t1: %lf\n", t2 / t1);
//     // printf("res x: %lf, y: %lf, z: %lf \n", res.x, res.y, res.z);
//     // fflush(stdout);
//
//     ASSERT_NEAR(res.x, check_res.x, 0.001);
//     ASSERT_NEAR(res.y, check_res.y, 0.001);
//     ASSERT_NEAR(res.z, check_res.z, 0.001);
//   }
// }
//
// TEST(Parallel_Operations_MPI, Test_With_F2) {
//   int rank, size;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   MPI_Comm_size(MPI_COMM_WORLD, &size);
//   double start = 0, end = 0.001;
//   Point3d res;
//   if (size > 1) {
//     start = MPI_Wtime();
//     res = getParallelOperations(-4.5, 4.5, -4.5, 4.5, F2);
//     end = MPI_Wtime();
//   }
//
//   if (rank == 0) {
//     double t1 = end - start;
//     start = MPI_Wtime();
//     Point3d check_res = getSequentialOperations(-4.5, 4.5, -4.5, 4.5, F2);
//     end = MPI_Wtime();
//     double t2 = end - start;
//
//     if (size == 1) {
//       res = check_res;
//     }
//     // printf("t2 / t1: %lf\n", t2 / t1);
//     // printf("res x: %lf, y: %lf, z: %lf \n", res.x, res.y, res.z);
//     // fflush(stdout);
//
//     ASSERT_NEAR(res.x, check_res.x, 0.001);
//     ASSERT_NEAR(res.y, check_res.y, 0.001);
//     ASSERT_NEAR(res.z, check_res.z, 0.001);
//   }
// }
//
// TEST(Parallel_Operations_MPI, Test_With_F3) {
//   int rank, size;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   MPI_Comm_size(MPI_COMM_WORLD, &size);
//   double start = 0, end = 0.001;
//   Point3d res;
//   if (size > 1) {
//     start = MPI_Wtime();
//     res = getParallelOperations(-10, 10, -10, 10, F3);
//     end = MPI_Wtime();
//   }
//
//   if (rank == 0) {
//     double t1 = end - start;
//     start = MPI_Wtime();
//     Point3d check_res = getSequentialOperations(-10, 10, -10, 10, F3);
//     end = MPI_Wtime();
//     double t2 = end - start;
//
//     if (size == 1) {
//       res = check_res;
//     }
//     // printf("t2 / t1: %lf\n", t2 / t1);
//     // printf("res x: %lf, y: %lf, z: %lf \n", res.x, res.y, res.z);
//     // fflush(stdout);
//
//     ASSERT_NEAR(res.x, check_res.x, 0.001);
//     ASSERT_NEAR(res.y, check_res.y, 0.001);
//     ASSERT_NEAR(res.z, check_res.z, 0.001);
//   }
// }
// TEST(Parallel_Operations_MPI, Test_With_F4) {
//   int rank, size;
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   MPI_Comm_size(MPI_COMM_WORLD, &size);
//   double start = 0, end = 0.001;
//   Point3d res;
//   if (size > 1) {
//     start = MPI_Wtime();
//     res = getParallelOperations(-10, 10, -10, 10, F4);
//     end = MPI_Wtime();
//   }
//
//   if (rank == 0) {
//     double t1 = end - start;
//     start = MPI_Wtime();
//     Point3d check_res = getSequentialOperations(-10, 10, -10, 10, F4);
//     end = MPI_Wtime();
//     double t2 = end - start;
//
//     if (size == 1) {
//       res = check_res;
//     }
//     // printf("t2 / t1: %lf\n", t2 / t1);
//     // printf("res x: %lf, y: %lf, z: %lf \n", res.x, res.y, res.z);
//     // fflush(stdout);
//
//     ASSERT_NEAR(res.x, check_res.x, 0.0015);
//     ASSERT_NEAR(res.y, check_res.y, 0.0015);
//     ASSERT_NEAR(res.z, check_res.z, 0.0015);
//   }
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
