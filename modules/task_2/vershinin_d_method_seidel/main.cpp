// Copyright 2021 Vershinin Daniil
#include <gtest/gtest.h>

#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./method_seidel.h"

TEST(Sequential_Operations, Test_On_Own_Matrix1) {
  const double eps = 0.0001;
  std::vector<std::vector<double>> matrix_A = {
      {10, 1, 1},
      {2, 10, 1},
      {2, 2, 10},
  };

  std::vector<double> b = {12, 13, 14};

  std::vector<double> res = getSequentialOperations(matrix_A, b, eps);
  ASSERT_NEAR(res[0], 1, 0.001);
  ASSERT_NEAR(res[1], 1, 0.001);
  ASSERT_NEAR(res[2], 1, 0.001);
}

TEST(Sequential_Operations, Test_On_Own_Matrix2) {
  const double eps = 0.0001;
  std::vector<std::vector<double>> matrix_A = {
      {10, -1, 1},
      {1, 10, -3},
      {1, -2, 10},
  };

  std::vector<double> b = {10, 8, 9};

  std::vector<double> res = getSequentialOperations(matrix_A, b, eps);
  ASSERT_NEAR(res[0], 1, 0.001);
  ASSERT_NEAR(res[1], 1, 0.001);
  ASSERT_NEAR(res[2], 1, 0.001);
}
// disabled
//
// TEST(Inversion_Operation, Test_On_Own_Matrix) {
//  std::vector<std::vector<double>> matrix_A = {
//      {10, 0, 0},
//      {2, 10, 0},
//      {2, 2, 10},
//  };
//
//  std::vector<std::vector<double>> expect{
//      {1.0 / 10.0, 0, 0},
//      {-1.0 / 50.0, 1.0 / 10.0, 0},
//      {-2.0 / 125.0, -1.0 / 50.0, 1.0 / 10.0}};
//
//  std::vector<std::vector<double>> res = inversion(matrix_A);
//  for (int i = 0; i < 3; i++) {
//    for (int j = 0; j < 3; j++) {
//      ASSERT_NEAR(res[i][j], expect[i][j], 0.001);
//    }
//  }
//}
// disabled
//
// TEST(MultOnNumber_Operation, Test_On_Own_Matrix) {
//  std::vector<std::vector<double>> matrix_A = {
//      {10, 1, 1},
//      {2, 10, 1},
//      {2, 2, 10},
//  };
//
//  std::vector<std::vector<double>> expect{
//      {50, 5, 5},
//      {10, 50, 5},
//      {10, 10, 50},
//  };
//
//  std::vector<std::vector<double>> res = multNumberOnMatrix(5, matrix_A);
//  for (int i = 0; i < 3; i++) {
//    for (int j = 0; j < 3; j++) {
//      ASSERT_NEAR(res[i][j], expect[i][j], 0.001);
//    }
//  }
//}
// disabled
//
// TEST(MultOnMatrix_Operation, Test_On_Own_Matrix) {
//  std::vector<std::vector<double>> matrix_A = {
//      {10, 1, 1},
//      {2, 10, 1},
//      {2, 2, 10},
//  };
//
//  std::vector<std::vector<double>> matrix_B{
//      {1, 0, 0},
//      {0, 1, 0},
//      {0, 0, 1},
//  };
//
//  std::vector<std::vector<double>> res = multMatrixOnMatrix(matrix_A,
//  matrix_B); for (int i = 0; i < 3; i++) {
//    for (int j = 0; j < 3; j++) {
//      ASSERT_NEAR(res[i][j], matrix_A[i][j], 0.001);
//    }
//  }
//}
// disabled
//
// TEST(MultOnMatrix_Operation, Test_On_Own_Matrix2) {
//  std::vector<std::vector<double>> matrix_A = {
//      {10, 1, 1},
//      {2, 10, 1},
//      {2, 2, 10},
//  };
//
//  std::vector<std::vector<double>> matrix_B{
//      {1, 1, 1},
//      {1, 1, 1},
//      {1, 1, 1},
//  };
//
//  std::vector<std::vector<double>> expect{
//      {12, 12, 12}, {13, 13, 13}, {14, 14, 14}};
//
//  std::vector<std::vector<double>> res = multMatrixOnMatrix(matrix_A,
//  matrix_B); for (int i = 0; i < 3; i++) {
//    for (int j = 0; j < 3; j++) {
//      ASSERT_NEAR(res[i][j], expect[i][j], 0.001);
//    }
//  }
//}
// disabled
//
// TEST(MultOnVector_Operation, Test_On_Own_Matrix) {
//  std::vector<std::vector<double>> matrix_A = {
//      {10, 1, 1},
//      {2, 10, 1},
//      {2, 2, 10},
//  };
//
//  std::vector<double> vector{{1}, {1}, {1}};
//
//  std::vector<double> expect{{12}, {13}, {14}};
//
//  std::vector<double> res = multMatrixOnVector(matrix_A, vector);
//  for (int i = 0; i < 3; i++) {
//    ASSERT_NEAR(res[i], expect[i], 0.001);
//  }
//}
// disabled
//
// TEST(getLMatrix_Operation, Test_On_Own_Matrix) {
//  std::vector<std::vector<double>> matrix_A = {
//      {10, 1, 1},
//      {2, 10, 1},
//      {2, 2, 10},
//  };
//
//  std::vector<std::vector<double>> expect = {
//      {10, 0, 0},
//      {2, 10, 0},
//      {2, 2, 10},
//  };
//
//  std::vector<std::vector<double>> res = getLMatrix(matrix_A);
//  for (int i = 0; i < 3; i++) {
//    for (int j = 0; j < 3; j++) {
//      ASSERT_NEAR(res[i][j], expect[i][j], 0.001);
//    }
//  }
//}
// disabled
//
// TEST(getUMatrix_Operation, Test_On_Own_Matrix) {
//  std::vector<std::vector<double>> matrix_A = {
//      {10, 1, 1},
//      {2, 10, 1},
//      {2, 2, 10},
//  };
//
//  std::vector<std::vector<double>> expect = {
//      {0, 1, 1},
//      {0, 0, 1},
//      {0, 0, 0},
//  };
//
//  std::vector<std::vector<double>> res = getUMatrix(matrix_A);
//  for (int i = 0; i < 3; i++) {
//    for (int j = 0; j < 3; j++) {
//      ASSERT_NEAR(res[i][j], expect[i][j], 0.001);
//    }
//  }
//}
// disabled
//
// TEST(matrixToVector_Operation, Test_On_Own_Matrix) {
//  std::vector<std::vector<double>> matrix_A = {
//      {10, 1, 1},
//      {2, 10, 1},
//      {2, 2, 10},
//  };
//
//  std::vector<double> expect = {10, 1, 1, 2, 10, 1, 2, 2, 10};
//
//  std::vector<double> res = matrixToVector(matrix_A);
//  for (int i = 0; i < 9; i++) {
//    ASSERT_NEAR(res[i], expect[i], 0.001);
//  }
//}
// disabled
//
// TEST(vectorToMatrix_Operation, Test_On_Own_Matrix) {
//  std::vector<double> A = {10, 1, 1, 2, 10, 1, 2, 2, 10};
//
//  std::vector<std::vector<double>> expect = {
//      {10, 1, 1},
//      {2, 10, 1},
//      {2, 2, 10},
//  };
//
//  std::vector<std::vector<double>> res = vectorToMatrix(A, 3);
//  for (int i = 0; i < 3; i++) {
//    for (int j = 0; j < 3; j++) {
//      ASSERT_NEAR(res[i][j], expect[i][j], 0.001);
//    }
//  }
//}

TEST(Parallel_Operations_MPI, Test_On_Own_Data) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<std::vector<double>> matrix_A;
  std::vector<double> b;
  std::vector<double> res;
  const double eps = 0.0001;

  if (rank == 0) {
    matrix_A = {
        {10, 1, 1},
        {2, 10, 1},
        {2, 2, 10},
    };
    b = {12, 13, 14};
  }

  if (size > 1 && size <= 3) {
    res = getParallelOperations(matrix_A, b, eps);
  }

  if (rank == 0) {
    std::vector<double> check_res = getSequentialOperations(matrix_A, b, eps);

    if (size == 1 || size > 3) {
      res = check_res;
    }

    for (int i = 0; i < 3; i++) {
      ASSERT_NEAR(res[i], check_res[i], 0.001);
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Seidel_10) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int count_size_vector = 10;
  const double eps = 0.0001;
  std::vector<std::vector<double>> matrix_A;
  std::vector<double> b;
  std::vector<double> res;
  if (rank == 0) {
    matrix_A = getRandomMatrix(count_size_vector);
    b = getRandomVector(count_size_vector);
  }
  if (size > 1 && size <= count_size_vector) {
    res = getParallelOperations(matrix_A, b, eps);
  }

  if (rank == 0) {
    std::vector<double> check_res = getSequentialOperations(matrix_A, b, eps);

    if (size == 1 || size > count_size_vector) {
      res = check_res;
    }

    for (int i = 0; i < count_size_vector; i++) {
      ASSERT_NEAR(res[i], check_res[i], 0.001);
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Seidel_20) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int count_size_vector = 20;
  const double eps = 0.0001;
  std::vector<std::vector<double>> matrix_A;
  std::vector<double> b;
  std::vector<double> res;
  if (rank == 0) {
    matrix_A = getRandomMatrix(count_size_vector);
    b = getRandomVector(count_size_vector);
  }
  if (size > 1 && size <= count_size_vector) {
    res = getParallelOperations(matrix_A, b, eps);
  }

  if (rank == 0) {
    std::vector<double> check_res = getSequentialOperations(matrix_A, b, eps);

    if (size == 1 || size > count_size_vector) {
      res = check_res;
    }

    for (int i = 0; i < count_size_vector; i++) {
      ASSERT_NEAR(res[i], check_res[i], 0.001);
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Seidel_50) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int count_size_vector = 50;
  const double eps = 0.0001;
  std::vector<std::vector<double>> matrix_A;
  std::vector<double> b;
  std::vector<double> res;
  if (rank == 0) {
    matrix_A = getRandomMatrix(count_size_vector);
    b = getRandomVector(count_size_vector);
  }
  if (size > 1 && size <= count_size_vector) {
    res = getParallelOperations(matrix_A, b, eps);
  }

  if (rank == 0) {
    std::vector<double> check_res = getSequentialOperations(matrix_A, b, eps);

    if (size == 1 || size > count_size_vector) {
      res = check_res;
    }

    for (int i = 0; i < count_size_vector; i++) {
      ASSERT_NEAR(res[i], check_res[i], 0.001);
    }
  }
}

TEST(Parallel_Operations_MPI, Test_Seidel_100) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int count_size_vector = 100;
  const double eps = 0.0001;
  std::vector<std::vector<double>> matrix_A;
  std::vector<double> b;
  std::vector<double> res;
  if (rank == 0) {
    matrix_A = getRandomMatrix(count_size_vector);
    b = getRandomVector(count_size_vector);
  }
  if (size > 1 && size <= count_size_vector) {
    res = getParallelOperations(matrix_A, b, eps);
  }

  if (rank == 0) {
    std::vector<double> check_res = getSequentialOperations(matrix_A, b, eps);

    if (size == 1 || size > count_size_vector) {
      res = check_res;
    }

    for (int i = 0; i < count_size_vector; i++) {
      ASSERT_NEAR(res[i], check_res[i], 0.001);
    }
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
