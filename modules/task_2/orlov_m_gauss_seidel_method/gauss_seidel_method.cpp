//  Copyright 2021 Maksim Orlov

#include <mpi.h>
#include <vector>
#include <ctime>
#include <iostream>
#include <cmath>
#include <random>
#include "../../../modules/task_2/orlov_m_gauss_seidel_method/gauss_seidel_method.h"

int randNumber(std::random_device* dev, std::mt19937* rng, int max) {
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, max - 1);
    return dist(*rng);
}

std::vector<double> sequentialGaussSeidel(std::vector<std::vector<double>> coeffMatrix,
    std::vector<double> rightSide, double eps) {
    int dimension = static_cast<int>(coeffMatrix.size());
    std::vector<double> res;
    std::vector<double> prev;
    for (int i = 0; i < dimension; i++)
        res.push_back(0);
    bool flag;
    do {
        prev = res;
        flag = true;
        for (int i = 0; i < dimension; i++) {
            res[i] = rightSide[i];
            for (int j = 0; j < i; j++)
                res[i] -= coeffMatrix[i][j] * res[j];
            for (int j = i + 1; j < dimension; j++) {
                res[i] -= coeffMatrix[i][j] * prev[j];
            }
            res[i] /= coeffMatrix[i][i];
            if (std::abs(res[i] - prev[i]) >= eps) {
                flag = false;
            }
        }
    } while (!flag);
    return res;
}

std::vector<double> gaussSeidel(std::vector<std::vector<double>> coeffMatrix,
    std::vector<double> rightSide, double eps) {
    int dimension = static_cast<int>(coeffMatrix.size()), rem, linesPerProc, procs, rank;
    std::vector<double> res;
    std::vector<double> prev;
    for (int i = 0; i < dimension; i++)
        res.push_back(0);
    int* displs;
    int* scounts;
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    rem = dimension % procs;
    linesPerProc = dimension / procs;
    displs = new int[procs];
    scounts = new int[procs];
    int offset = 0;
    for (int i = 0; i < procs; i++) {
        displs[i] = offset;
        if (i < rem) {
            offset += linesPerProc + 1;
            scounts[i] = linesPerProc + 1;
        } else {
            offset += linesPerProc;
            scounts[i] = linesPerProc;
        }
    }
    int flag;
    do {
        prev = res;
        flag = 1;
        for (int i = 0; i < dimension; i++) {
            double sum = 0, fullSum = 0;
            for (int j = displs[rank]; j < displs[rank] + scounts[rank]; j++) {
                if (j < i) {
                    sum += coeffMatrix[i][j] * res[j];
                }
                if (j > i) {
                    sum += coeffMatrix[i][j] * prev[j];
                }
            }
            MPI_Allreduce(&sum, &fullSum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            res[i] = rightSide[i] - fullSum;
            res[i] /= coeffMatrix[i][i];
            if (std::abs(res[i] - prev[i]) >= eps) {
                flag = 0;
            }
        }
    } while (!flag);
    return res;
}
