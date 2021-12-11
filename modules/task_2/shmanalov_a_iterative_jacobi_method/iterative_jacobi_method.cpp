// Copyright 2021 Shmanalov Alexander
#include <mpi.h>
#include <random>
#include <cmath>
#include <cstring>
#include "../../../modules/task_2/shmanalov_a_iterative_jacobi_method/iterative_jacobi_method.h"

void generationRandomData(int* matrixA, int* matrixB, int size) {
    std::random_device random;
    std::mt19937 generate(random());
    std::uniform_int_distribution<> range(-20, 20);
    for (int i = 0; i < size; i++) {
        double sumElemRow = 0;
        for (int j = 0; j < size; j++) {
            if (i != j) {
                matrixA[i * size + j] = range(generate);
                sumElemRow += abs(matrixA[i * size + j]);
            }
        }
        matrixA[i * size + i] = sumElemRow + 15;
        matrixB[i] = range(generate);
    }
}

void sequentialJacobi(int* matrixA, int* matrixB, double* resultX,
                      const double epsilon, int size) {
    double* tempX = new double[size];
    double norm;
    do {
        for (int i = 0; i < size; i++) {
            tempX[i] = matrixB[i];
            for (int j = 0; j < size; j++) {
                if (i != j)
                    tempX[i] -= matrixA[i * size + j] * resultX[j];
            }
            tempX[i] /= matrixA[i * size + i];
        }
        norm = fabs(resultX[0] - tempX[0]);
        for (int i = 0; i < size; i++) {
            if (fabs(resultX[i] - tempX[i]) > norm) {
                norm = fabs(resultX[i] - tempX[i]);
            }
            resultX[i] = tempX[i];
        }
    } while (norm > epsilon);
    delete[] tempX;
}

void parallelIteration(int* tempA, int* tempB, double* xPrev, double* xRes,
                       int rank, int rowsOnProcess, int size) {
    for (int i = 0; i < rowsOnProcess; i++) {
        xRes[i] = tempB[i];
        for (int j = 0; j < size; j++) {
            if (j != i + rank * rowsOnProcess)
                xRes[i] -= tempA[i * size + j] * xPrev[j];
        }
        xRes[i] /= tempA[i * size + i + rank * rowsOnProcess];
    }
}

double calcDifference(double* xPrev, double* xRes, int size) {
    double norm = 0.0;
    for (int i = 0; i < size; i++) {
        if (fabs(xPrev[i] - xRes[i]) > norm)
            norm = fabs(xPrev[i] - xRes[i]);
    }
    return norm;
}

void parallelJacobi(int* matrixA, int* matrixB, double* resultX,
                    const double epsilon, int size) {
    // MPI information
    int processRank, numberOfProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
    // data separation
    int possibleDivision = size % numberOfProcess;
    if (possibleDivision == 0) {
        // number of equations per process
        int rowsOnProcess = size / numberOfProcess;
        MPI_Bcast(&rowsOnProcess, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // initializing intermediate arrays
        int* tempA = new int[rowsOnProcess * size];
        int* tempB = new int[rowsOnProcess];
        double* tempX = new double[size];
        // initial values for X
        double* xPrev = new double[size];
        for (int i = 0; i < size; i++) {
            xPrev[i] = 0.0;
        }
        // data distribution
        MPI_Scatter(matrixA, rowsOnProcess * size, MPI_INT,
                    tempA, rowsOnProcess * size, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(matrixB, rowsOnProcess, MPI_INT,
                    tempB, rowsOnProcess, MPI_INT, 0, MPI_COMM_WORLD);
        // algorithm
        double norm;
        int iter = 0;
        do {
            MPI_Bcast(xPrev, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            parallelIteration(tempA, tempB, xPrev, tempX,
                              processRank, rowsOnProcess, size);
            MPI_Gather(tempX, rowsOnProcess, MPI_DOUBLE,
                       resultX, rowsOnProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            if (processRank == 0) {
                norm = calcDifference(xPrev, resultX, size);
                memcpy(xPrev, resultX, size * sizeof(double));
            }
            MPI_Bcast(&norm, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            iter++;
        } while (norm > epsilon);
        // delete intermediate arrays
        delete[]tempA;
        delete[]tempB;
        delete[]tempX;
    } else {
        if (processRank == 0) {
            for (int i = 0; i < size; i++) {
                resultX[i] = 0.0;
            }
            sequentialJacobi(matrixA, matrixB, resultX, epsilon, size);
        }
    }
}
