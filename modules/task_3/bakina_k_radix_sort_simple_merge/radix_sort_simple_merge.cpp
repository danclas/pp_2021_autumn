// Copyright 2021 Bakina Ksenia
#include "../../../modules/task_3/bakina_k_radix_sort_simple_merge/radix_sort_simple_merge.h"

void get_random_vector(std::vector<int>* vector) {
    std::srand(std::time(0));
    int size = vector->size();
    for (int i = 0; i < size; i++) {
        vector->at(i) = std::rand() * (std::rand() % 2 == 0 ? 1 : -1);
    }
}

std::vector<int> merge(const std::vector<int>& array1, const std::vector<int>& array2) {
    int size1 = array1.size();
    int size2 = array2.size();
    std::vector<int> result(size1 + size2);
    int it_res, it1, it2;
    it_res = it1 = it2 = 0;
    while (it1 < size1 && it2 < size2) {
        if (array1[it1] < array2[it2]) {
            result[it_res] = array1[it1];
            ++it_res; ++it1;
        } else {
            result[it_res] = array2[it2];
            ++it_res; ++it2;
        }
    }
    while (it1 < size1) {
        result[it_res] = array1[it1];
        ++it_res; ++it1;
    }
    while (it2 < size2) {
        result[it_res] = array2[it2];
        ++it_res; ++it2;
    }
    return result;
}

void sort_of_rank(std::vector<int>* array, int64_t rank) {
    int size = array->size();
    std::vector<std::vector<int>> tmp(19);
    for (int i = 0; i < size; i++) {
        int digit = (array->at(i) % rank) / (rank / 10);
        tmp[9 + digit].push_back(array->at(i));
    }
    int k = 0;
    for (int i = 0; i < 19; i++) {
        int tmp_size = tmp[i].size();
        for (int j = 0; j < tmp_size; j++, k++) {
            array->at(k) = tmp[i][j];
        }
    }
}

void linear_radix_sort(std::vector<int>* array) {
    int64_t x = 10;
    for (int i = 1; i < 11; i++) {
        sort_of_rank(array, x);
        x *= 10;
    }
}

std::vector<int> parallel_radix_sort(const std::vector<int>& array) {
    int proc_count;
    int proc_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    std::vector<int> result_array;

    int size = array.size();
    std::vector<int> sendcounts(proc_count);
    std::vector<int> displs(proc_count);
    int elem_for_proc = size / proc_count + (proc_rank == proc_count - 1 ? size % proc_count : 0);
    std::vector<int> recvbuf(elem_for_proc);
    if (proc_rank == 0) {
        for (int i = 0; i < proc_count; i++) {
            sendcounts[i] = size / proc_count + (i == proc_count - 1 ? size % proc_count : 0);
        }
        displs[0] = 0;
        for (int i = 1; i < proc_count; i++) {
            displs[i] = displs[i - 1] + sendcounts[i - 1];
        }
    }

    MPI_Scatterv(array.data(), sendcounts.data(), displs.data(),
        MPI_INT, recvbuf.data(), elem_for_proc, MPI_INT, 0, MPI_COMM_WORLD);

    linear_radix_sort(&recvbuf);

    if (proc_rank != 0) {
        MPI_Send(reinterpret_cast<void*>(recvbuf.data()), elem_for_proc, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        result_array = recvbuf;
        MPI_Status status;
        for (int i = 1; i < proc_count; i++) {
            recvbuf.resize(sendcounts[i]);
            MPI_Recv(recvbuf.data(), sendcounts[i], MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            result_array = merge(result_array, recvbuf);
        }
    }
    return result_array;
}
