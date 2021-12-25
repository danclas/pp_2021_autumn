// Copyright 2021 Kulikov Ilya
#include <mpi.h>
#include <vector>
#include <random>
#include "../../../modules/task_2/kulikov_i_gauss-jordan_method/gauss-jordan_method.h"


int* get_random_matrix(int sz) {
    int* out_matrix = new int[sz * (sz + 1)];
    for (int i = 0; i < sz; i++) { for (int j = 0; j < sz + 1; j ++) { out_matrix[i * (sz + 1) + j] = rand() % 100000 - 50000; }}
    return out_matrix;
}


double* division_in_str(double* strings, double* subtrahend, double* coeffs, int sz) {
    int part = sizeof(coeffs);

    for (int i = 0; i < part; i++) {
        for (int j = 0; j < sz + 1; j ++) {
            strings[i * (sz + 1) + j] -= coeffs[i] * subtrahend[j];
        }
    }
    return strings;
}


double* gauss_jordan_finding(double* matrix, int sz) {
    int root = 0, rank = 0, commSize = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);

    int part_size = sz / commSize; 
    int num_parts = sz % part_size;

    int* matrix_sendcounts = new int[commSize];
    int* coeffs_sendcounts = new int[commSize];

    int* matrix_displs = new int[commSize];
    int* coeffs_displs = new int[commSize];

    matrix_displs[0] = 0;
    coeffs_displs[0] = 0;

    for (int i = 0; i < commSize; i ++) {
        matrix_displs[i] = matrix_displs[i - 1] + matrix_sendcounts[i - 1];
        coeffs_displs[i] = coeffs_displs[i - 1] + coeffs_sendcounts[i - 1];

        int q;

        if (i < num_parts) q = part_size + 1;
        else q = part_size;

        matrix_sendcounts[i] = q * (sz + 1);
        coeffs_sendcounts[i] = q;
    }

    double* buffer = new double[part_size * (sz + 1)];
    double* coeff_buffer = new double[part_size];

    for (int i = 0; i < sz; i++) {
        double* coeffs = new double[sz];
        double k = matrix[i * (sz + 1) + i];

        for (int j = 0; j < sz; j ++) {
            if (j != i) {
                coeffs[j] = matrix[j * (sz + 1) + i] / k;
            } else {
                coeffs[j] = 0;
            }
        }

        double* subtrahend = new double[sz + 1];
        std::copy(matrix + (i * (sz + 1)), matrix + (i + 1) * (sz + 1), subtrahend);

        MPI_Scatterv(matrix, matrix_sendcounts, matrix_displs, MPI_DOUBLE, buffer, (part_size + 1) * (sz + 1), MPI_DOUBLE, root, MPI_COMM_WORLD);

        MPI_Scatterv(coeffs, coeffs_sendcounts, coeffs_displs, MPI_DOUBLE, coeff_buffer, part_size + 1, MPI_DOUBLE, root, MPI_COMM_WORLD);

        MPI_Bcast(subtrahend, sz + 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        buffer = division_in_str(buffer, subtrahend, coeff_buffer, sz);

        MPI_Gatherv(buffer, part_size * (sz + 1), MPI_DOUBLE, matrix, matrix_sendcounts, matrix_displs, MPI_DOUBLE, root, MPI_COMM_WORLD);     
        // Может придется расписывать Гавера для каждого процесса
    }

    double* res = new double[sz];

    for (int i = 0; i < sz; i++) {
        res[i] = matrix[(i + 1) * (sz + 1) - 1];
    }

    return res;
}
