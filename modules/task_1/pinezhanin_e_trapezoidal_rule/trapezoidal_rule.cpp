// Copyright 2021 Pinezhanin Evgeny
#include <mpi.h>
#include "../../../modules/task_1/pinezhanin_e_trapezoidal_rule/trapezoidal_rule.h"

double getIntegralTrapezoidalRuleParallel(const std::function<double(double)>& f, double a, double b, int n) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double res_sum = 0.0;
    double sum = 0.0;
    const double h = (b - a) / n;
    if (n > 0) {
        for (int i = rank; i < n; i += size) {
            sum += (f(a + i * h) + f(a + (i + 1) * h)) * 0.5 * h;
        }
        MPI_Reduce(&sum, &res_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    return res_sum;
}

double getIntegralTrapezoidalRuleSequential(const std::function<double(double)>& f, double a, double b, int n) {
    double res_sum = 0.0;
    const double h = (b - a) / n;
    if (n > 0) {
        for (int i = 0; i < n; i++) {
            res_sum += (f(a + i * h) + f(a + (i + 1) * h)) * 0.5 * h;
        }
    }
    return res_sum;
}
