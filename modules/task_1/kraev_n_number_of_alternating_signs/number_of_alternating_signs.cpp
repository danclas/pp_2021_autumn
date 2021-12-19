// Copyright 2021 Kraev Nikita

#include "../../../modules/task_1/kraev_n_number_of_alternating_signs/number_of_alternating_signs.h"
#include <mpi.h>
#include <vector>
#include <random>

std::vector<int> getRandomVector(const int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100, 100);
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = dis(gen);
    }
    return vec;
}

int findingNumberOfSignAlternations(const std::vector<int>& vec) {
    int result = 0;
    int size = vec.size();
    for (int i = 1; i < size; i++) {
        if (vec[i - 1] * vec[i] < 0) {
            result++;
        } else if (vec[i] == 0) {
            i++;
        }
    }
    return result;
}

int parallelFindingNumberOfSignAlternations(const std::vector<int>& vec,
    const int size) {
    int number_proc, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &number_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int global_res = 0;
    int number_of_transmitted_elements = size - 1 + number_proc;
    int proc_size = number_of_transmitted_elements / number_proc;
    int rem_of_div = number_of_transmitted_elements % number_proc;
    std::vector<int> proc_vector(proc_size);

    if (rank == 0) {
        if (number_proc > (size - 1) || number_proc == 1) {
            global_res = findingNumberOfSignAlternations(vec);
            return global_res;
        }
        int offset = proc_size + rem_of_div - 1;
        MPI_Send(vec.data() + offset, proc_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
        for (int i = 2; i < number_proc; i++) {
            offset = offset + proc_size - 1;
            MPI_Send(vec.data() + offset, proc_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        proc_size += rem_of_div;
        proc_vector.resize(proc_size);
        for (int i = 0; i < proc_size; i++) {
            proc_vector[i] = vec[i];
        }
    } else {
        MPI_Status status;
        MPI_Recv(proc_vector.data(), proc_size, MPI_INT, 0, 0, MPI_COMM_WORLD,
            &status);
    }

    int local_res = findingNumberOfSignAlternations(proc_vector);

    MPI_Reduce(&local_res, &global_res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_res;
}







