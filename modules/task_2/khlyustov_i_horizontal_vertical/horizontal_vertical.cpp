// Copyright 2021 Khlyustov Ilya
#include <random>
#include <algorithm>
#include "../../../modules/task_2/khlyustov_i_horizontal_vertical/horizontal_vertical.h"

Matrix getRandomMatrix(sizeType row_count, sizeType col_count) {
    std::random_device dev;
    std::mt19937 gen(dev());
    Matrix mtrx(row_count * col_count);
    for (sizeType row = 0; row < row_count; row++) {
        for (sizeType col = 0; col < col_count; col++) {
            mtrx[row * col_count + col] = gen() % 1000;
        }
    }
    return mtrx;
}

Matrix getSequential(const Matrix& mtrx1, const Matrix& mtrx2,
    sizeType row_mtrx1, sizeType col_mtrx1, sizeType col_mtrx2) {

    sizeType row_mtrx2 = col_mtrx1;
    sizeType res_procCount = row_mtrx1 * col_mtrx2;
    Matrix result(res_procCount, 0);

    for (sizeType i = 0; i < row_mtrx1; i++) {
        for (sizeType j = 0; j < col_mtrx2; j++) {
            for (sizeType k = 0; k < col_mtrx1; k++) {
                result[i * col_mtrx2 + j] += mtrx1[i * row_mtrx2 + k] * mtrx2[k * col_mtrx2 + j];
            }
        }
    }
    return result;
}

Matrix getParallel(const Matrix& mtrx1, const Matrix& mtrx2,
    sizeType row_mtrx1, sizeType col_mtrx1) {

    int proc_count, proc_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_count > static_cast<int>(row_mtrx1) || proc_count == 1) {
        return proc_rank == 0 ? getSequential(mtrx1, mtrx2, row_mtrx1, col_mtrx1, row_mtrx1) : Matrix{};
    }
    sizeType row_mtrx2 = col_mtrx1;
    sizeType col_mtrx2 = row_mtrx1;
    sizeType local_count = row_mtrx1 / proc_count;
    sizeType remainder = row_mtrx1 % proc_count;

    if (proc_rank == 0) {
        for (int proc = 1; proc < proc_count; proc++) {
            sizeType step_mtrx2 = proc * local_count + remainder;

            Matrix send_vec_b(local_count * row_mtrx2, 0);
            for (sizeType i = 0; i < send_vec_b.size(); i++) {
                send_vec_b[i] = mtrx2[step_mtrx2 + (i % local_count) + (i / local_count) * col_mtrx2];
            }
            MPI_Send(mtrx1.data() + step_mtrx2 * col_mtrx1, static_cast<int>(local_count * col_mtrx1),
                MPI_INT, proc, 1, MPI_COMM_WORLD);
            MPI_Send(send_vec_b.data(), static_cast<int>(send_vec_b.size()),
                MPI_INT, proc, 2, MPI_COMM_WORLD);
        }
    }
    sizeType local_size = (local_count + remainder) * col_mtrx1;
    Matrix local_vec_a(local_size, 0);
    Matrix local_vec_b(local_size + 1, 0);

    if (proc_rank == 0) {
        for (sizeType i = 0; i < local_size; i++) {
            local_vec_b[i] = mtrx2[(i % (local_count + remainder)) +
                (i / (local_count + remainder)) * col_mtrx2];
            local_vec_a[i] = mtrx1[i];
        }
    } else {
        int tmp = static_cast<int>(local_size - remainder * col_mtrx1);
        MPI_Recv(local_vec_a.data(), tmp, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(local_vec_b.data(), tmp, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    local_vec_b[local_size] = proc_rank == 0 ? 0 : proc_rank * local_count + remainder;
    Matrix local_result(row_mtrx1 * col_mtrx2, 0);

    for (int i = 0; i < proc_count; i++) {
        sizeType local_row_mtrx1 = local_count + (proc_rank == 0 ? remainder : 0);
        sizeType local_col_mtrx2 = local_count + ((proc_rank + i) % proc_count == 0 ? remainder : 0);
        Matrix tmp_res = getSequential(local_vec_a, local_vec_b,
            local_row_mtrx1, col_mtrx1, local_col_mtrx2);

        sizeType step_mtrx = proc_rank == 0 ? 0 : (proc_rank * local_count + remainder) * row_mtrx1;
        for (sizeType j = 0; j < tmp_res.size(); j++) {
            sizeType step_row = local_vec_b[local_size] + j % local_col_mtrx2;
            sizeType step_col = (j / local_col_mtrx2) * row_mtrx1;
            local_result[step_mtrx + step_row + step_col] += tmp_res[j];
        }
        MPI_Send(local_vec_b.data(), static_cast<int>(local_vec_b.size()),
            MPI_INT, ((proc_rank < 1) ? proc_count - 1 : proc_rank - 1), i, MPI_COMM_WORLD);
        MPI_Recv(local_vec_b.data(), static_cast<int>(local_vec_b.size()),
            MPI_INT, ((proc_rank + 1) % proc_count), i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    Matrix result(row_mtrx1 * col_mtrx2, 0);
    MPI_Reduce(local_result.data(), result.data(), static_cast<int>(local_result.size()),
        MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return result;
}
