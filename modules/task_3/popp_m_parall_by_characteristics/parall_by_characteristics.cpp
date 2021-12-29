// Copyright 2021 Popp Maksim
#include "../../../modules/task_3/popp_m_parall_by_characteristics/parall_by_characteristics.h"

#include <mpi.h>

#include <climits>
#include <random>
#include <set>

threeRes calc(double a, double b, double x, double (*function)(double x, double y),
    double eps, int iterations_max, double r) {
    bool flag = false;
    std::set<oneElemDim> set;
    threeRes result;
    result.x = x;

    double M, curr_M, m, R, curr_R, new_x;
    double temp = function(x, a);

    set.insert(oneElemDim(a, temp));
    result.y = a;
    result.z = temp;

    temp = function(x, b);
    set.insert(oneElemDim(b, temp));

    if (result.z > temp) {
        result.y = b;
        result.z = temp;
    }

    int iteration = 2;
    auto max_R_iter = set.begin();
    auto max_prev_R_iter = set.begin();
    while (!flag && iteration < iterations_max) {
        M = -1;
        auto iter = set.begin();
        iter++;
        auto prev_iter = set.begin();
        while (iter != set.end()) {
            curr_M = std::abs(static_cast<double>(
                (iter->y - prev_iter->y) / (iter->x - prev_iter->x)));
            if (curr_M > M) M = curr_M;
            iter++;
            prev_iter++;
        }

        if (M > 0) {
            m = r * M;
        } else {
            m = 1;
        }

        iter = set.begin();
        iter++;
        prev_iter = set.begin();

        R = INT_MIN;

        while (iter != set.end()) {
            curr_R = m * (iter->x - prev_iter->x) +
                (std::pow((iter->y - prev_iter->y), 2) /
                    (m * (iter->x - prev_iter->x))) -
                2 * (iter->y - prev_iter->y);
            if (curr_R > R) {
                R = curr_R;
                max_R_iter = iter;
                max_prev_R_iter = prev_iter;
            }
            iter++;
            prev_iter++;
        }

        iteration++;

        new_x = (0.5) * (max_R_iter->x + max_prev_R_iter->x) -
            ((max_R_iter->y - max_prev_R_iter->y) / (2 * m));
        temp = function(x, new_x);
        set.insert(oneElemDim(new_x, temp));

        if (result.z > temp) {
            result.y = new_x;
            result.z = temp;
        }
        if (max_R_iter->x - max_prev_R_iter->x <= eps) {
            flag = true;
        }
    }
    return result;
}

threeRes parallelOptimization(double a1, double b1, double a2,
    double b2,
    double (*function)(double x, double y),
    double eps, int iterations_max,
    double eps_one,
    int iterations_max_one, double r) {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        return sequentialOptimization(a1, b1, a2, b2, function);
    }

    threeRes result_global = { 0, 0, 0 };
    threeRes result;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::set<twoElemDim> set;

        int iteration;
        if (std::abs(a1 - b1) <= 0.0001 || std::abs(a2 - b2) <= 0.0001) {
            iteration = size + 1;
        } else {
            iteration = size;
        }

        double delta = (b1 - a1) / (iteration - 1);

        for (int i = 0; i < size - 1; ++i) {
            double x = a1 + i * delta;
            MPI_Send(&x, 1, MPI_DOUBLE, i + 1, 1, MPI_COMM_WORLD);
        }
        result = calc(a2, b2, b1, function, eps_one);
        set.insert(twoElemDim(result.x, result.y, result.z));
        result_global = result;

        if (iteration != size) {
            result = calc(a2, b2, a1 + delta * size, function, eps_one);
            set.insert(twoElemDim(result.x, result.y, result.z));
            if (result.z < result_global.z) {
                result_global = result;
            }
        }

        for (int i = 0; i < size - 1; ++i) {
            MPI_Recv(&result, 3, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD,
                MPI_STATUSES_IGNORE);
            if (result.z < result_global.z) result_global = result;
            set.insert(twoElemDim(result.x, result.y, result.z));
        }

        std::set<elemR> set_R;
        double M, curr_M, m, curr_R, new_x;

        bool flag = false;
        while (!flag && iteration < iterations_max) {
            set_R.clear();
            M = -1;
            auto iter = set.begin();
            iter++;
            auto prev_iter = set.begin();

            while (iter != set.end()) {
                curr_M =
                    std::abs(static_cast<double>((iter->z - prev_iter->z) /
                        (iter->x - prev_iter->x)));
                if (curr_M > M) M = curr_M;
                iter++;
                prev_iter++;
            }

            if (M > 0) {
                m = r * M;
            } else {
                m = 1;
            }

            iter = set.begin();
            iter++;
            prev_iter = set.begin();

            while (iter != set.end()) {
                curr_R = m * (iter->x - prev_iter->x) +
                    (std::pow((iter->z - prev_iter->z), 2) /
                        (m * (iter->x - prev_iter->x))) -
                    2 * (iter->z - prev_iter->z);
                set_R.insert(elemR(curr_R, iter->x, iter->z,
                    prev_iter->x, prev_iter->z));
                iter++;
                prev_iter++;
            }

            auto iter_R = set_R.begin();
            for (int i = 0; i < size - 1; ++i) {
                iteration++;
                new_x = (0.5) * (iter_R->x + iter_R->x_prev) -
                    ((iter_R->z - iter_R->z_prev) / (2 * m));
                MPI_Send(&new_x, 1, MPI_DOUBLE, i + 1, 1, MPI_COMM_WORLD);
                if (iter_R->x - iter_R->x_prev <= eps) {
                    flag = true;
                }
                iter_R++;
            }

            for (int i = 0; i < size - 1; ++i) {
                MPI_Recv(&result, 3, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD,
                    MPI_STATUSES_IGNORE);
                if (result.z < result_global.z) result_global = result;
                set.insert(twoElemDim(result.x, result.y, result.z));
            }
        }

        for (int i = 0; i < size - 1; ++i) {
            double flag = eps * 0.001;
            MPI_Send(&flag, 1, MPI_DOUBLE, i + 1, 1, MPI_COMM_WORLD);
        }
    } else {
        bool flag = false;
        while (!flag) {
            double x;
            MPI_Recv(&x, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            if (x == eps * 0.001) {
                flag = true;
            } else {
                result = calc(a2, b2, x, function, eps_one, iterations_max_one);
                MPI_Send(&result, 3, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
            }
        }
    }
    return result_global;
}

threeRes sequentialOptimization(double a1, double b1, double a2, double b2,
    double (*function)(double x, double y), double eps, int iterations_max,
    double eps_one,
    int iterations_max_one, double r) {
    threeRes result_global = { 0, 0, 0 };
    threeRes result;
    std::set<twoElemDim> set;

    result = calc(a2, b2, a1, function, eps_one, iterations_max_one);
    set.insert(twoElemDim(result.x, result.y, result.z));
    result_global = result;
    result = calc(a2, b2, b1, function, eps_one, iterations_max_one);
    set.insert(twoElemDim(result.x, result.y, result.z));

    if (result.z < result_global.z) {
        result_global = result;
    }

    double M, curr_M, m, curr_R, new_x;
    int iteration = 2;

    std::set<elemR> set_R;

    bool flag = false;
    while (!flag && iteration < iterations_max) {
        set_R.clear();
        M = -1;

        auto iter = set.begin();
        iter++;
        auto prev_iter = set.begin();

        while (iter != set.end()) {
            curr_M = std::abs(static_cast<double>(
                (iter->z - prev_iter->z) / (iter->x - prev_iter->x)));
            if (curr_M > M) M = curr_M;
            iter++;
            prev_iter++;
        }

        if (M > 0) {
            m = r * M;
        } else {
            m = 1;
        }

        iter = set.begin();
        iter++;
        prev_iter = set.begin();

        while (iter != set.end()) {
            curr_R = m * (iter->x - prev_iter->x) +
                (std::pow((iter->z - prev_iter->z), 2) /
                    (m * (iter->x - prev_iter->x))) -
                2 * (iter->z - prev_iter->z);
            set_R.insert(elemR(curr_R, iter->x, iter->z, prev_iter->x,
                prev_iter->z));
            iter++;
            prev_iter++;
        }

        iteration++;
        auto iter_R = set_R.begin();
        new_x = (0.5) * (iter_R->x + iter_R->x_prev) -
            ((iter_R->z - iter_R->z_prev) / (2 * m));
        result = calc(a2, b2, new_x, function, eps_one, iterations_max_one);
        set.insert(twoElemDim(result.x, result.y, result.z));

        if (result.z < result_global.z) {
            result_global = result;
        }
        if (iter_R->x - iter_R->x_prev <= eps) {
            flag = true;
        }
    }
    return result_global;
}

bool correct(threeRes a, threeRes b, double eps) {
    bool flag = false;
    if (std::abs(static_cast<double>(a.x - b.x)) <= eps) {
        if (std::abs(static_cast<double>(a.y - b.y)) <= eps) {
            if (std::abs(static_cast<double>(a.z - b.z)) <= eps) {
                flag = true;
            }
        }
    }
    return flag;
}
