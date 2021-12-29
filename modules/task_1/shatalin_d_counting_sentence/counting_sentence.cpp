// Copyright 2021 Shatalin Dmitriy
#include "../../../modules/task_1/shatalin_d_counting_sentence/counting_sentence.h"

std::string random_str(int size) {
    std::string s;
    std::mt19937 rand_r(time(0));
    static const char ch[] = "ABCDEFGHIJKLM .!?NOPQRSTUVWXYZ";

    for (int i = 0; i < size; ++i) {
        s += ch[rand_r() % (sizeof(ch) - 1)];
    }
    return s;
}

int num_of_sentences_seq(std::string str) {
    const int str_size = str.length();
    int count = 0;
    for (int i = 0; i < str_size; i++) {
        if (str[i] == '.' || str[i] == '!' || str[i] == '?') {
            count++;
        }
    }
    return count;
}

int num_of_sentences_par(std::string str) {
    int total_amount = 0;
    int size;
    int rank;
    int part_size = 0;
    int rest = 0;
    int count_of_s = 0;
    int str_size = str.length();
    int start_part;
    int end_part;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (str_size == 0)
        return 0;
    if (size > str_size || size == 1) {
        return num_of_sentences_seq(str);
    } else {
        part_size = str_size / size;
        rest = str_size % size;
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            start_part = 0;
            end_part = 0;
            start_part = i * part_size + rest;
            end_part = start_part + part_size;
            MPI_Send(&start_part, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&end_part, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    if (rank > 0) {
        MPI_Status status;
        MPI_Recv(&start_part, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&end_part, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        for (int i = start_part; i < end_part; i++) {
            if (str[i] == '.' || str[i] == '!' || str[i] == '?') {
                count_of_s++;
            }
        }
    } else {
        int from_zero_to_start = part_size + rest;
        for (int i = 0; i < from_zero_to_start; i++) {
            if (str[i] == '.' || str[i] == '!' || str[i] == '?') {
                count_of_s++;
            }
        }
    }

    MPI_Reduce(&count_of_s, &total_amount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return total_amount;
}
