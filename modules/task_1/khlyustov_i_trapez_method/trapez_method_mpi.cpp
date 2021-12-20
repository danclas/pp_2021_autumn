// Copyright 2021 Khlyustov Ilya
#include "../../../modules/task_1/khlyustov_i_trapez_method/trapez_method_mpi.h"

double getTrapez(double a, double h, double n, const std::function<double(double)>& f,
    int proc_rank = 0, int proc_count = 1) {
    double sum = 0;
    double x;
    for (int i = proc_rank; i < n; i += proc_count) {
        x = a + i * h;
        sum += h * (f(x) + f(x + h)) / 2;
    }
    return sum;
}

double getSequential(double a, double b, int n, const std::function<double(double)>& f) {
    return getTrapez(a, (b - a) / n, n, f);
}

double getParallel(double a, double b, int n, const std::function<double(double)>& f) {
    int proc_count, proc_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    double global_sum;
    double local_sum = getTrapez(a, (b - a) / n, n, f, proc_rank, proc_count);
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_sum;
}
