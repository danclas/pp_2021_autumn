// Copyright 2021 Kulikov Ilya
#include <mpi.h>
#include <random>
#include "../../../modules/task_1/kulikov_i_lexicographic_ordering/lexicographic_ordering.h"
const char abc[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


char* get_random_str(int sz) {
    char* out_str = new char[sz + 1];
    for (int i = 0; i < sz; i++) { out_str[i] = abc[rand() % 26]; }
    out_str[sz] = '\0';
    return out_str;
}

int finding_in_part(const char* str1, const char* str2, int sz) {
    int count = 0;
    for (int i = 0; i < sz; i++) {
        if (str1[i] > str2[i]) {
            count = 1;
            break;
        } else if (str1[i] < str2[i]) {
            count = -1;
            break;
        }
    }
    return count;
}

int global_finding(const char* str1, const char* str2, int sz) {
    int root = 0, rank = 0, commSize = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int part_size = sz / commSize;
    const int last_part = sz % part_size;

    char* buffer1 = new char[part_size];
    char* buffer2 = new char[part_size];

    int* out_res = new int[commSize+1];
    for (int i = 0; i < commSize; i++)
        out_res[i] = 0;

    MPI_Scatter(str1 + last_part, part_size, MPI_CHAR, buffer1, part_size,
        MPI_CHAR, root, MPI_COMM_WORLD);
    MPI_Scatter(str2 + last_part, part_size, MPI_CHAR, buffer2, part_size,
        MPI_CHAR, root, MPI_COMM_WORLD);

    int local_res = 0;
    local_res = finding_in_part(buffer1, buffer2, part_size);
    if (rank != root) {
        MPI_Send(&local_res, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == root) {
        for (int i = 1; i < commSize; i++) {
            MPI_Status status;
            MPI_Recv(out_res + i + 1, 1, MPI_INT,
                i, 0, MPI_COMM_WORLD, &status);
        }
        out_res[0] = finding_in_part(str1, str2, last_part);
        out_res[1] = local_res;
    }

    for (int i=0; i < commSize + 1; i++)
        if (out_res[i] != 0)
            return out_res[i];
    return 0;
}
