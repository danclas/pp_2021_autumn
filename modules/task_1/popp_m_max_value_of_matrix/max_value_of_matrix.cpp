// Copyright 2021 Popp Maksim
#include <mpi.h>
#include <random>
#include "../../../modules/task_1/popp_m_max_value_of_matrix/max_value_of_matrix.h"

int searchMaxSequentially(int* arr, int k, int l) {
    int max = arr[0];
    for (int i = 1; i < k * l; i++)
        if (max < arr[i]) {
            max = arr[i];
        }
    return max;
}

int* generateMatrixRandom(int k, int l) {
    std::random_device dev;
    std::mt19937 gen(dev());
    int* matrix = new int[k * l];
    for (int i = 0; i < k * l; i++) { matrix[i] = gen() % 100; }
    return matrix;
}



int findingMaxInParallel(int* arr, int k, int l) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int counterOfElem = k * l / size;
    const int difference = k * l % size;

    int* matrix = new int[counterOfElem];

    MPI_Scatter(arr + difference, counterOfElem, MPI_INT,
        matrix, counterOfElem, MPI_INT, 0, MPI_COMM_WORLD);

    int simpleMax = searchMaxSequentially(matrix, 1, counterOfElem);
    int hardMax = matrix[0];
    MPI_Reduce(&simpleMax, &hardMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        for (int i = 0; i < difference; i++) {
            if (hardMax < arr[i]) {
                hardMax = arr[i];
            }
        }
    }

    return hardMax;
}
