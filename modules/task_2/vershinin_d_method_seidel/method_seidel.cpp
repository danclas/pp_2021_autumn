// Copyright 2021 Vershinin Daniil
#include "../../../modules/task_2/vershinin_d_method_seidel/method_seidel.h"

#include <mpi.h>

#include <random>
#include <vector>

std::vector<double> getRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<double> urd(0, 100);
  std::vector<double> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = urd(gen);
  }
  return vec;
}

std::vector<std::vector<double>> getRandomMatrix(int sz) {
  std::vector<std::vector<double>> matrix(sz);
  std::vector<double> tmp;
  for (int i = 0; i < sz; i++) {
    tmp = getRandomVector(sz);
    tmp[i] = tmp[i] * sz * 100;
    matrix[i] = tmp;
  }
  return matrix;
}

std::vector<double> getSequentialOperations(
    std::vector<std::vector<double>> matrix_A, std::vector<double> vector_b,
    double eps) {
  int N = static_cast<int>(matrix_A.size());
  std::vector<double> res(N);
  std::vector<double> tmp(N);
  double part1, part2;
  res[0] = vector_b[0];
  if (checkDiagDominance(matrix_A)) {
    bool continueProcess = true;
    while (continueProcess) {
      tmp = res;
      for (int i = 0; i < N; i++) {
        part1 = 0;
        part2 = 0;
        for (int j = 0; j < i; j++) {
          part1 -= matrix_A[i][j] * res[j];
        }
        for (int j = i + 1; j < N; j++) {
          part2 -= matrix_A[i][j] * tmp[j];
        }
        res[i] = part1 + part2 + vector_b[i];
        res[i] /= matrix_A[i][i];
        if (std::abs(res[i] - tmp[i]) < eps) {
          continueProcess = false;
        }
      }
    }
  }
  return res;
}

bool isSquareness(std::vector<std::vector<double>> matrix) {
  int m = static_cast<int>(matrix.size());
  int n = static_cast<int>(matrix[0].size());
  if (m != n) {
    return false;
  } else {
    return true;
  }
}

bool checkDiagDominance(std::vector<std::vector<double>> matrix) {
  int m = static_cast<int>(matrix.size());
  int n = static_cast<int>(matrix[0].size());
  double sum;

  if (!isSquareness(matrix)) {
    return false;
  }

  for (int i = 0; i < n; i++) {
    sum = 0;
    for (int j = 0; j < m; j++) {
      if (j != i) {
        sum += std::abs(matrix[i][j]);
      }
    }
    if (std::abs(matrix[i][i]) < sum) {
      return false;
    }
  }
  return true;
}

std::vector<std::vector<double>> inversion(std::vector<std::vector<double>> A) {
  double temp;
  // std::vector<std::vector<double>> A(matrix);
  int N = static_cast<int>(A.size());
  std::vector<std::vector<double>> res(N);

  for (int i = 0; i < N; i++) {
    res[i] = std::vector<double>(N);
    res[i][i] = 1;
  }

  for (int k = 0; k < N; k++) {
    temp = A[k][k];

    for (int j = 0; j < N; j++) {
      A[k][j] /= temp;
      res[k][j] /= temp;
    }

    for (int i = k + 1; i < N; i++) {
      temp = A[i][k];

      for (int j = 0; j < N; j++) {
        A[i][j] -= A[k][j] * temp;
        res[i][j] -= res[k][j] * temp;
      }
    }
  }

  for (int k = N - 1; k > 0; k--) {
    for (int i = k - 1; i >= 0; i--) {
      temp = A[i][k];

      for (int j = 0; j < N; j++) {
        A[i][j] -= A[k][j] * temp;
        res[i][j] -= res[k][j] * temp;
      }
    }
  }

  return res;
}

std::vector<std::vector<double>> multNumberOnMatrix(
    double num, std::vector<std::vector<double>> A) {
  int N = static_cast<int>(A.size());
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i][j] *= num;
    }
  }

  return A;
}

std::vector<std::vector<double>> multMatrixOnMatrix(
    std::vector<std::vector<double>> A, std::vector<std::vector<double>> B) {
  int m_A = static_cast<int>(A.size());
  int n_A = static_cast<int>(A[0].size());

  int m_B = static_cast<int>(B.size());
  int n_B = static_cast<int>(B[0].size());

  std::vector<std::vector<double>> res(m_A);

  if (n_A == m_B) {
    for (int i = 0; i < m_A; i++) {
      res[i] = std::vector<double>(n_B);
      for (int j = 0; j < n_B; j++) {
        for (int k = 0; k < n_A; k++) {
          res[i][j] += A[i][k] * B[k][j];
        }
      }
    }
  }

  return res;
}

std::vector<double> multMatrixOnVector(std::vector<std::vector<double>> A,
                                       std::vector<double> B) {
  int m_A = static_cast<int>(A.size());
  int n_A = static_cast<int>(A[0].size());

  int m_B = static_cast<int>(B.size());

  std::vector<double> res(m_A);

  if (n_A == m_B) {
    for (int i = 0; i < m_A; i++) {
      for (int j = 0; j < m_B; j++) {
        res[i] += A[i][j] * B[j];
      }
    }
  }

  return res;
}

std::vector<std::vector<double>> getLMatrix(
    std::vector<std::vector<double>> A) {
  int m = static_cast<int>(A.size());
  int n = static_cast<int>(A[0].size());

  std::vector<std::vector<double>> res(m);
  for (int i = 0; i < m; i++) {
    res[i] = std::vector<double>(n);
    for (int j = 0; j < i + 1; j++) {
      res[i][j] = A[i][j];
    }
  }

  return res;
}

std::vector<std::vector<double>> getUMatrix(
    std::vector<std::vector<double>> A) {
  int m = static_cast<int>(A.size());
  int n = static_cast<int>(A[0].size());

  std::vector<std::vector<double>> res(m);
  for (int i = 0; i < m; i++) {
    res[i] = std::vector<double>(n);
    for (int j = i + 1; j < n; j++) {
      res[i][j] = A[i][j];
    }
  }

  return res;
}

std::vector<double> getParallelOperations(
    std::vector<std::vector<double>> matrix_A, std::vector<double> vector_b,
    double eps) {
  int size, rank, line_count;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> sendcounts_line;
  std::vector<int> displs_line;
  std::vector<int> sendcounts;
  std::vector<int> displs;

  std::vector<std::vector<double>> C1;
  std::vector<double> C2;
  std::vector<double> C1_line;
  int vector_b_size = static_cast<int>(vector_b.size());
  int matrix_column_size = static_cast<int>(matrix_A.size());

  MPI_Bcast(&matrix_column_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  std::vector<double> res(matrix_column_size);
  bool allContinueProcess;

  if (rank == 0) {
    res[0] = vector_b[0];
    if (checkDiagDominance(matrix_A)) {
      allContinueProcess = true;
      std::vector<std::vector<double>> matrix_L = getLMatrix(matrix_A);
      std::vector<std::vector<double>> matrix_U = getUMatrix(matrix_A);
      matrix_L = inversion(matrix_L);
      C1 = multMatrixOnMatrix(matrix_L, matrix_U);
      C1 = multNumberOnMatrix(-1, C1);
      C2 = multMatrixOnVector(matrix_L, vector_b);

      C1_line = matrixToVector(C1);
      int count = vector_b_size / size;
      int delta = vector_b_size % size;
      int displs_delta_line = 0;
      int displs_delta = 0;
      for (int i = 0; i < size; i++) {
        if (i < delta) {
          sendcounts_line.push_back((count + 1) * matrix_column_size);
          displs_line.push_back(displs_delta_line);
          displs_delta_line += (count + 1) * matrix_column_size;

          sendcounts.push_back(count + 1);
          displs.push_back(displs_delta);
          displs_delta += count + 1;
        } else {
          sendcounts_line.push_back(count * matrix_column_size);
          displs_line.push_back(displs_delta_line);
          displs_delta_line += count * matrix_column_size;

          sendcounts.push_back(count);
          displs.push_back(displs_delta);
          displs_delta += count;
        }
      }
    } else {
      allContinueProcess = false;
    }
  }
  MPI_Bcast(&allContinueProcess, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);

  if (allContinueProcess) {
    MPI_Scatter(sendcounts.data(), 1, MPI_INT, &line_count, 1, MPI_INT, 0,
                MPI_COMM_WORLD);

    std::vector<double> local_vector_b(line_count),
        local_matrix_C1_line(line_count * matrix_column_size),
        local_vector_C2(line_count);
    std::pair<std::vector<double>, bool> new_local_res;

    MPI_Scatterv(C1_line.data(), sendcounts_line.data(), displs_line.data(),
                 MPI_DOUBLE, local_matrix_C1_line.data(),
                 line_count * matrix_column_size, MPI_DOUBLE, 0,
                 MPI_COMM_WORLD);

    std::vector<std::vector<double>> local_matrix_C1 =
        vectorToMatrix(local_matrix_C1_line, line_count);

    MPI_Scatterv(C2.data(), sendcounts.data(), displs.data(), MPI_DOUBLE,
                 local_vector_C2.data(), line_count, MPI_DOUBLE, 0,
                 MPI_COMM_WORLD);

    while (allContinueProcess) {
      MPI_Bcast(res.data(), matrix_column_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      new_local_res = getLocalRes(local_matrix_C1, local_vector_C2, res, eps);

      MPI_Gatherv(new_local_res.first.data(), line_count, MPI_DOUBLE,
                  res.data(), sendcounts.data(), displs.data(), MPI_DOUBLE, 0,
                  MPI_COMM_WORLD);

      MPI_Allreduce(&(new_local_res.second), &allContinueProcess, 1, MPI_C_BOOL,
                    MPI_LAND, MPI_COMM_WORLD);
    }
  }
  return res;
}

std::pair<std::vector<double>, bool> getLocalRes(
    std::vector<std::vector<double>> C1, std::vector<double> C2,
    std::vector<double> res_prev, double eps) {
  bool continueProcess = true;
  int N = static_cast<int>(C1.size());

  std::vector<double> res = multMatrixOnVector(C1, res_prev);
  for (int i = 0; i < N; i++) {
    res[i] += C2[i];
    if (std::abs(res[i] - res_prev[i]) < eps) {
      continueProcess = false;
    }
  }

  return std::make_pair(res, continueProcess);
}

std::vector<double> matrixToVector(std::vector<std::vector<double>> A) {
  int m = static_cast<int>(A.size());
  int n = static_cast<int>(A[0].size());
  std::vector<double> res;

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      res.push_back(A[i][j]);
    }
  }
  return res;
}

std::vector<std::vector<double>> vectorToMatrix(std::vector<double> v,
                                                int column) {
  std::vector<std::vector<double>> res(column);
  int line = static_cast<int>(v.size()) / column;
  for (int i = 0; i < column; i++) {
    for (int j = i * line; j < (i + 1) * line; j++) {
      res[i].push_back(v[j]);
    }
  }
  return res;
}
