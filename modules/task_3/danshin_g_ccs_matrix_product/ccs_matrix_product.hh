// Copyright 2021 Gleb "belgad" Danshin
#ifndef MODULES_TASK_3_DANSHIN_G_CCS_MATRIX_PRODUCT_CCS_MATRIX_PRODUCT_HH_
#define MODULES_TASK_3_DANSHIN_G_CCS_MATRIX_PRODUCT_CCS_MATRIX_PRODUCT_HH_

#include <vector>

struct CCSMatrix {
  int row_count;
  int column_count;
  std::vector<double> values;
  std::vector<int> rows;
  std::vector<int> pointers;

  CCSMatrix();
  CCSMatrix(const int& row_count, const int& column_count, const std::vector<double>& values, const std::vector<int>& rows, const std::vector<int>& pointers);

  bool operator==(const CCSMatrix &other);
};

struct CRSMatrix {
  int row_count;
  int column_count;
  std::vector<double> values;
  std::vector<int> columns;
  std::vector<int> pointers;

  CRSMatrix(const int& row_count, const int& column_count, const std::vector<double>& values, const std::vector<int>& columns, const std::vector<int>& pointers);
  CRSMatrix(const CCSMatrix& matrix);
  CRSMatrix(const int& row_count, const int& column_count, const int& length);
};

CCSMatrix* GenerateRandomMatrix(double g, int row_count, int column_count);
CCSMatrix* MatrixProduct(CCSMatrix* matrix1, CCSMatrix* matrix2);
CCSMatrix* MatrixProduct(CRSMatrix* matrix1, CCSMatrix* matrix2);
CCSMatrix* MatrixProductParallel(CCSMatrix* matrix1, CCSMatrix* matrix2);

#endif  // MODULES_TASK_3_DANSHIN_G_CCS_MATRIX_PRODUCT_CCS_MATRIX_PRODUCT_HH_
