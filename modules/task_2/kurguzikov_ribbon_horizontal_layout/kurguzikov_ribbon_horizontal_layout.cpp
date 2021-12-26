  // Copyright 2021 Kurguzikov Kirill
#include <mpi.h>
#include <random>
#include "../../../modules/task_2/kurguzikov_ribbon_horizontal_layout/kurguzikov_ribbon_horizontal_layout.h"


int* initEmptyMatrix(int str, int column) {
    int* matrix = new int[str * column];
    for (int i = 0; i < str * column; i++) {
        matrix[i] = 0;
    }
    return matrix;
}

int* getRandomMatrix(int str, int column) {
    std::random_device dev;
    std::mt19937 gen(dev());
    int* matrix = new int[str * column];
    for (int i = 0; i < str * column; i++) {
        matrix[i] = gen() % 20;
    }
    return matrix;
}

void SequentialMatrixMultiplication(int* A, int Astring, int Acolumn,
    int* B, int Bstring, int Bcolumn, int* C) {
    int tmp;
    for (int i = 0; i < Astring; i++) {
        for (int j = 0; j < Bcolumn; j++) {
            C[i * Bcolumn + j] = 0;
            tmp = 0;
            for (int k = 0; k < Acolumn; k++) {
                tmp += A[i * Acolumn + k] * B[k * Bcolumn + j];
                C[i * Bcolumn + j] = tmp;
            }
        }
    }
}

void ParallelMatrixMultiplication(int* A, int Astring, int Acolumn,
    int* B, int Bstring, int Bcolumn, int* C) {

    if (Acolumn != Bstring) {
        throw - 1;
    }

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int ribbon_width = Astring / world_size;
    const int remainder = Astring % world_size;
    int* ribbon = nullptr;
    int* local_product = nullptr;

    int* Bt = new int[Bcolumn * Bstring];

    if (rank == 0) {
        for (int n = 0; n < Bstring * Bcolumn; n++) {
            int i = n / Bstring;
            int j = n % Bstring;
            Bt[n] = B[Bcolumn * j + i];
        }
    }

    MPI_Bcast(Bt, Bcolumn * Bstring, MPI_INT, 0, MPI_COMM_WORLD);

    ribbon = new int[Acolumn * ribbon_width];
    local_product = new int[Bcolumn * ribbon_width];

    MPI_Scatter(A, ribbon_width * Acolumn, MPI_INT,
        ribbon, ribbon_width * Acolumn, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < ribbon_width; i++) {
        for (int j = 0; j < Bcolumn; j++) {
            int tmp = 0;
            for (int k = 0; k < Acolumn; k++) {
                tmp += ribbon[i * Acolumn + k] * Bt[j * Bstring + k];
            }
            local_product[i * Bcolumn + j] = tmp;
        }
    }

    MPI_Gather(local_product, Bcolumn * ribbon_width, MPI_INT,
        C, Bcolumn * ribbon_width, MPI_INT, 0, MPI_COMM_WORLD);

    if (remainder) {
        if (rank == 0) {
            for (int i = ribbon_width * world_size;
                i < ribbon_width * world_size + remainder; i++) {
                for (int j = 0; j < Bcolumn; j++) {
                    int tmp = 0;
                    for (int k = 0; k < Acolumn; k++) {
                        tmp += A[i * Acolumn + k] * Bt[j * Bstring + k];
                    }
                    C[i * Bcolumn + j] = tmp;
                }
            }
        }
    }
}
