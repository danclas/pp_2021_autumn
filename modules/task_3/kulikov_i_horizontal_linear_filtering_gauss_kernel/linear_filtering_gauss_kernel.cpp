// Copyright 2021 Kulikov Ilya
#define _USE_MATH_DEFINES
#include <mpi.h>
#include <math.h>
#include <random>
#include <iostream>
#include "../../../modules/task_3/kulikov_i_horizontal_linear_filtering_gauss_kernel/linear_filtering_gauss_kernel.h"


double* get_random_img(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());

    double* out_img = new double[sz * sz];
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            out_img[i * sz + j] = gen() % 255 + 1;
        }
    }
    return out_img;
}

double* get_matrix(double sigma) {
    double* matrix = new double[9];
    double sum = 0.0;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            matrix[i * 3 + j + 4] = 1 / (2 * M_PI * sigma * sigma)
            * exp(-(i * i + j * j) / (2 * sigma * sigma));
            sum += matrix[i * 3 + j + 4];
        }
    }

    for (int i = 0; i < 9; i++) {
        matrix[i] /= sum;
    }
    return matrix;
}

double* changing_img(double* strings, double* matrix, int sz, int part) {
    double* out_strings = new double[sz * part];
    for (int i = 1; i <= part; i++) {
        for (int j = 0; j < sz ; j ++) {
            if (j == 0) {
                double s = strings[(i - 1) * sz] * matrix[0] + strings[i * sz]
                * matrix[3] + strings[(i + 1) * sz] * matrix[6];

                for (int x = -1; x < 2 ; x ++) {
                    for (int y = 0; y < 2 ; y ++) {
                        s += strings[(i + x) * sz + y] * matrix[x * 3 + y + 4];
                    }
                }

                out_strings[(i - 1) * sz] = s;
            } else if (j == sz - 1) {
                double s = strings[i * sz - 1] * matrix[2] +
                strings[(i + 1) * sz - 1] * matrix[5] +
                strings[(i + 2) * sz - 1] * matrix[8];

                for (int x = -1; x < 2 ; x ++) {
                    for (int y = -1; y < 1 ; y ++) {
                        s += strings[(i + x + 1) * sz + y - 1]
                        * matrix[x * 3 + y + 4];
                    }
                }

                out_strings[(i - 1) * sz + j] = s;
            } else {
                double s = 0;

                for (int x = -1; x < 2 ; x ++) {
                    for (int y = -1; y < 2 ; y ++) {
                        s += strings[(i + x) * sz + j + y]
                        * matrix[x * 3 + y + 4];
                    }
                }

                out_strings[(i - 1) * sz + j] = s;
            }
        }
    }
    return out_strings;
}

double* horizontal_linear_filter_gauss(double* img, double* matrix, int sz) {
    int root = 0, rank = 0, commSize = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int part_size = 0;
    int num_parts = 0;

    int* img_sendcounts_in = nullptr;
    int* img_sendcounts_out = nullptr;
    int* img_displs = nullptr;
    double* work_img = nullptr;
    double* out_img = nullptr;

    if (rank == root) {
        work_img = new double[(sz + 2) * sz];

        std::copy(img, img + sz, work_img);
        std::copy(img, img + sz * sz, work_img + sz);
        std::copy(img + (sz - 1) * sz, img + sz * sz, work_img + (sz + 1) * sz);

        part_size = sz / commSize;
        num_parts = sz % commSize;

        img_sendcounts_in = new int[commSize];
        img_sendcounts_out = new int[commSize];
        img_displs = new int[commSize];
        out_img = new double[sz * sz];

        img_displs[0] = 0;

        int q;
        if (0 != num_parts) q = part_size + 3;
        else
        q = part_size + 2;

        img_sendcounts_in[0] = q * sz;
        img_sendcounts_out[0] = (q - 2) * sz;

        for (int i = 1; i < commSize; i++) {
            if (i < num_parts) q = part_size + 3;
            else
            q = part_size + 2;

            img_sendcounts_in[i] = q * sz;
            img_sendcounts_out[i] = (q - 2) * sz;

            img_displs[i] = img_displs[i - 1] + img_sendcounts_out[i - 1];
        }
    }

    MPI_Bcast(&part_size, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&num_parts, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(matrix, 9, MPI_DOUBLE, root, MPI_COMM_WORLD);

    int local_part_size;

    if (rank < num_parts) {
        local_part_size = part_size + 1;
    } else {
        local_part_size = part_size;
    }

    double* buffer = new double[(local_part_size + 2) * sz];
    double* out_buffer = nullptr;

    MPI_Scatterv(work_img, img_sendcounts_in, img_displs, MPI_DOUBLE,
    buffer, (local_part_size + 2) * sz, MPI_DOUBLE,
    root, MPI_COMM_WORLD);

    out_buffer = changing_img(buffer, matrix, sz, local_part_size);

    MPI_Gatherv(out_buffer, local_part_size * sz, MPI_DOUBLE, out_img,
    img_sendcounts_out, img_displs, MPI_DOUBLE,
    root, MPI_COMM_WORLD);

    delete[] img_displs;
    delete[] buffer;
    delete[] out_buffer;
    delete[] img_sendcounts_in;
    delete[] img_sendcounts_out;
    delete[] work_img;

    return out_img;
}
