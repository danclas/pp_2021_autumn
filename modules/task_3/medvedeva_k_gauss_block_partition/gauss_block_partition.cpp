// Copyright 2021 Medvedeva Karina
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/medvedeva_k_gauss_block_partition/gauss_block_partition.h"

static std::vector<double> gauss_kernel(9, 0);

std::vector<int> getRandomMatrix(std::vector<int>::size_type row_count, std::vector<int>::size_type column_count) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(row_count * column_count);
    for (std::vector<int>::size_type i = 0; i < row_count; i++) {
        for (std::vector<int>::size_type j = 0; j < column_count; j++) {
            vec[i * column_count + j] = gen() % 100;
        }
    }
    return vec;
}

void getGaussKernel(double sigma) {
    double norm = 0;

    for (std::vector<int>::size_type i = 0; i < 3; i++) {
        for (std::vector<int>::size_type j = 0; j < 3; j++) {
            gauss_kernel[i * 3 + j] = exp(-(i * 3. + j) * (i * 3. + j) / (2. * sigma * sigma));
            norm += gauss_kernel[i * 3 + j];
        }
    }

    for (std::vector<int>::size_type i = 0; i < gauss_kernel.size(); i++) {
        if (norm != 0) {
            gauss_kernel[i] /= norm;
        }
    }
}

std::vector<int> getSequentialOperations(const std::vector<int>& matrix,
    std::vector<int>::size_type row_count,
    std::vector<int>::size_type column_count) {
    std::vector<int> res(matrix);
    std::vector<int> block(9, 0);
    for (std::size_t i = column_count + 1; i < row_count * column_count - column_count - 1; i++) {
        if (i % column_count != 0 && (i + 1) % column_count != 0) {
            block[0] = i - column_count - 1;
            block[1] = i - column_count;
            block[2] = i - column_count + 1;
            block[3] = i - 1;
            block[4] = i;
            block[5] = i + 1;
            block[6] = i + column_count - 1;
            block[7] = i + column_count;
            block[8] = i + column_count + 1;

            double sum = 0;
            for (std::size_t j = 0; j < 9; j++) {
                sum += static_cast<double>(matrix[block[j]]) * gauss_kernel[j];
            }
            res[i] = static_cast<int>(sum / 9);
        }
    }
    return res;
}

std::vector<int> getParallelOperations(const std::vector<int>& matrix,
    std::vector<int>::size_type matrix_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int square_root_size = 0;
    for (square_root_size = static_cast<int>(sqrt(size)); matrix_size % square_root_size != 0; square_root_size--) {}
    int new_size = square_root_size * square_root_size;

    if (new_size < 4 || matrix_size < 4) {
        return rank == 0 ? getSequentialOperations(matrix, matrix_size, matrix_size) : std::vector<int>{};
    }

    MPI_Group MPI_GROUP, MPI_ACTUAL_GROUP;
    MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP);

    std::vector<int> needed_vector(new_size);
    for (int i = 0; i < new_size; i++) {
        needed_vector[i] = i;
    }

    MPI_Comm MPI_ACTUAL_PROC;
    MPI_Group_incl(MPI_GROUP, new_size, needed_vector.data(), &MPI_ACTUAL_GROUP);
    MPI_Comm_create(MPI_COMM_WORLD, MPI_ACTUAL_GROUP, &MPI_ACTUAL_PROC);

    if (rank >= new_size) {
        return std::vector<int>{};
    }

    int blocks_count = static_cast<int>(sqrt(new_size));

    std::vector<int>::size_type local_size = matrix_size / blocks_count;
    std::vector<int>::size_type local_left_step = std::min(rank % blocks_count, 1);
    std::vector<int>::size_type local_right_step = std::min((rank + 1) % blocks_count, 1);
    std::vector<int>::size_type local_up_step = std::min(rank / blocks_count, 1);
    std::vector<int>::size_type local_down_step = rank / blocks_count == blocks_count - 1 ? 0 : 1;
    std::vector<int>::size_type row_count = local_size + local_up_step + local_down_step;
    std::vector<int>::size_type column_count = local_size + local_left_step + local_right_step;

    if (rank == 0) {
        for (int proc = 1; proc < new_size; proc++) {
            std::vector<int>::size_type step = (proc / blocks_count) * matrix_size * local_size +
                proc % blocks_count * local_size;
            std::vector<int>::size_type step_left = std::min(proc % blocks_count, 1);
            std::vector<int>::size_type step_right = std::min((proc + 1) % blocks_count, 1);
            std::vector<int>::size_type step_up = std::min(proc / blocks_count, 1);
            std::vector<int>::size_type step_down = proc / blocks_count == blocks_count - 1 ? 0 : 1;
            std::vector<int>::size_type proc_row_count = local_size + step_up + step_down;
            std::vector<int>::size_type proc_column_count = local_size + step_left + step_right;

            if (step_left == 1)
                step--;

            if (step_up == 1)
                step -= matrix_size;

            std::vector<int> block(proc_row_count * proc_column_count, 0.0);

            for (std::vector<int>::size_type i = 0; i < proc_row_count; ++i) {
                for (std::vector<int>::size_type j = 0; j < proc_column_count; ++j) {
                    std::vector<int>::size_type global_step = matrix_size * i;
                    std::vector<int>::size_type local_step = j;

                    block[i * proc_column_count + j] = matrix[step + local_step + global_step];
                }
            }

            MPI_Send(block.data(), static_cast<int>(proc_row_count * proc_column_count),
                MPI_INT, proc, 1, MPI_ACTUAL_PROC);
        }
    }

    std::vector<int> local_matrix(row_count * column_count, 0);

    if (rank == 0) {
        for (std::vector<int>::size_type i = 0; i < row_count; ++i) {
            for (std::vector<int>::size_type j = 0; j < column_count; ++j) {
                std::vector<double>::size_type global_step = matrix_size * i;
                std::vector<double>::size_type local_step = j;

                local_matrix[i * column_count + j] = matrix[local_step + global_step];
            }
        }
    } else {
        MPI_Status status;
        MPI_Recv(local_matrix.data(), static_cast<int>(row_count * column_count),
            MPI_INT, 0, 1, MPI_ACTUAL_PROC, &status);
    }

    auto new_local_matrix = getSequentialOperations(local_matrix, row_count, column_count);
    std::vector<int> local_res(matrix_size * matrix_size, 0);

    for (std::vector<int>::size_type i = local_up_step; i < row_count - local_down_step; i++) {
        for (std::vector<int>::size_type j = local_left_step; j < column_count - local_right_step; j++) {
            std::vector<double>::size_type row = (rank / blocks_count) * local_size + (i - local_up_step);
            std::vector<double>::size_type column = (rank % blocks_count) * local_size + (j - local_left_step);

            local_res[row * matrix_size + column] = new_local_matrix[i * column_count + j];
        }
    }

    std::vector<int> res(matrix_size * matrix_size, 0);
    MPI_Reduce(local_res.data(), res.data(), static_cast<int>(matrix_size * matrix_size),
        MPI_INT, MPI_SUM, 0, MPI_ACTUAL_PROC);

    MPI_Comm_free(&MPI_ACTUAL_PROC);

    return res;
}
