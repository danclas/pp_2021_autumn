// Copyright 2021 Bakina Ksenia
#include "../../../modules/task_1/bakina_k_matrix_max/max_matrix.h"
#include <mpi.h>
#include <iostream>
#include <climits>
#include <ctime>


void get_random_matrix(int** matrix, const int row_count, const int column_count) {
    std::srand(std::time(0));
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < column_count; j++) {
            matrix[i][j] = std::rand();
        }
    }
}

int linear_search_max_el(const int* sub_matrix, const int size) {
    if (size <= 0 )
        throw "Incorrect size of column(size=0)";
    int max_element = sub_matrix[0];
    for (int i = 0; i < size; i++) {
        if (sub_matrix[i] > max_element)
            max_element = sub_matrix[i];
    }
    return max_element;
}

int parallel_search_max_el(int** matrix, int row_count, int column_count) {
    if (row_count <= 0 || column_count <= 0)
        throw "Incorrect size of column(size=0)";
    int proc_count;
    int proc_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    int* trans_matrix = nullptr;
    int* sendcounts = nullptr;
    int* displs = nullptr;
    int* recvbuf = nullptr;
    int col_for_proc = column_count / proc_count + (proc_rank < column_count % proc_count ? 1 : 0);
    if (proc_rank == 0) {
        int matrix_size = column_count * row_count;
        trans_matrix = new int[matrix_size];
        for (int i = 0; i < column_count; i++) {
            for (int j = 0; j < row_count; j++) {
                trans_matrix[i* row_count +j] = matrix[j][i];
            }
        }
        sendcounts = new int[proc_count];
        for (int i = 0; i < proc_count; i++) {
            sendcounts[i] = (column_count / proc_count + (i < column_count % proc_count ? 1 : 0)) * row_count;
        }
        displs = new int[proc_count];
        displs[0] = 0;
        for (int i = 1; i < proc_count; i++) {
            displs[i] = displs[i - 1] + sendcounts[i - 1];
        }
    }
    int size_recvbuf = col_for_proc * row_count;
    recvbuf = new int[size_recvbuf];

    MPI_Scatterv(reinterpret_cast<void*>(trans_matrix), sendcounts, displs,
        MPI_INT, recvbuf, col_for_proc * row_count, MPI_INT, 0, MPI_COMM_WORLD);

    int max_in_column = linear_search_max_el(recvbuf, size_recvbuf);
    int max_in_matrix = INT_MIN;
    MPI_Reduce(reinterpret_cast<void*>(&max_in_column), reinterpret_cast<void*>(&max_in_matrix),
        1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    return max_in_matrix;
}
