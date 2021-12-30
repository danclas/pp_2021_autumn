// Copyright 2021 Chelebaev Artem
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include "../chelebaev_strassen_algorithm/strassen_algorithm.h"

Matrix createRandomMatrix(int n, int m, double max_number) {
    std::random_device device;
    std::mt19937 gen(device());
    Matrix matrix(n, std::vector<double>(m));
    std::uniform_real_distribution<> dis(-max_number, max_number);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = (dis(gen));
        }
    }
    return matrix;
}


Matrix sequentialMulti(const Matrix& a, const Matrix& b) {
    Matrix res(a.size(), std::vector<double>(b[0].size()));
    for (std::size_t i = 0; i < a.size(); i++) {
        for (std::size_t j = 0; j < b[0].size(); j++) {
            for (std::size_t k = 0; k < a[0].size(); k++) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return res;
}

Matrix parallelMulti(const Matrix& a, const Matrix& b) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int n = static_cast<int>(a.size());
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (n <= 2 && my_rank == 0) {
        return sequentialMulti(a, b);
    }
    if (n <= 2) {
        return Matrix();
    }
    Matrix a11, a12, a21, a22;
    Matrix b11, b12, b21, b22;
    if (my_rank == 0) {
        n = n / 2;
        a11 = Matrix(n, std::vector<double>(n));
        a12 = Matrix(n, std::vector<double>(n));
        a21 = Matrix(n, std::vector<double>(n));
        a22 = Matrix(n, std::vector<double>(n));

        b11 = Matrix(n, std::vector<double>(n));
        b12 = Matrix(n, std::vector<double>(n));
        b21 = Matrix(n, std::vector<double>(n));
        b22 = Matrix(n, std::vector<double>(n));
    }

    if (my_rank == 0) {
        std::vector<Matrix> all;
        all = divide(a);
        a11 = std::move(all[0]), a12 = std::move(all[1]), a21 = std::move(all[2]), a22 = std::move(all[3]);

        all = divide(b);
        b11 = std::move(all[0]), b12 = std::move(all[1]), b21 = std::move(all[2]), b22 = std::move(all[3]);
    }
    Matrix p1, p2, p3, p4, p5, p6, p7;
    p1 = parallelMulti(msum(a11, a22), msum(b11, b22));
    p2 = parallelMulti(msum(a21, a22), b11);
    p3 = parallelMulti(a11, msub(b12, b22));
    p4 = parallelMulti(a22, msub(b21, b11));
    p5 = parallelMulti(msum(a11, a12), b22);
    p6 = parallelMulti(msub(a21, a11), msum(b11, b12));
    p7 = parallelMulti(msub(a12, a22), msum(b21, b22));

    Matrix c11 = msum(msum(p1, p4), msub(p7, p5));
    Matrix c12 = msum(p3, p5);
    Matrix c21 = msum(p2, p4);
    Matrix c22 = msum(msub(p1, p2), msum(p3, p6));

    return connect(c11, c12, c21, c22);
}

std::vector<Matrix> divide(const Matrix& a) {
    int n = static_cast<int>(a.size() / 2);
    std::vector<Matrix> result(4, Matrix(n));
    for (int i = 0; i < n; i++) {
        result[0][i] = { a[i].begin(), a[i].begin() + n };
        result[1][i] = { a[i].begin() + n, a[i].begin() + 2 * n };
        result[2][i] = { a[i + n].begin(), a[i + n].begin() + n };
        result[3][i] = { a[i + n].begin() + n, a[i + n].begin() + 2 * n };
    }
    return result;
}

Matrix connect(const Matrix& a, const Matrix& b, const Matrix& c, const Matrix& d) {
    int n = static_cast<int>(a.size());
    Matrix result(n * 2, std::vector<double>(n * 2));
    for (int i = 0; i < n; i++) {
        result[i] = a[i];
        result[i].insert(result[i].end(), b[i].begin(), b[i].end());
        result[i + n] = c[i];
        result[i + n].insert(result[i + n].end(), d[i].begin(), d[i].end());
    }
    return result;
}

Matrix msum(const Matrix& a, const Matrix& b) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int n = 0;
    if (my_rank == 0) {
        n = static_cast<int>(a.size());
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    Matrix ca, cb, res;
    if (my_rank == 0) {
        int k = static_cast<int>(a.size() / tasks);
        int rem = static_cast<int>(a.size() % tasks);
        int ck = k;
        if (my_rank < rem)
            ck++;
        int j = ck;
        for (int i = 1; i < tasks; i++) {
            int ck = k;
            if (i < rem)
                ck++;
            MPI_Send(&ck, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            for (int c = 0; c < ck; c++, j++) {
                MPI_Send(a[j].data(), n, MPI_DOUBLE,
                    i, c + 1, MPI_COMM_WORLD);
                MPI_Send(b[j].data(), n, MPI_DOUBLE,
                    i, c + 1, MPI_COMM_WORLD);
            }
        }
        res.assign(ck, std::vector<double>(n));
        for (int i = 0; i < ck; i++) {
            for (int j = 0; j < n; j++) {
                res[i][j] = a[i][j] + b[i][j];
            }
        }
    } else {
        int h;
        MPI_Status* status = new MPI_Status();
        MPI_Recv(&h, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, status);
        ca.assign(h, std::vector<double>(n));
        cb.assign(h, std::vector<double>(n));
        for (int i = 0; i < h; i++) {
            MPI_Recv(ca[i].data(), n,
                MPI_DOUBLE, 0, i + 1, MPI_COMM_WORLD, status);
            MPI_Recv(cb[i].data(), n,
                MPI_DOUBLE, 0, i + 1, MPI_COMM_WORLD, status);
        }
        res.assign(h, std::vector<double>(n));
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < n; j++) {
                res[i][j] = ca[i][j] + cb[i][j];
            }
        }
        MPI_Send(&h, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        for (int i = 0; i < h; i++) {
            MPI_Send(res[i].data(), n, MPI_DOUBLE, 0, i + 1, MPI_COMM_WORLD);
        }
    }
    Matrix result;
    if (my_rank == 0) {
        result.assign(n, std::vector<double>(n));
        int j = 0;
        for (std::size_t i = 0; i < res.size(); i++, j++) {
            result[j] = std::move(res[i]);
        }
        for (int i = 1; i < tasks; i++) {
            int h;
            MPI_Status* status = new MPI_Status();
            MPI_Recv(&h, 1, MPI_INT, i, 0, MPI_COMM_WORLD, status);
            for (int c = 0; c < h; c++, j++) {
                MPI_Recv(result[j].data(), n, MPI_DOUBLE, i,
                    c + 1, MPI_COMM_WORLD, status);
            }
        }
    }

    return result;
}

Matrix msub(const Matrix& a, const Matrix& b) {
    int tasks, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int n = 0;
    if (my_rank == 0) {
        n = static_cast<int>(a.size());
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    Matrix ca, cb, res;
    if (my_rank == 0) {
        int k = static_cast<int>(a.size() / tasks);
        int rem = static_cast<int>(a.size() % tasks);
        int ck = k;
        if (my_rank < rem)
            ck++;
        int j = ck;
        for (int i = 1; i < tasks; i++) {
            int ck = k;
            if (i < rem)
                ck++;
            MPI_Send(&ck, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            for (int c = 0; c < ck; c++, j++) {
                MPI_Send(a[j].data(), n, MPI_DOUBLE,
                    i, c + 1, MPI_COMM_WORLD);
                MPI_Send(b[j].data(), n, MPI_DOUBLE,
                    i, c + 1, MPI_COMM_WORLD);
            }
        }
        res.assign(ck, std::vector<double>(n));
        for (int i = 0; i < ck; i++) {
            for (int j = 0; j < n; j++) {
                res[i][j] = a[i][j] - b[i][j];
            }
        }
    } else {
        int h;
        MPI_Status* status = new MPI_Status();
        MPI_Recv(&h, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, status);
        ca.assign(h, std::vector<double>(n));
        cb.assign(h, std::vector<double>(n));
        for (int i = 0; i < h; i++) {
            MPI_Recv(ca[i].data(), n,
                MPI_DOUBLE, 0, i + 1, MPI_COMM_WORLD, status);
            MPI_Recv(cb[i].data(), n,
                MPI_DOUBLE, 0, i + 1, MPI_COMM_WORLD, status);
        }
        res.assign(h, std::vector<double>(n));
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < n; j++) {
                res[i][j] = ca[i][j] - cb[i][j];
            }
        }
        MPI_Send(&h, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        for (int i = 0; i < h; i++) {
            MPI_Send(res[i].data(), n, MPI_DOUBLE, 0, i + 1, MPI_COMM_WORLD);
        }
    }
    Matrix result;
    if (my_rank == 0) {
        result.assign(n, std::vector<double>(n));
        int j = 0;
        for (std::size_t i = 0; i < res.size(); i++, j++) {
            result[j] = std::move(res[i]);
        }
        for (int i = 1; i < tasks; i++) {
            int h = 0;
            MPI_Status* status = new MPI_Status();
            MPI_Recv(&h, 1, MPI_INT, i, 0, MPI_COMM_WORLD, status);
            for (int c = 0; c < h; c++, j++) {
                MPI_Recv(result[j].data(), n, MPI_DOUBLE, i,
                    c + 1, MPI_COMM_WORLD, status);
            }
        }
    }

    return result;
}

bool areEqual(const Matrix& a, const Matrix& b, double delta) {
    if (a.size() != b.size()) {
        return false;
    }
    for (std::size_t i = 0; i < a.size(); i++) {
        if (a[i].size() != b[i].size()) {
            return false;
        }
        for (std::size_t j = 0; j < a[i].size(); j++) {
            if (std::abs(a[i][j] - b[i][j]) > delta) {
                return false;
            }
        }
    }
    return true;
}
