// Copyright 2021 Lakhov Kirill
#include "../../../modules/task_3/lakhov_k_optimization_params/optimization_params.h"

double f1(double x, double y) {
    return pow(y - 1, 2) + pow(x, 2);
}

double f2(double x, double y) {
    return 4 + pow(pow(x, 2) + pow(y, 2), 2.0 / 3);
}

double f3(double x, double y) {
    return x + 4 * y - 2 * log10(x*y) - 3 * log10(y);
}

double f4(double x, double y) {
    return pow(x, 2) - 2*x*y + pow(y, 3);
}

double f5(double x, double y) {
    return pow(x, 2) + 3*x*y + 4*pow(y, 2);
}

Point singleDimensionMin(double left_x, double right_x, double const_y,
                         double(*func)(double x, double y)) {
    int n_max_value = 1000;
    double eps_in_func = 0.01;
    double r = 2;

    Point result;
    std::set<singleDimensionChar> set;


    double left_func_value = func(left_x, const_y);
    set.insert(singleDimensionChar(left_x, left_func_value));

    result.x = left_x;
    result.y = const_y;
    result.z = left_func_value;

    double right_func_value = func(right_x, const_y);
    set.insert(singleDimensionChar(right_x, right_func_value));

    if (result.z > right_func_value) {
        result.x = right_x;
        result.z = right_func_value;
    }

    int k = 2;
    auto maxRiter = set.begin();
    auto r_i_value_previous_max = set.begin();
    double mu, mu_current;
    double M;
    bool st_flag = false;
    while (!st_flag && k < n_max_value) {
        // calc mu and M
        mu = -1;
        auto i_value = set.begin();
        i_value++;
        auto i_previous_value = set.begin();
        while (i_value != set.end()) {
            mu_current = std::abs(static_cast<double>(
            (i_value->functionValue - i_previous_value->functionValue) /
            (i_value->x - i_previous_value->x)));
            if (mu_current > mu) {mu = mu_current;}
            i_value++; i_previous_value++;
        }
        if (mu > 0) {
            M = r * mu;
        } else {
            M = 1;
        }

        i_value = set.begin();
        i_value++;
        i_previous_value = set.begin();
        double R, current_r;
        R = std::numeric_limits<double>::lowest();;
        while (i_value != set.end()) {
            double delta_x = (i_value->x - i_previous_value->x);
            double a = M * delta_x;
            double delta_f = i_value->functionValue -
                                    i_previous_value->functionValue;
            double b = pow(delta_f, 2) / a;
            double c = 2 * (delta_f);
            current_r = a + b - c;
            if (current_r > R) {
                R = current_r;
                maxRiter = i_value;
                r_i_value_previous_max = i_previous_value;
            }
            i_value++; i_previous_value++;
        }
        k++;
        double new_x = 0.5 * (maxRiter->x + r_i_value_previous_max->x) -
        ((maxRiter->functionValue - r_i_value_previous_max->functionValue) /
                                                                     (2 * M));
        double new_func_value = func(new_x, const_y);
        set.insert(singleDimensionChar(new_x, new_func_value));
        if (result.z > new_func_value) {
            result.x = new_x;
            result.z = new_func_value;
        }
        if (maxRiter->x - r_i_value_previous_max->x <= eps_in_func) {
            st_flag = true;
        }
    }

    return result;
}

Point sequentialCalc(double left_x, double right_x,
                     double left_y, double right_y,
                     double(*func)(double x, double y)) {
    double eps = 0.01;
    double n_max_value = 1000;
    double(*ptr)(double, double) = func;
    double r = 2;

    Point result = {0, 0, 0};
    Point last_result = {0, 0, 0};
    std::set<doubleDimensionChar> set;

    result = singleDimensionMin(left_x, right_x, left_y, ptr);
    set.insert(doubleDimensionChar(result.x, result.y, result.z));
    last_result = result;

    result = singleDimensionMin(left_x, right_x, right_y, ptr);
    set.insert(doubleDimensionChar(result.x, result.y, result.z));

    if (result.z < last_result.z) { last_result = result;}

    int k = 2;
    bool stop = false;
    double mu, mu_current;
    double M;
    while (!stop && k < n_max_value) {
        // calc mu and M
        mu = -1;
        auto i_value = set.begin();
        i_value++;
        auto i_previous_value = set.begin();
        while (i_value != set.end()) {
            double delta_f = i_value->functionValue -
                                i_previous_value->functionValue;
            mu_current = std::abs(static_cast<double>((delta_f) /
            (i_value->y - i_previous_value->y)));
            if (mu_current > mu) {mu = mu_current;}
            i_value++; i_previous_value++;
        }
        if (mu > 0) {
            M = r * mu;
        } else {
            M = 1;
        }

        // calc R(i)
        // R(i) = m(x(i) - x(i-1)) + (z(i) - z(i-1))^2/ m(x(i)-x(i-1))
        // - 2(z(i)-z(i-1))
        i_value = set.begin();
        i_value++;
        i_previous_value = set.begin();
        double R, current_r;
        R = std::numeric_limits<double>::lowest();;

        auto maxRiter = set.begin();
        auto r_i_value_previous_max = set.begin();
        while (i_value != set.end()) {
            double delta_y = (i_value->y - i_previous_value->y);
            double delta_f = i_value->functionValue -
                                i_previous_value->functionValue;
            double a = M * delta_y;
            double b = pow((delta_f), 2) / a;
            double c = 2 * (delta_f);
            current_r = a + b - c;
            if (current_r > R) {
                R = current_r;
                maxRiter = i_value;
                r_i_value_previous_max = i_previous_value;
            }
            i_value++; i_previous_value++;
        }
        k++;

        double new_y = 0.5 * (maxRiter->y + r_i_value_previous_max->y) -
        ((maxRiter->functionValue - r_i_value_previous_max->functionValue) /
                                                                    (2 * M));

        result = singleDimensionMin(left_x, right_x, new_y, ptr);
        set.insert(doubleDimensionChar(result.x, result.y, result.z));
        if (result.z < last_result.z) {
            last_result = result;
        }
        if (maxRiter->y - r_i_value_previous_max->y <= eps) {
            stop = true;
        }
    }
    return last_result;
}

Point parralelCalc(double left_x, double right_x,
                   double left_y, double right_y,
                   double(*func)(double x, double y)) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size <= 1) {
        return sequentialCalc(left_x, right_x, left_y, right_y, func);
    }

    double eps = 0.01;
    double r = 2;
    int max_iterations = 1000;
    Point result = {0, 0, 0};
    Point last_result = {0, 0, 0};
    std::vector<double> result_sender(3);

    if (rank == 0) {
        int should_stop = 0;
        int stop_signal = 0;
        std::set<doubleDimensionChar> set;
        double len_of_part = (right_y - left_y) / (size-1);
        for (int i = 1; i < size; i++) {
            double y_value_send = left_y + i*len_of_part;
            MPI_Send(&stop_signal, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&y_value_send, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        }
        result = singleDimensionMin(left_x, right_x, left_y, func);
        set.insert(doubleDimensionChar(result.x, result.y, result.z));
        last_result = result;
        for (int i = 1; i < size ; i++) {
            MPI_Recv(result_sender.data(), 3, MPI_DOUBLE, MPI_ANY_SOURCE, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            result.x = result_sender[0]; result.y = result_sender[1];
            result.z = result_sender[2];
            if (result.z < last_result.z) { last_result = result; }
            set.insert(doubleDimensionChar(result.x, result.y, result.z));
        }

        int k = size;
        std::set<Interval> total_interval_set;
        double mu, mu_current;
        double M;
        while (should_stop == 0 && k < max_iterations) {
            auto set_iter = set.begin(); set_iter++;
            auto set_iter_previos = set.begin();

            mu = -1;
            while (set_iter != set.end()) {
                double delta_f = set_iter->functionValue -
                                set_iter_previos->functionValue;
                mu_current = std::abs(static_cast<double>((delta_f) /
                (set_iter->y - set_iter_previos->y)));
                if (mu_current > mu) {mu = mu_current;}
                set_iter++; set_iter_previos++;
            }
            if (mu > 0) {
                M = r * mu;
            } else {
                M = 1;
            }

            set_iter = set.begin(); set_iter++;
            set_iter_previos = set.begin();
            double current_r;

            total_interval_set.clear();
            while (set_iter != set.end()) {
                double delta_y = (set_iter->y - set_iter_previos->y);
                double delta_f = set_iter->functionValue -
                                set_iter_previos->functionValue;
                double a = M * delta_y;
                double b = pow((delta_f), 2) / a;
                double c = 2 * (delta_f);
                current_r = a + b - c;
                total_interval_set.insert(Interval(current_r,
                set_iter_previos->y, set_iter_previos->functionValue,
                set_iter->y, set_iter->functionValue));
                set_iter++; set_iter_previos++;
            }

            k += size;
            auto r_iter = total_interval_set.begin();
            for (int i = 1; i < size; i++) {
                double new_y = 0.5 *
                    (r_iter->variableFirst + r_iter->variableSecond) -
                    ((r_iter->functionValueSecond - r_iter->functionValueFirst)
                    / (2 * M));
                MPI_Send(&stop_signal, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
                MPI_Send(&new_y, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
                if (r_iter->variableSecond - r_iter->variableFirst <= eps) {
                    should_stop = 1;
                }
                r_iter++;
            }
            for (int i = 1; i < size; i++) {
                MPI_Recv(result_sender.data(), 3, MPI_DOUBLE, MPI_ANY_SOURCE, 1,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                result.x = result_sender[0]; result.y = result_sender[1];
                result.z = result_sender[2];
                if (result.z < last_result.z) { last_result = result; }
                set.insert(doubleDimensionChar(result.x, result.y, result.z));
            }
        }
        stop_signal = 1;
        for (int i = 1; i < size; ++i) {
            MPI_Send(&stop_signal, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    } else {
        while (true) {
            int stop_signal;
            MPI_Recv(&stop_signal, 1, MPI_INT, 0, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (stop_signal) {
                break;
            }
            double const_y;
            MPI_Recv(&const_y, 1, MPI_DOUBLE, 0, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            result = singleDimensionMin(left_x, right_x, const_y, func);
            result_sender[0] = result.x; result_sender[1] = result.y;
            result_sender[2] = result.z;
            MPI_Send(result_sender.data(), 3, MPI_DOUBLE,
                                           0, 1, MPI_COMM_WORLD);
        }
    }
    return last_result;
}

bool comparePoints(const Point& left, const Point& right) {
    const double eps = 0.01;
    bool dx = std::abs(left.x - right.x) <= eps;
    bool dy = std::abs(left.y - right.y) <= eps;
    bool dz = std::abs(left.z - right.z) <= eps;
    return dx && dy && dz;
}
