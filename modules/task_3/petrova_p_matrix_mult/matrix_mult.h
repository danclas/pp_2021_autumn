// Copyright 2021 Petrova Polina

#ifndef MODULES_TASK_3_PETROVA_P_MATRIX_MULT_MATRIX_MULT_H_
#define MODULES_TASK_3_PETROVA_P_MATRIX_MULT_MATRIX_MULT_H_

#include <iostream>
#include <vector>

std::vector<double> fillRandomVector(int len);
std::vector< std::vector<double>> fillRandomMatrix(int n);

void _daxpy(int N, double a, double* x, int incx, double* y, int incy);
void _daxpby(int N, double a, double* x, int incx,
    double b, double* y, int incy);
double _ddot(const int n, /*const*/ double* x, const int incx,
    /*const*/ double* y, const int incy);
void _dgemv(int n, int m, double alpha, double* pa, int lda, double* x,
    int incx, double beta, double* y, int incy);
double _dnrm2(const int n, /*const*/ double* x, const int incx);

void Fox_sequence(int N, int q, double* A, int lda, double* B,
    int ldb, double* C, int ldc);
void Fox_mpi(int N, double* A, int lda, double* B, int ldb, double* C, int ldc);

#endif  // MODULES_TASK_3_PETROVA_P_MATRIX_MULT_MATRIX_MULT_H_

