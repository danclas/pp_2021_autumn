// Copyright 2021 ghanga moukela brexe juniors
#include <mpi.h>
#include <algorithm>
#include <stdexcept>
#include <ctime>
#include <vector>
#include "random"
#include "iostream"
#include "../../../modules/task_3/ghanga_b_sobel/operator_sobel.h"

int clamp(int v, int max, int min) {
    if (v > max) return max;
    else if (v < min) return min;
    return v;
}

std::vector<int> calc_sobel(std::vector<int> image, int rows, int cols) {
    std::vector<int> result_image(rows * cols);
    std::vector<int> vectorx = {
         -1, 0, 1,
         -2, 0, 2,
         -1, 0, 1
    };
    std::vector<int> vectory = {
       -1, -2, -1,
        0,  0,  0,
        1,  2,  1
    };
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            int Gx = 0;
            int Gy = 0;
            int G = 0;
            if (y == 0 || y == cols) {
                G = 0;
            } else if (x == 0 || x == rows) {
                G = 0;
            } else {
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        int idx = (i + 1) * 3 + j + 1;
                        int x1 = clamp(x + j, rows - 1, 0);
                        int y1 = clamp(y + i, cols - 1, 0);
                        Gx += image[x1 * cols + y1] * vectorx[idx];
                        Gy += image[x1 * cols + y1] * vectory[idx];
                    }
                }
            }
            G = sqrt(pow(Gx, 2) + pow(Gy, 2));
            G = clamp(G, 255, 0);
            result_image[x * cols + y] = G;
        }
    }
    return result_image;
}

int calc_kernel(std::vector<int> image, int x, int y, int rows, int cols) {
    std::vector<int> vectorx = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };
    std::vector<int> vectory = {
       -1, -2, -1,
        0,  0,  0,
        1,  2,  1
    };
    int Gx = 0;
    int Gy = 0;
    int G = 0;
    if (y == 0 || y == cols) {
        G = 0;
    } else if (x == 0 || x == rows) {
        G = 0;
    } else {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int idx = (i + 1) * 3 + j + 1;
                int x1 = clamp(x + j, rows - 1, 0);
                int y1 = clamp(y + i, cols - 1, 0);
                Gx += image[x1 * cols + y1] * vectorx[idx];
                Gy += image[x1 * cols + y1] * vectory[idx];
            }
        }
    }
    G = sqrt(pow(Gx, 2) + pow(Gy, 2));
    G = clamp(G, 255, 0);
    return G;
}

std::vector<int> parallel_sobel(std::vector<int> image, int rows, int cols) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if ((rows < size) || (size == 1)) {
        return calc_sobel(image, rows, cols);
    }
    int delta = rows / size;
    int remain = rows % size;
    int* sendbuf = new int[rows];
    int* sendcounts = new int[size];
    int* displs = new int[size];
    for (int i = 0; i < rows; ++i) {
        sendbuf[i] = i;
    }
    for (int i = 0; i < size; ++i) {
        sendcounts[i] = delta + !!remain;
        if (remain) {
            --remain;
        }
    }
    int k = 0;
    for (int i = 0; i < size; ++i) {
        displs[i] = k;
        k += sendcounts[i];
    }
    std::vector<int> recvbuf(sendcounts[rank]);
    MPI_Scatterv(&sendbuf[0], sendcounts, displs, MPI_INT, &recvbuf[0], sendcounts[rank],
        MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> local_result(cols * recvbuf.size());
    for (int i = 0; i < sendcounts[rank]; ++i) {
        for (int j = 0; j < cols; ++j) {
            local_result[i * cols + j] = calc_kernel(image, recvbuf[i], j, rows, cols);
        }
    }
    std::vector<int> global_result(rows * cols);
    int* sendcounts1 = new int[size];
    int* displs1 = new int[size];
    for (int i = 0; i < size; ++i) {
        sendcounts1[i] = cols * recvbuf.size();
    }
    k = 0;
    for (int i = 0; i < size; i++) {
        displs1[i] = k * cols;
        k += sendcounts[i];
    }
    MPI_Gatherv(local_result.data(), sendcounts1[rank], MPI_INT, global_result.data(),
        sendcounts1, displs1, MPI_INT, 0, MPI_COMM_WORLD);
    return global_result;
}

