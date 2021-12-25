// Copyright 2021 Belyaev Ilya
#include <mpi.h>
#include <functional>
#include "../../../modules/task_1/belyaev_i_trapezoid_method1/trapezoid_method.h"

double getIntegral(double a, double b, int n,
    const std::function<double(double)>& f) {
    const double width = (b - a) / n;

    double trapezoidal_integral = 0;
    for (int i = 0; i < n; i++) {
        const double x1 = a + i * width;
        const double x2 = a + (i + 1) * width;

        trapezoidal_integral += 0.5 * (x2 - x1) * (f(x1) + f(x2));
    }

    return trapezoidal_integral;
}

double getParallel(double a, double b, int n,
    const std::function<double(double)>& f) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double global_sum;

    const double width = (b - a) / n;
    double local_sum = 0;

    for (int i = rank; i < n; i += size) {
        const double x1 = a + i * width;
        const double x2 = a + (i + 1) * width;

        local_sum += 0.5 * (x2 - x1) * (f(x1) + f(x2));
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
        MPI_COMM_WORLD);

    return global_sum;
}
