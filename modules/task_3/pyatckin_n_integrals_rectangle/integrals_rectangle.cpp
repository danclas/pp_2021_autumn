// Copyright 2021 Pyatckin Nikolai
#include <math.h>
#include <mpi.h>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <utility>
#include "../../../modules/task_3/pyatckin_n_integrals_rectangle/integrals_rectangle.h"

double ParallelIntegral(std::function<double(const std::vector<double>)>
    func, std::vector <std::pair<double, double>> scope, const int num) {

    int dim = scope.size();
    int num_rang;
    std::vector<double> local_scope(dim);
    std::vector<std::pair<double, double >> ranges(dim);
    int num_elem;

    int numprocs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ranges = scope;
    num_rang = num;
    num_elem = pow(num_rang, dim - 1);
    if (rank == 0) {
        for (int i = 0; i < dim; ++i) {
            local_scope[i] = (scope[i].second - scope[i].first) / num_rang;
        }
    }

    MPI_Bcast(&num_elem, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&num_rang, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&local_scope[0], dim, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ranges[0], 2 * dim, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int rem = num_elem % numprocs;
    int count = num_elem / numprocs;
    std::vector<std::vector<double>> params(count);
    int tmp = 0;
    if (rank < rem) {
        count++;
        tmp = rank * count;
    } else {
        tmp = rem * (count + 1) + (rank - rem) * count;
    }

    for (int i = 0; i < count; ++i) {
        int number = tmp + i;
        for (int j = 0; j < dim - 1; ++j) {
            int cef = number % num_rang;
            params[i].push_back(ranges[j].first
                + cef * local_scope[j] + local_scope[j] / 2);
            number /= num_rang;
        }
    }
    double sum = 0.0;
    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < num_rang; ++j) {
            params[i].push_back(ranges[dim - 1].first + j
                * local_scope[dim - 1] + local_scope[dim - 1] / 2);
            sum += func(params[i]);
            params[i].pop_back();
        }
    }

    double res = 0.0;
    MPI_Reduce(&sum, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    for (int i = 0; i < dim; ++i) {
        res *= local_scope[i];
    }
    return res;
}

double SeqIntegral(std::function<double(const std::vector<double>)> func,
    std::vector <std::pair<double, double>> scope, const int  num) {
    int count = scope.size();
    std::vector<double> local_scope(count);
    int64_t number = 1;
    for (int i = 0; i < count; ++i) {
        local_scope[i] = (scope[i].second - scope[i].first) / num;
        number *= num;
    }
    std::vector <double> params(count);
    double sum = 0.0;
    for (int i = 0; i < number; ++i) {
        int x = i;
        for (int j = 0; j < count; ++j) {
            int cef = x % num;
            params[j] = scope[j].first + cef *
                local_scope[j] + local_scope[j] / 2;
            x /= num;
        }
        sum += func(params);
    }
    for (int i = 0; i < count; ++i) {
        sum *= local_scope[i];
    }
    return sum;
}
