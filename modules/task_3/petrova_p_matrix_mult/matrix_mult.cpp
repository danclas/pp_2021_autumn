
//  Copyright 2021 Petrova Polina
#include <mpi.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <random>
#include <ctime>
#include <cstring>

#include"../../../modules/task_3/petrova_p_matrix_mult/matrix_mult.h"

void _daxpy(int N, double a, double* x, int incx, double* y, int incy) {
    double* pa = x;
    double* pb = y;
    for (int i = 0; i < N; i++, pa += incx, pb += incy) {
        pb[0] += a * pa[0];
    }
}
void _daxpby(int N, double a, double* x, int incx,
    double b, double* y, int incy) {
    double* pa = x;
    double* pb = y;
    for (int i = 0; i < N; i++, pa += incx, pb += incy) {
        pb[0] = a * pa[0] + b * pb[0];
    }
}

double _ddot(const int n, /*const*/ double* x, const int incx,
    /*const*/ double* y, const int incy) {
    double a = 0.0;
    double* px = x;
    // double* px = reinterpret_cast<double*>(x);
    double* py = y;
    // double* py = reinterpret_cast<double*>(y);
    for (int i = 0; i < n; i++, px += incx, py += incy) {
        a += px[0] * py[0];
    }
    return a;
}

void _dgemv(int n, int m, double alpha, double* pa, int lda,
    double* x, int incx, double beta, double* y, int incy) {
    double* py = y;
    for (int i = 0; i < n; i++, py += incy) {
        py[0] *= beta;
        double* px = x;
        for (int j = 0; j < m; j++, px += incx) {
            py[0] += alpha * pa[i + n * j] * px[0];
        }
    }
}

double _dnrm2(const int n, /*const*/ double* x, const int incx) {
    double a = 0.0;
    double* px = x;
    // double* px = reinterpret_cast<double*>(x);
    for (int i = 0; i < n; i++, px += incx) {
        a += px[0] * px[0];
    }
    return a;
}
void Fox_sequence(int N, int q, double* A, int lda, double* B, int ldb, double* C, int ldc) {
    std::div_t dv = std::div(N, q);
    int p = dv.quot + (dv.rem ? 1 : 0);   // Количество блоков в строке/столбце
    memset(C, 0, sizeof(double) * ldc * ldc);
    double* pa, * pb, * pc;
    for (int K = 0; K < p; K++) {
        for (int I = 0; I < p; I++) {
            for (int J = 0; J < p; J++) {
                int k = (I + J + K) % p;
                pa = A + k * q * lda + I * q;
                pb = B + J * q * ldb + k * q;
                pc = C + J * q * ldc + I * q;

                // C[i,j] += A[i,k]*B[k,j]
                for (int i = 0; i < q; i++) {
                    for (int j = 0; j < q; j++) {
                        for (int k = 0; k < q; k++) {
                            pc[j * ldc + i] += pa[k * lda + i] * pb[j * ldb + k];
                        }
                    }
                }
            }
        }
    }
}

void Fox_mpi(int N, double* A, int lda, double* B, int ldb, double* C, int ldc) {
    int size, rank, p;
    MPI_Status status;
    double* buf, * pa, * pb;

    const int tag = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (p = 1; p * p <= size; p++) {}
        p--;
        std::div_t dv = std::div(N, p);
        N = dv.quot + (dv.rem ? 1 : 0);
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank < p * p) {
        buf = new double[N * N];

        if (rank == 0) {
            for (int i = 0; i < p; i++, pa += N) {
                for (int j = 0; j < p; j++) {
                    if ((i == 0) && (j == 0)) continue;
                    int k = (i + j) % p;
                    pa = A + k * N * lda + i * N;
                    pb = buf;
                    for (int k = 0; k < N; k++, pa += lda, pb += N) {
                        memcpy(pb, pa, sizeof(double) * N);
                    }
                    MPI_Send(buf, N * N, MPI_DOUBLE, p * j + i,
                        tag, MPI_COMM_WORLD);

                    pa = B + j * N * ldb + k * N;
                    pb = buf;
                    for (int k = 0; k < N; k++, pa += ldb, pb += N) {
                        memcpy(pb, pa, sizeof(double) * N);
                    }
                    MPI_Send(buf, N * N, MPI_DOUBLE, p * j + i,
                        tag, MPI_COMM_WORLD);
                }
            }
        } else {
            lda = N;
            ldb = N;
            ldc = N;
            A = new double[N * N];
            B = new double[N * N];
            C = new double[N * N];
            MPI_Recv(A, N * N, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
            MPI_Recv(B, N * N, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        }
        //  Узлы коммуникации
        int Top = (rank / p) == ((rank - 1) / p) && (rank - 1) >= 0 ? rank - 1 : p * ((rank / p) + 1) - 1;
        int Bottom = (rank / p) == ((rank + 1) / p) ? rank + 1 : p * (rank / p);
        int Left = (rank - p) >= 0 ? rank - p : p * (p - 1) + (rank % p);
        int Right = (rank + p) < p * p ? rank + p : (rank % p);

        memset(C, 0, sizeof(double) * ldc * ldc);

        // std::cout << N << " " << p << std::endl;
        for (int q = 0; q < p; q++) {
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    for (int k = 0; k < N; k++) {
                        C[j * ldc + i] += A[k * lda + i] * B[j * ldb + k];
                    }
                }
            }

            if (p > 1) {
                if (rank == 0) {
                    pa = A;
                    pb = buf;
                    for (int i = 0; i < N; i++, pa += lda, pb += N) {
                        memcpy(pb, pa, sizeof(double) * N);
                    }
                    MPI_Sendrecv_replace(
                        buf, N * N, MPI_DOUBLE, Left, tag, Right, tag,
                        MPI_COMM_WORLD, &status);
                } else {
                    MPI_Sendrecv(
                        A, N * N, MPI_DOUBLE, Left, tag,
                        buf, N * N, MPI_DOUBLE, Right, tag,
                        MPI_COMM_WORLD, &status);
                }
                pa = A;
                pb = buf;
                for (int i = 0; i < N; i++, pa += lda, pb += N) {
                    memcpy(pa, pb, sizeof(double) * N);
                }

                if (rank == 0) {
                    pa = B;
                    pb = buf;
                    for (int i = 0; i < N; i++, pa += ldb, pb += N) {
                        memcpy(pb, pa, sizeof(double) * N);
                    }
                    MPI_Sendrecv_replace(
                        buf, N * N, MPI_DOUBLE, Top, tag, Bottom, tag,
                        MPI_COMM_WORLD, &status);
                } else {
                    MPI_Sendrecv(
                        B, N * N, MPI_DOUBLE, Top, tag,
                        buf, N * N, MPI_DOUBLE, Bottom, tag,
                        MPI_COMM_WORLD, &status);
                }

                pa = B;
                pb = buf;
                for (int i = 0; i < N; i++, pa += ldb, pb += N) {
                    memcpy(pa, pb, sizeof(double) * N);
                }
            }
        }

        if (rank == 0) {
            pa = C;
            double* pbuf;
            for (int I = 0; I < p; I++, pa += N) {
                pb = pa;
                for (int J = 0; J < p; J++) {
                    if ((I == 0) && (J == 0)) {
                        pb += ldc * N;
                        continue;
                    }
                    MPI_Recv(buf, N * N, MPI_DOUBLE, J * p + I, tag, MPI_COMM_WORLD, &status);
                    pbuf = buf;
                    for (int i = 0; i < N; i++, pb += ldc, pbuf += N) {
                        memcpy(pb, pbuf, sizeof(double) * N);
                    }
                }
            }
        } else {
            MPI_Send(C, N * N, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
            delete[] A;
            delete[] B;
            delete[] C;
        }
        delete[] buf;
    }
}





