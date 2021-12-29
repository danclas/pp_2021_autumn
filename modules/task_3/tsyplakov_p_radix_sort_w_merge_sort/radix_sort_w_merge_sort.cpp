// Copyright 2021 Tsyplakov Pavel
#include <mpi.h>
#include <vector>
#include <random>
#include <queue>
#include <cmath>
#include "../../../modules/task_3/tsyplakov_p_radix_sort_w_merge_sort/radix_sort_w_merge_sort.h"


std::vector<double> getRandomVector(int sz) {
    std::mt19937 gen(121);
    std::vector<double> vec(sz);
    for (int i = 0; i < sz; i++) { vec[i] = gen() % 10000000 / 100.0; }
    return vec;
}

int calcValueByRank(const double& value, int rank) {
    int power = pow(10, rank);
    return ((static_cast<int>(value * 100)) / power) % 10;
}

int maxRank(const std::vector<double>& vect) {
    double maxElement = *std::max_element(vect.begin(), vect.end());
    maxElement *= 100.0;
    maxElement = static_cast<int>(maxElement);
    int rank = 0;
    while (maxElement > 9) {
        rank++;
        maxElement /= 10;
    }
    return rank;
}

std::vector<double> SequentialRadixSort(const std::vector<double>& vec) {
    std::vector<double> resultVec = vec;
    int maxim_rank = maxRank(vec);
    for (int i = 0; i <= maxim_rank; ++i) {
        std::vector<std::queue<double>> vect_queue(10);  // queue for each rank

        for (std::vector<double>::iterator it = (&resultVec)->begin();
                                      it != (&resultVec)->end(); ++it) {
            vect_queue[calcValueByRank(*it, i)].push(*it);
        }
        int currentQueue = 0;
        for (std::vector<double>::size_type j = 0; j < resultVec.size();) {
            while (!vect_queue[currentQueue].empty()) {
                resultVec[j] = vect_queue[currentQueue].front();
                vect_queue[currentQueue].pop();
                ++j;
            }
            ++currentQueue;
        }
    }
    return resultVec;
}

std::vector<double> simpleMerge(const std::vector<double>& first_vec,
                               const std::vector<double>& second_vec) {
    std::vector<double> result_vec(first_vec.size() + second_vec.size());
    std::vector<double>::size_type firstIt = 0, secondIt = 0, resultIt = 0;

    while (firstIt < first_vec.size() && secondIt < second_vec.size()) {
        if (first_vec[firstIt] < second_vec[secondIt]) {
            result_vec[resultIt] = first_vec[firstIt];
            resultIt++;
            firstIt++;
        } else {
            result_vec[resultIt] = second_vec[secondIt];
            resultIt++;
            secondIt++;
        }
    }

    while (firstIt < first_vec.size()) {
        result_vec[resultIt] = first_vec[firstIt];
        resultIt++;
        firstIt++;
    }
    while (secondIt < second_vec.size()) {
        result_vec[resultIt] = second_vec[secondIt];
        resultIt++;
        secondIt++;
    }

    return result_vec;
}

std::vector<double> ParallelRadixSort(const std::vector<double>& global_vec,
              const std::vector<double>::size_type count_size_vector) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int delta = count_size_vector / size;
    int tail = count_size_vector % size;

    std::vector<double> localVector(delta), globalVector;

    MPI_Scatter(global_vec.data(), delta, MPI_DOUBLE, localVector.data(),
        delta, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    localVector = SequentialRadixSort(localVector);

    if (rank != 0) {
        MPI_Send(localVector.data(), delta, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        globalVector = localVector;

        for (int i = 1; i < size; ++i) {
            MPI_Recv(localVector.data(), delta, MPI_DOUBLE, i, 0,
                             MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            // PROBLEM: OUT OF BOUNDS
            globalVector = simpleMerge(globalVector, localVector);
        }

        if (tail) {
            localVector = std::vector<double>(global_vec.end() - tail,
                                                    global_vec.end());
            localVector = SequentialRadixSort(localVector);

            globalVector = simpleMerge(globalVector, localVector);
        }
    }
    return globalVector;
}
