// Copyright 2021 Shurygina A
#include <mpi.h>
#include <random>
#include <algorithm>
#include <ctime>
#include <vector>
#include <iostream>
#include "../../../modules/task_3/shurygina_a_linear_filtration/linefilter_block.h"

std::vector<int> createRandomImg(int width, int height) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    int size = height * width;
    std::vector<int> img(size);
    for (int i = 0; i < size; i++) {
        img[i] = gen() % 255;
    }
    return img;
}

int findBoundaries(int input, int left, int right) {
    if (input < left) {
        input = left;
    } else {
        if (input > right) {
            input = right;
        }
    }
    return input;
}

int simpleGaussianFilter(std::vector<int>& img,
    int height, int width, int radius,
    double sigma, std::vector<double>& kernel,
    int row, int cols) {
    int Local = 0;
    int diam = radius * 2 + 1;
    double sum = 0;
    for (int i = -radius; i < radius; i++) {
        for (int j = -radius; j < radius; j++) {
            int index = (j + radius) + ((i + radius) * diam);
            int x = findBoundaries(row + j, 0, height - 1);
            int y = findBoundaries(cols + i, 0, width - 1);
            double color = img[x * width + y];
            sum += color * kernel[index];
        }
    }
    return Local = findBoundaries(sum, 0, 255);
}

std::vector<double> createGaussianKernel(int size, double sigma) {
    int diam = size * 2 + 1;
    double normal = 0;
    std::vector<double> kernel(diam * diam);
    for (int i = -size; i <= size; i++) {
        for (int j = -size; j <= size; j++) {
            int index = (j + size) + ((i + size) * diam);
            kernel[index] = exp(-(pow(i, 2) + pow(j, 2)) / (pow(sigma, 2)));
            normal += kernel[index];
        }
    }
    for (int i = 0; i < pow(diam, 2); i++) {
        kernel[i] = kernel[i] / normal;
    }
    return kernel;
}

std::vector<int> parallelSegmentFilter(std::vector<int>& img, int height,
    int width, int radius, double sigma,
    std::vector<double>& kernel, int begin, int add) {
    std::vector<int> local_result_mas(add * width);
    for (int i = 0; i < add; i++) {
        for (int j = 0; j < width; j++) {
            local_result_mas[i * width + j] =
                simpleGaussianFilter(img, height, width, radius,
                    sigma, kernel, i + begin, j);
        }
    }
    return local_result_mas;
}

std::vector<int> parallelGaussianFilter(std::vector<int>& img, int height,
    int width, int radius, double sigma,
    std::vector<double>& kernel) {
    int processNumber, processRank;
    MPI_Comm_size(MPI_COMM_WORLD, &processNumber);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

    int del = height / processNumber;
    int remaind = height % processNumber;
    int pixesNumber = height * width;
    int* sendbuf = new int[height];
    int* sendCount = new int[processNumber];
    int* displs = new int[processNumber];

    for (int i = 0; i < height; i++) {
        sendbuf[i] = i;
    }
    for (int i = 0; i < processNumber; i++) {
        if (remaind > 0) {
            sendCount[i] = del + 1;
            remaind = remaind - 1;
        } else {
            sendCount[i] = del;
        }
    }
    int flagSended = 0;
    for (int i = 0; i < processNumber; i++) {
        displs[i] = flagSended;
        flagSended += sendCount[i];
    }
    std::vector<int> recvBuf(sendCount[processRank]);
    // int MPI_Scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype,
    // void* recvbuf, int recvcount, MPI_Datatype recvtype,
    // int root, MPI_Comm comm)
    MPI_Scatterv(&sendbuf[0], sendCount, displs, MPI_INT, &recvBuf[0],
        sendCount[processRank], MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> local_result(recvBuf.size() * width);
    int sendedData = 0;
    for (int i = 0; i < processRank; i++) {
        sendedData += sendCount[i];
    }
    local_result = parallelSegmentFilter(img, height, width, radius, sigma,
        kernel, sendedData, sendCount[processRank]);

    std::vector<int> globalResult(pixesNumber);
    int* sendCountsGlobal = new int[processNumber];
    int* displsGlobal = new int[processNumber];
    for (int i = 0; i < processNumber; i++) {
        sendCountsGlobal[i] = static_cast<int>(width * recvBuf.size());
    }
    int sendedData2 = 0;
    for (int i = 0; i < processNumber; i++) {
        displsGlobal[i] = sendedData2 * width;
        sendedData2 += sendCount[i];
    }
    // int MPI_Gather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
    // void* recvbuf, int recvcount, MPI_Datatype recvtype,
    // int root, MPI_Comm comm)
    MPI_Gatherv(&local_result[0], sendCountsGlobal[processRank],
        MPI_INT, &globalResult[0], sendCountsGlobal, displsGlobal, MPI_INT,
        0, MPI_COMM_WORLD);
    return globalResult;
}

// void show(std::vector<int> lol, int height, int width) {
//    for (int i = 0; i < height * width; i++) {
//        std::cout << lol[i] << " ";
//        if ((i + 1) % width == 0) {
//            std::cout << std::endl;
//        }
//    }
//    std::cout << std::endl;
//}
