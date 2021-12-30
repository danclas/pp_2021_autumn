// Copyright 2021 Stepanov Alexander
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/stepanov_a_gauss_horizontal_scheme/gauss_horizontal_scheme.h"


std::vector<double> generateSLE(std::vector<double>* matrix_sle,
    std::vector<double>* vector_result,
    std::size_t number_unknows) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<> urd(-50, 50);
    std::vector<double> expected_solution(number_unknows, 0);

    for (std::size_t i = 0; i < number_unknows; i++) {
        auto random = urd(gen);
        expected_solution[i] = random == 0.0 ? 1.0 : random;
        for (std::size_t j = 0; j < number_unknows; j++) {
            (*matrix_sle)[i * number_unknows + j] = urd(gen);
        }
    }

    for (std::size_t i = 0; i < number_unknows; i++) {
        (*vector_result)[i] = 0;
        for (std::size_t j = 0; j < number_unknows; j++) {
            (*vector_result)[i] += (*matrix_sle)[i * number_unknows + j] * expected_solution[j];
        }
    }

    return expected_solution;
}

std::vector<double> sequentialGaussScheme(const std::vector<double>& matrix_sle,
    const std::vector<double>& vector_result,
    std::size_t number_unknows) {
    std::vector<double> modiffed_sle(matrix_sle);
    std::vector<double> modiffed_vector_res(vector_result);
    std::vector<double> solution(number_unknows, 0);

    for (std::size_t i = 0; i < number_unknows; i++) {
        for (std::size_t j = i + 1; j < number_unknows; j++) {
            double coeff = modiffed_sle[j * number_unknows + i] / modiffed_sle[i * number_unknows + i];
            modiffed_vector_res[j] -= modiffed_vector_res[i] * coeff;
            for (std::size_t k = i + 1; k < number_unknows; k++) {
                modiffed_sle[j * number_unknows + k] -= modiffed_sle[i * number_unknows + k] * coeff;
            }
            modiffed_sle[j * number_unknows + i] = 0;
        }
    }

    for (auto i = number_unknows - 1; i > 0; i--) {
        double coeff = modiffed_vector_res[i];
        for (auto j = number_unknows - 1; j > i; j--) {
            coeff -= modiffed_sle[i * number_unknows + j] * solution[j];
        }
        solution[i] = coeff / modiffed_sle[i * number_unknows + i];
    }

    double coeff = modiffed_vector_res[0];
    for (std::size_t i = 1; i < number_unknows; i++) {
        coeff -= modiffed_sle[i] * solution[i];
    }
    solution[0] = coeff / modiffed_sle[0];

    return solution;
}

std::vector<double> parallelGaussScheme(const std::vector<double>& matrix_sle,
    const std::vector<double>& vector_result,
    std::size_t number_unknows) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size > static_cast<int>(number_unknows) || size == 1) {
        return rank == 0 ? sequentialGaussScheme(matrix_sle, vector_result, number_unknows) :
            std::vector<double>();
    }

    std::size_t local_size = number_unknows / size;
    std::size_t remains_proc0 = number_unknows % size;

    int matrix_tag = 0;
    int vector_tag = 1;
    if (rank == 0) {
        for (int num_proc = 1; num_proc < size; num_proc++) {
            std::size_t offset_sle = (local_size * num_proc + remains_proc0) * number_unknows;
            std::size_t offset_vector_res = local_size * num_proc + remains_proc0;

            MPI_Send(matrix_sle.data() + offset_sle, static_cast<int>(local_size * number_unknows),
                MPI_DOUBLE, num_proc, matrix_tag, MPI_COMM_WORLD);
            MPI_Send(vector_result.data() + offset_vector_res, static_cast<int>(local_size),
                MPI_DOUBLE, num_proc, vector_tag, MPI_COMM_WORLD);
        }
    }

    std::vector<double>local_matrix_sle((local_size + remains_proc0) * number_unknows);
    std::vector<double>local_vector_res(local_size + remains_proc0);

    if (rank == 0) {
        for (std::size_t i = 0; i < local_size + remains_proc0; i++) {
            local_vector_res[i] = vector_result[i];
            for (std::size_t j = 0; j < number_unknows; j++) {
                local_matrix_sle[i * number_unknows + j] = matrix_sle[i * number_unknows + j];
            }
        }
    } else {
        MPI_Status status;

        MPI_Recv(local_matrix_sle.data(), static_cast<int>(local_size * number_unknows),
            MPI_DOUBLE, 0, matrix_tag, MPI_COMM_WORLD, &status);
        MPI_Recv(local_vector_res.data(), static_cast<int>(local_size),
            MPI_DOUBLE, 0, vector_tag, MPI_COMM_WORLD, &status);
    }

    std::vector<double> recivied_part_sle((local_size + remains_proc0) * number_unknows);
    std::vector<double> recivied_part_vector(local_size + remains_proc0);

    for (int iter = 0; iter <= rank; iter++) {
        if (iter == rank) {
            auto offset = iter == 0 ? 0 : iter * local_size + remains_proc0;
            auto current_size = iter == 0 ? local_size + remains_proc0 : local_size;
            for (std::size_t i = 0; i < current_size; i++) {
                for (std::size_t j = i + 1; j < current_size; j++) {
                    double coeff = local_matrix_sle[j * number_unknows + i + offset] /
                        local_matrix_sle[i * number_unknows + i + offset];
                    local_vector_res[j] -= local_vector_res[i] * coeff;
                    for (std::size_t k = i + offset + 1; k < number_unknows; k++) {
                        local_matrix_sle[j * number_unknows + k] -= local_matrix_sle[i * number_unknows + k] * coeff;
                    }
                    local_matrix_sle[j * number_unknows + i + offset] = 0;
                }
            }

            for (int num_proc = iter + 1; num_proc < size; num_proc++) {
                MPI_Send(local_matrix_sle.data(), static_cast<int>(local_matrix_sle.size()),
                    MPI_DOUBLE, num_proc, matrix_tag, MPI_COMM_WORLD);
                MPI_Send(local_vector_res.data(), static_cast<int>(local_vector_res.size()),
                    MPI_DOUBLE, num_proc, vector_tag, MPI_COMM_WORLD);
            }
        } else {
            MPI_Status status;
            MPI_Recv(recivied_part_sle.data(), static_cast<int>(recivied_part_sle.size()),
                MPI_DOUBLE, iter, matrix_tag, MPI_COMM_WORLD, &status);
            MPI_Recv(recivied_part_vector.data(), static_cast<int>(recivied_part_vector.size()),
                MPI_DOUBLE, iter, vector_tag, MPI_COMM_WORLD, &status);

            auto offset = iter == 0 ? 0 : local_size + remains_proc0 + iter * local_size - local_size;
            auto size_reciv_sle = iter == 0 ? local_size + remains_proc0 : local_size;

            for (std::size_t i = 0; i < local_size; i++) {
                for (std::size_t j = 0; j < size_reciv_sle; j++) {
                    double coeff = local_matrix_sle[i * number_unknows + j + offset] /
                        recivied_part_sle[j * number_unknows + j + offset];
                    local_vector_res[i] -= recivied_part_vector[j] * coeff;
                    for (std::size_t k = j + offset + 1; k < number_unknows; k++) {
                        local_matrix_sle[i * number_unknows + k] -= recivied_part_sle[j * number_unknows + k] * coeff;
                    }
                    local_matrix_sle[i * number_unknows + j + offset] = 0;
                }
            }
        }
    }

    std::vector<double> solution(number_unknows, 0);
    int sender = rank + 1;
    int recipient = rank - 1;
    int solution_tag = 2;

    if (rank != size - 1) {
        MPI_Status status;

        MPI_Recv(solution.data(), static_cast<int>(number_unknows),
            MPI_DOUBLE, sender, solution_tag, MPI_COMM_WORLD, &status);
    }

    auto offset = ((size - 1) - rank) * local_size;
    if (rank == 0) {
        local_size += remains_proc0;
    }

    for (std::size_t i = 0; i < local_size; i++) {
        double coeff = local_vector_res[local_size - 1 - i];
        for (std::size_t j = 0; j < i + offset; j++) {
            coeff -= local_matrix_sle[local_size * number_unknows - 1 - i * number_unknows - j] *
                solution[number_unknows - 1 - j];
        }
        solution[number_unknows - 1 - i - offset] = coeff /
            local_matrix_sle[local_size * number_unknows - 1 - number_unknows * i - i - offset];
    }

    if (rank != 0) {
        MPI_Send(solution.data(), static_cast<int>(number_unknows),
            MPI_DOUBLE, recipient, solution_tag, MPI_COMM_WORLD);
    }

    return solution;
}
