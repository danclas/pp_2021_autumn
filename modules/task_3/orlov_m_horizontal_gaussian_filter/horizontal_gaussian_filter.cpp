//  Copyright 2021 Maksim Orlov

#define _USE_MATH_DEFINES
#include <mpi.h>
#include <vector>
#include <ctime>
#include <iostream>
#include <cmath>
#include <random>
#include "../../../modules/task_3/orlov_m_horizontal_gaussian_filter/horizontal_gaussian_filter.h"

int randNumber(std::random_device* dev, std::mt19937* rng, int max) {
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, max - 1);
    return dist(*rng);
}

double gaussianFunction(int i, int j, double sigma) {
    return 1 / (2 * M_PI * sigma * sigma) * exp(-(i * i + j * j) / (2 * sigma * sigma));
}

std::vector<std::vector<char>> sequentialGaussianFilter(std::vector<std::vector<char>> image, double sigma) {
    int Y = static_cast<int>(image.size());
    int X = static_cast<int>(image[0].size());
    std::vector<std::vector<char>> res;
    for (int y = 1; y < Y - 1; y++) {
        std::vector<char> line;
        for (int x = 1; x < X - 1; x++) {
            double brightness = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    brightness += gaussianFunction(i, j, sigma) * static_cast<int>(image[y + i][x + j]);
                }
            }
            line.push_back(static_cast<char>(brightness));
        }
        res.push_back(line);
    }
    return res;
}

std::vector<std::vector<char>> gaussianFilter(std::vector<std::vector<char>> image, double sigma) {
    int Y = static_cast<int>(image.size()), lineBlocks = Y - 2, rem, lineBlocksPerProc, procs, rank;
    int X = static_cast<int>(image[0].size());
    std::vector<std::vector<char>> res;
    int* displs;
    int* scounts;
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    rem = lineBlocks % procs;
    lineBlocksPerProc = lineBlocks / procs;
    displs = new int[procs];
    scounts = new int[procs];
    int offset = 1;
    for (int i = 0; i < procs; i++) {
        displs[i] = offset;
        if (i < rem) {
            offset += lineBlocksPerProc + 1;
            scounts[i] = lineBlocksPerProc + 1;
        } else {
            offset += lineBlocksPerProc;
            scounts[i] = lineBlocksPerProc;
        }
    }
    //  std::cout << rank << " 1 " << displs[rank] << " " << scounts[rank] << std::endl;
    char* pixels = new char[scounts[rank] * (X - 2)];
    for (int y = displs[rank]; y < displs[rank] + scounts[rank]; y++) {
        //  std::cout << "working on line " << y << std::endl;
        for (int x = 1; x < X - 1; x++) {
            double brightness = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    brightness += gaussianFunction(i, j, sigma) * static_cast<int>(image[y + i][x + j]);
                }
            }
            pixels[(y - displs[rank]) * (X - 2) + (x - 1)] = static_cast<char>(brightness);
        }
    }
    //  std::cout << rank << " 2" << std::endl;
    if (rank != 0) {
        MPI_Send(pixels, scounts[rank] * (X - 2), MPI_CHAR, 0, rank, MPI_COMM_WORLD);
    }
    //  std::cout << rank << " 3" << std::endl;
    if (rank == 0) {
        for (int i = 0; i < procs; i++) {
            char* _p = nullptr;
            if (i != 0) {
                _p = new char[scounts[i] * (X - 2)];
                MPI_Recv(_p, scounts[i] * (X - 2), MPI_CHAR, i, i, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            }
            for (int y = displs[i]; y < displs[i] + scounts[i]; y++) {
                std::vector<char> line(X - 2);
                for (int x = 1; x < X - 1; x++) {
                    if (i != 0) {
                        line[x - 1] = _p[(y - displs[i]) * (X - 2) + (x - 1)];
                    } else {
                        line[x - 1] = pixels[(y - displs[i]) * (X - 2) + (x - 1)];
                    }
                }
                res.push_back(line);
            }
            if (i != 0) {
                delete[] _p;
            }
        }
    }
    //  std::cout << rank << " 4" << std::endl;
    delete[] pixels;
    delete[] displs;
    delete[] scounts;
    return res;
}
