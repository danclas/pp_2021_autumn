// Copyright 2021 Myasnikova Varvara

#ifndef MODULES_TASK_3_MYASNIKOVA_SPARSE_MATRIX_MULTIPLICATION_MYASNIKOVA_SPARSE_MATRIX_MULTIPLICATION_H_
#define MODULES_TASK_3_MYASNIKOVA_SPARSE_MATRIX_MULTIPLICATION_MYASNIKOVA_SPARSE_MATRIX_MULTIPLICATION_H_

#include <vector>

struct SparseMatrix {
  // Non-zero coefficients of the matrix
  std::vector<double> val;
  // Pointer to the index of the non-zero coefficient of the array val
  std::vector<int> col_ptr;
  // Lowercase indexes for each column of the matrix
  std::vector<int> row_index;
  int columns = 0, rows = 0, non_zero = 0;

  friend const std::vector<double> operator*(const SparseMatrix& A,
    const SparseMatrix& B);
};

SparseMatrix CCS(const std::vector<double>& new_matrix,
  const int new_columns, const int new_rows);

std::vector<double> Multiply(SparseMatrix A, SparseMatrix B);
std::vector<double> RandMatrix(const int columns, const int rows);

#endif  // MODULES_TASK_3_MYASNIKOVA_SPARSE_MATRIX_MULTIPLICATION_MYASNIKOVA_SPARSE_MATRIX_MULTIPLICATION_H_
