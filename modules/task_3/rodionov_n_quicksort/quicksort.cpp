// Copyright 2021 TexHik620953
#include "./../../modules/task_3/rodionov_n_quicksort/quicksort.h"
#include <mpi.h>
#include <stdlib.h>
#include <cstring>
#include <random>


void RandomArray(int* arr, int length) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 10000);
    for (int i = 0; i < length; i++) {
        arr[i] = dist(rng);
    }
}

bool CheckOrdering(int* arr, int length) {
    for (int i = 1; i < length; i++) {
        if ((arr[i] - arr[i - 1]) < 0)
            return false;
    }
    return true;
}

void quickSort(int* arr, int size) {
    int i = 0;
    int j = size - 1;
    int mid = arr[size / 2];

    do {
        while (arr[i] < mid) {
            i++;
        }
        while (arr[j] > mid) {
            j--;
        }
        if (i <= j) {
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;

            i++;
            j--;
        }
    } while (i <= j);

    if (j > 0) {
        quickSort(arr, j + 1);
    }
    if (i < size) {
        quickSort(&arr[i], size - i);
    }
}



void QuickSortMpi(int* arr, int size) {
    int procCount = 0, rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int block_size = size / procCount;
    int shift = size % procCount;

    int* buffer = new int[block_size];
    int* result = nullptr;

    // Send copy of array to each process
    if (rank == 0) {
        result = new int[size];
        for (int i = 0; i < size; i++) {
            result[i] = 0;
        }
    }

    MPI_Scatter(
        arr + shift, block_size, MPI_INT,
        buffer, block_size, MPI_INT,
        0, MPI_COMM_WORLD);
    // Sort part of array
    quickSort(buffer, block_size);

    if (rank != 0) {
        MPI_Send(buffer, block_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        // Copy shift values
        memcpy(result, arr, shift * sizeof(int));
        // Copy this process result values
        memcpy(result + shift, buffer, block_size * sizeof(int));
        // Receive values from other processes
        for (int i = 1; i < procCount; i++) {
            MPI_Status status{};
            MPI_Recv(result + shift + i * block_size,
                block_size,
                MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
        // Sort result array
        quickSort(result, size);
        memcpy(arr, result, size * sizeof(int));
        delete[] result;
    }
    delete[] buffer;
}
