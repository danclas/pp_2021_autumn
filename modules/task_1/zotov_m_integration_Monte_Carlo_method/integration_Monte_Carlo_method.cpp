// Copyright 2021 Zotov Maksim
#include <mpi.h>
#include <random>
#include <cmath>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_1/zotov_m_integration_Monte_Carlo_method/integration_Monte_Carlo_method.h"


double f1(double x) {
    return pow(x, 2);
}
double f2(double x) {
    return x / 2 + 1;
}
double f3(double x) {
    return cos(x);
}
double f4(double x) {
    return sin(x) * sin(x);
}
double f5(double x) {
    return pow(x, 2) / 3 + 1;
}

double integralMonteCarlo(double a, double b, int n, double(*f)(double)) {
    std::mt19937 generate;
    generate.seed(time(0));
    std::uniform_real_distribution<> uid(0, RAND_MAX);
    double res;
    double x = 0.0;
    double y = 0.0;

    for (int i = 0; i < n; i++) {
        x = (uid(generate)/RAND_MAX) * (static_cast<double>(b - a) + a);
        y += f(x);
    }
    res = (static_cast<double>(b - a) + a) * y / n;
    return res;
}

double integralParallel(double a, double b, int n, double(*f)(double)) {
    std::mt19937 generate;
    generate.seed(time(0));
    std::uniform_real_distribution<> uid(0, RAND_MAX);

    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double myRes = 0.0, res = 0.0;
    double x = 0.0;
    double y = 0.0;

    for (int i = 0; i < n; i += ProcNum) {
        x = (uid(generate) / RAND_MAX) * (static_cast<double>(b - a) + a);
        y += f(x);
    }
    myRes = (static_cast<double>(b - a) + a) * y / n;
    MPI_Reduce(&myRes, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return res;
}
