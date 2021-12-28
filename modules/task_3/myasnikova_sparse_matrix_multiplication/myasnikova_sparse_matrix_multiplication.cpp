// Copyright 2021 Myasnikova Varvara

#include "../../../modules/task_3/myasnikova_sparse_matrix_multiplication/myasnikova_sparse_matrix_multiplication.h"
#include <mpi.h>
#include <ctime>
#include <random>
#include <vector>

// Converting a matrix to columnar storage
SparseMatrix CCS(const std::vector<double>& new_matrix, const int new_columns,
  const int new_rows) {
  SparseMatrix result;
  result.columns = new_columns;
  result.rows = new_rows;
  result.non_zero = 0;

  result.col_ptr.push_back(0);
  for (int column = 0; column < new_columns; column++) {
    int nnz_count = 0;
    for (int i = column; i <= (new_rows - 1) * new_columns + column;
      i += new_columns) {
      if (new_matrix[i] != 0) {
        nnz_count++;
        result.val.push_back(new_matrix[i]);
        result.row_index.push_back((i - column) / new_columns);
      }
    }
    result.col_ptr.push_back(result.col_ptr.back() + nnz_count);
    result.non_zero += nnz_count;
  }

  return result;
}

// Multiplication operator
const std::vector<double> operator*(const SparseMatrix& A,
  const SparseMatrix& B) {
  if (A.columns != B.rows) {
    throw "incorrect size";
  }

  std::vector<double> result(A.rows * B.columns, 0);

  for (int column = 0; column < A.columns; column++) {
    for (int b_column = 0; b_column < B.columns; b_column++) {
      for (int i = A.col_ptr[column]; i <= A.col_ptr[column + 1] - 1; i++) {
        if (B.col_ptr[b_column + 1] - B.col_ptr[b_column] == 0) {
          continue;
        }

        for (int j = B.col_ptr[b_column];
          j <= B.col_ptr[b_column + 1] - 1; j++) {
          if (B.row_index[j] == column) {
            // [interpreting a two-dimensional array into a one-dimensional one]
            result[A.row_index[i] * B.columns + b_column]
              += A.val[i] * B.val[j];
          }
        }
      }
    }
  }

  return result;
}

// Parallel multiplication
std::vector<double> Multiply(SparseMatrix A, SparseMatrix B) {
  int ProcRank, ProcNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

  if (ProcNum == 1) {
    return A * B;
  }

  // Data is sent to all processes
  MPI_Bcast(&A.columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.non_zero, 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Bcast(&B.columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.non_zero, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (A.non_zero == 0 || B.non_zero == 0) {
    // The main process does the multiplication sequentially
    if (ProcRank == 0) {
      return A * B;
    } else {
      // the rest of the processes get an empty vector to get something,
      // in order to avoid an error
      return std::vector<double>();
    }
  }

  if (A.columns != B.rows) {
    throw "incorrect size";
  }
  // if the number of columns is less than the number of processes
  if (A.columns < ProcNum) {
    if (ProcRank == 0) {
      return A * B;  // The main process does the multiplication sequentially
    } else {
      // the rest of the processes get an empty vector to get something,
      // in order to avoid an error
      return std::vector<double>();
    }
  }

  if (ProcRank != 0) {
    A.val.resize(A.non_zero);  // Changing the size
    A.row_index.resize(A.non_zero);
    A.col_ptr.resize(A.columns + 1);
  }
  // send data about matrix A to all processes
  MPI_Bcast(&A.val[0], A.non_zero, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.row_index[0], A.non_zero, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.col_ptr[0], A.columns + 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (ProcRank != 0) {
    B.val.resize(B.non_zero);
    B.row_index.resize(B.non_zero);
    B.col_ptr.resize(B.columns + 1);
  }
  // send data about matrix B to all processes
  MPI_Bcast(&B.val[0], B.non_zero, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.row_index[0], B.non_zero, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.col_ptr[0], B.columns + 1, MPI_INT, 0, MPI_COMM_WORLD);

  // distribute to each process its own piece of the matrix A
  int delta = A.columns / ProcNum;
  // Denoting the boundaries
  int left_bound = ProcRank * delta;
  int right_bound = (ProcRank + 1) * delta;

  if (ProcRank == ProcNum - 1) {
    right_bound = A.columns;
  }

  // Declaring a vector to store the local result
  // (each process has its own result)
  std::vector<double> local_result(A.rows * B.columns);

  for (int column = left_bound; column < right_bound; column++) {
    for (int b_column = 0; b_column < B.columns; b_column++) {
      for (int i = A.col_ptr[column]; i <= A.col_ptr[column + 1] - 1; i++) {
        if (B.col_ptr[b_column + 1] - B.col_ptr[b_column] == 0) {
          continue;
        }

        for (int j = B.col_ptr[b_column];
          j <= B.col_ptr[b_column + 1] - 1; j++) {
          if (B.row_index[j] == column) {
            local_result[A.row_index[i] * B.columns + b_column]
              += A.val[i] * B.val[j];
          }
        }
      }
    }
  }

  // Declaring a vector to collect local results
  std::vector<double> global_result;
  if (ProcRank == 0) {
    global_result.resize(A.rows * B.columns);
  }

  if (ProcRank == 0) {
    MPI_Reduce(&local_result[0], &global_result[0], A.rows * B.columns,
      MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  } else {
    MPI_Reduce(&local_result[0], MPI_IN_PLACE, A.rows * B.columns,
      MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  }

  return global_result;
}

std::vector<double> RandMatrix(const int columns, const int rows) {
  if (rows <= 0 || columns <= 0) {
    throw "incorrect size";
  }

  std::srand(std::time(nullptr));
  std::vector<double> result(columns * rows);

  for (int i = 0; i < rows * columns; i++) {
    double rand_val = static_cast<double>(std::rand() % 50 + 1);
    if (rand_val < 4) {
      result[i] = rand_val;
    } else {
      result[i] = 0;
    }
  }
  return result;
}
