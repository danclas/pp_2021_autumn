// Copyright 2021 Preobrazhenskaya Yuliya
#include "../../../modules/task_3/preobrazhenskaya_y_multi_dimension_monte_carlo/multi_dimension_monte_carlo.h"

double getSequentialOperations(const std::vector<double>& lower,
    const std::vector<double>& upper,
    int number_of_points,
    const std::function<double(const std::vector<double>&)>& func) {
    int dimension = static_cast<int>(upper.size());
    std::mt19937 gen(time(0));
    std::vector<std::uniform_real_distribution<double>> uniform(dimension);

    double result = 0.0;

    for (int i = 0; i < dimension; i++)
        uniform[i] = std::uniform_real_distribution<double>(lower[i], upper[i]);

    std::vector<double> node(dimension);

    for (int i = 0; i < number_of_points; i++) {
        for (int j = 0; j < dimension; j++)
            node[j] = uniform[j](gen);
        result += func(node);
    }
    for (int i = 0; i < dimension; i++) {
        result *= (upper[i] - lower[i]);
    }
    result /= number_of_points;
    return result;
}

double getParallelOperations(const std::vector<double>& lower,
    const std::vector<double>& upper,
    int number_of_points,
    const std::function<double(const std::vector<double>&)>& func) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int dimension = static_cast<int>(upper.size());
    std::vector<double>
        node(static_cast<unsigned int>(number_of_points * dimension));

    if (rank == 0) {
        std::mt19937 gen(time(0));
        std::vector<std::uniform_real_distribution<double>> uniform(dimension);
        for (int i = 0; i < dimension; i++) {
            uniform[i] = std::uniform_real_distribution<double>
                (lower[i], upper[i]);
        }
        for (int i = 0; i < number_of_points; i++) {
            for (int j = 0; j < dimension; j++) {
                node[(i * dimension) + j] = uniform[j](gen);
            }
        }
    }

    double local_result = 0.0;
    double global_result = 0.0;
    int local_number_of_points = number_of_points / size;
    int delta = number_of_points % size;
    std::vector<double> local_node(local_number_of_points * dimension);

    MPI_Scatter(&node[0], local_number_of_points * dimension, MPI_DOUBLE,
        &local_node[0], local_number_of_points * dimension, MPI_DOUBLE,
        0, MPI_COMM_WORLD);

    std::vector<double> node_(dimension);

    for (int i = 0; i < local_number_of_points; i++) {
        for (int j = 0; j < dimension; j++) {
            node_[j] = local_node[(i * dimension) + j];
        }
        local_result += func(node_);
    }

    if (rank == 0) {
        for (int i = 0; i < delta; i++) {
            for (int j = 0; j < dimension; j++) {
                node_[j] =
                    node[(local_number_of_points * size + i) * dimension + j];
            }
            local_result += func(node_);
        }
    }

    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE,
        MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < dimension; i++) {
            global_result *= (upper[i] - lower[i]);
        }
        global_result /= number_of_points;
    }

    return global_result;
}
