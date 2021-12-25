// Copyright 2021 Zaytsev Mikhail
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include "../../../modules/task_3/kulemin_p_linear_vertical_filtration/linear_vectrical_filtration.h"

using std::vector;


void getKernell(vector<float>* matrix, float sigma) {
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 1);
    float norm = 0;
    matrix->resize(9);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            int idx = (x + 1) * 3 + (y + 1);
            (*matrix)[idx] = std::exp(-(x * x + y * y) / (sigma * sigma));
            norm += (*matrix)[idx];
        }
    }
    for (int i = 0; i < 9; i++) {
        if ((i != 1) && (i != 4) && (i != 7))(*matrix)[i] = 0.f;
        (*matrix)[i] /= norm;
    }
}

int calculate_index(int x, int y, int weight) {
    return y * weight + x;
}


float calculatedNewPixelColor(const vector<float>& matrix,
 const vector<float>&image, int height,
    int weight, const int x, const int y) {
    float color = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int idx = (i + 1) * 3 + j + 1;
            float imgColor = image[calculate_index(clamp(x + j, weight - 1, 0),
             clamp(y + i, height - 1, 0), weight)];
            color += (imgColor) * matrix[idx];
        }
    }
    return color;
}


void getRandomImg(std::vector<float>* img, int weight, int height) {
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 1);

    img->resize(weight*height);
    for (int i = 0; i < (weight*height); ++i) {
        img->at(i) = urd(gen);
    }
}

std::vector<float> getSequentialOperations(const std::vector<float>& matrix,
 const std::vector<float>& img, int weight, int height) {
    vector<float> resultMatrix(weight * height);

    for (int x = 0; x < weight; ++x) {
        for (int  y = 0; y < height; y++) {
            float value = calculatedNewPixelColor(matrix, img,
             height, weight, x, y);
            resultMatrix[calculate_index(x, y, weight)] = value;
        }
    }

    return resultMatrix;
}

std::vector<float> getParallelOperations(const std::vector<float>& matrix,
const std::vector<float>& img, int weight, int height) {
    int num, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int delta = weight / num;
    int rem = weight % num;
    vector<int> processes(num);
    vector<int> toright(num);
    vector< vector<float> > buffer(height);
    vector<float>res;
    for (int i = 0; i < height; i++)
    buffer[i].resize(weight);
    processes[0] = delta + rem;
    toright[0] = 0;
    for (int i = 1; i < num; i++) {
        processes[i] = delta;
        toright[i] = toright[i - 1] + processes[i-1];
    }
    if (delta) {
            for (int row = 0; row < (height); row++) {
               MPI_Scatterv(img.data() + row * weight, processes.data(),
               toright.data(), MPI_FLOAT, (buffer[row]).data(), processes[rank],
               MPI_FLOAT, 0, MPI_COMM_WORLD);
            }


        vector<float>ipart;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < processes[rank]; j++) {
                ipart.push_back(buffer[i][j]);
            }
        }
        ipart = getSequentialOperations(matrix, ipart, processes[rank], height);

        for (int i = 0; i < height; i++) {
            MPI_Gatherv(ipart.data() + i * processes[rank],
            processes[rank], MPI_FLOAT,
            (buffer[i]).data(), processes.data(),
            toright.data(), MPI_FLOAT, 0, MPI_COMM_WORLD);
        }
        if (rank == 0) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < weight; j++) {
                    res.push_back(buffer[i][j]);
                }
            }
        }
    } else {
        if (rank == 0) {
            res = getSequentialOperations(matrix, img, weight, height);
        }
    }
    return res;
}
