// Copyright 2021 Gordeev Viktor
#ifndef MODULES_TASK_3_GORDEEV_V_STRONGIN_STRONGIN_MPI_H_
#define MODULES_TASK_3_GORDEEV_V_STRONGIN_STRONGIN_MPI_H_

#include <math.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <list>
#include <string>

struct block {
    double R;
    double M;
    double x_left;
    double x_right;
    double z_left;
    double z_right;
};

bool operator<(const block& i1, const block& i2);
bool operator>(const block& i1, const block& i2);

double R(const double& _m_small, const double& _z_curr, const double& _z_prev,
    const double& _x_curr, const double& _x_prev);
double M(const double& _z_curr, const double& _z_prev, const double& _x_curr,
    const double& _x_prev);
double StronginParallel(double left, double right, const double _Epsilon,
    const int _Steps, double r, int procs);
void StronginSeq(double left, double right, double r, int procs, int rank);
double func(const double& _x);

#define XLEFT 1
#define XRIGHT 2
#define ZLEFT 3
#define ZRIGHT 4
#define MSEND 5
#define RSEND 6

#endif  // MODULES_TASK_3_GORDEEV_V_STRONGIN_STRONGIN_MPI_H_
