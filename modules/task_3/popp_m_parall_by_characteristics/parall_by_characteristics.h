#ifndef MODULES_TASK_3_POPP_M_PARALL_BY_CHARACTERISTICS_PARALL_BY_CHARACTERISTICS_H_
#define MODULES_TASK_3_POPP_M_PARALL_BY_CHARACTERISTICS_PARALL_BY_CHARACTERISTICS_H_
#include <cmath>

struct oneElemDim {
    double x;
    double y;
    oneElemDim(double x, double y) : x(x), y(y) {}
    friend bool operator<(const oneElemDim& l, const oneElemDim& r) {
        return l.x < r.x;
    }
};

struct twoElemDim {
    double x;
    double y;
    double z;
    twoElemDim(double x, double y, double z = 0) : x(x), y(y), z(z) {}
    friend bool operator<(const twoElemDim& l, const twoElemDim& r) {
        return l.x < r.x;
    }
};

struct elemR {
    double R;
    double x;
    double z;
    double x_prev;
    double z_prev;
    elemR(double R, double x, double z, double x_prev, double z_prev)
        : R(R), x(x), z(z), x_prev(x_prev), z_prev(z_prev) {}
    friend bool operator<(const elemR& l, const elemR& r) { return l.R > r.R; }
};

struct threeRes {
    double x;
    double y;
    double z;
};

bool correct(threeRes a, threeRes b, double eps);

threeRes calc(double a, double b, double x,
    double (*func)(double x, double y), double eps = 0.1,
    int iterations_max = 100, double r = 2.0);

threeRes parallelOptimization(double a1, double b1, double a2, double b2,
    double (*func)(double x, double y), double eps = 0.1,
    int iterations_max = 100, double eps_one = 0.1,
    int iterations_max_one = 100, double r = 2.0);

threeRes sequentialOptimization(
    double a1, double b1, double a2, double b2,
    double (*func)(double x, double y), double eps = 0.1,
    int iterations_max = 100, double eps_one = 0.1,
    int iterations_max_one = 100, double r = 2.0);

#endif  // MODULES_TASK_3_POPP_M_PARALL_BY_CHARACTERISTICS_PARALL_BY_CHARACTERISTICS_H_