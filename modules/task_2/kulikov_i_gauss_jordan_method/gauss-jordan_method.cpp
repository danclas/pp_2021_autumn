// Copyright 2021 Kulikov Ilya
#include <mpi.h>
#include <random>
#include <iostream>
#include "../../../modules/task_2/kulikov_i_gauss_jordan_method/gauss-jordan_method.h"


double* get_random_matrix(int* num_vec, int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());

    double* out_matrix = new double[sz * (sz + 1)];
    for (int i = 0; i < sz; i++) {
        double sum = 0;
        for (int j = 0; j < sz; j ++) {
            out_matrix[i * (sz + 1) + j] = gen() % 100 + 1;
            sum += out_matrix[i * (sz + 1) + j] * num_vec[j];
        }
        out_matrix[(i + 1) * (sz + 1) - 1] = sum;
    }
    return out_matrix;
}


void division_in_str(double* strings, double* subtrahend,
int num, int sz, int part) {
    for (int i = 0; i < part; i++) {
        double coeff = strings[i * (sz + 1) + num] / subtrahend[num];
        for (int j = 0; j < sz + 1; j ++) {
            strings[i * (sz + 1) + j] -= coeff * subtrahend[j];
        }
    }
}


double* gauss_jordan_finding(double* matrix, int sz) {
    int root = 0, rank = 0, commSize = 0;

    // sz -= 1;

    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int part_size = 0;
    int num_parts = 0;
    int* matrix_sendcounts = nullptr;
    int* matrix_displs = nullptr;

    if (rank == root) {
        part_size = sz / commSize;
        num_parts = sz % part_size;

        matrix_sendcounts = new int[commSize];
        matrix_displs = new int[commSize];

        matrix_displs[0] = 0;

        int q;
        if (0 != num_parts) q = part_size + 1;
        else
        q = part_size;
        matrix_sendcounts[0] = q * (sz + 1);

        for (int i = 1; i < commSize; i++) {
            if (i < num_parts) q = part_size + 1;
            else
            q = part_size;

            matrix_sendcounts[i] = q * (sz + 1);

            matrix_displs[i] = matrix_displs[i - 1] + matrix_sendcounts[i - 1];
        }
    }

    MPI_Bcast(&part_size, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&num_parts, 1, MPI_INT, root, MPI_COMM_WORLD);

    int local_part_size;

    if (rank < num_parts) {
        local_part_size = part_size + 1;
    } else {
        local_part_size = part_size;
    }

    double* buffer = new double[local_part_size * (sz + 1)];
    double* subtrahend = new double[sz + 1];

    for (int i = 0; i < sz; i++) {
        if (rank == root) {
            std::copy(matrix + (i * (sz + 1)), matrix + (i + 1) * (sz + 1),
            subtrahend);
        }

        MPI_Bcast(subtrahend, sz + 1, MPI_DOUBLE, root, MPI_COMM_WORLD);

        MPI_Scatterv(matrix, matrix_sendcounts, matrix_displs, MPI_DOUBLE,
        buffer, local_part_size * (sz + 1), MPI_DOUBLE,
        root, MPI_COMM_WORLD);

        division_in_str(buffer, subtrahend, i, sz, local_part_size);

        MPI_Gatherv(buffer, local_part_size * (sz + 1), MPI_DOUBLE, matrix,
        matrix_sendcounts, matrix_displs, MPI_DOUBLE,
        root, MPI_COMM_WORLD);

        if (rank == root) {
            std::copy(subtrahend, subtrahend + sz + 1, matrix + (i * (sz + 1)));
        }
    }

    double* res = new double[sz];

    if (rank == root) {
        for (int i = 0; i < sz; i++) {
            if (matrix[(i + 1) * (sz + 1) - 1] == 0) {
                res[i] = 0;
            } else {
                res[i] = matrix[(i + 1) * (sz + 1) - 1]
                / matrix[i * (sz + 1) + i];
            }
        }
    }

    delete [] matrix_sendcounts;
    delete [] matrix_displs;
    delete [] buffer;
    delete [] subtrahend;

    return res;
}
