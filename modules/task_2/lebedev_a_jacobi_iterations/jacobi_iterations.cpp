// Copyright 2021 Lebedev Alexey
#include "../../../modules/task_2/lebedev_a_jacobi_iterations/jacobi_iterations.h"

#include <mpi.h>
#include <math.h>

#include <cstring>


float dist(const Tensor<float>& t1, const Tensor<float>& t2) {
    if (t1.get_shape() != t2.get_shape()) {
        throw std::logic_error("Can't measure distance beetwen tensor with different shapes!");
    }
    float d = 0;
    for (int i = 0; i < t1.get_size(); i++) {
        d += pow(t1[i] - t2[i], 2);
    }
    return sqrt(d);
}


Tensor<float> LinearSystem::solve(const float accuracy) {
    Tensor<float> B(A.get_shape());
    Tensor<float> d(b.get_shape());

    std::memcpy(B.get_data(), A.get_data(), A.get_size() * sizeof(float));
    std::memcpy(d.get_data(), b.get_data(), b.get_size() * sizeof(float));

    std::vector<int> shape = B.get_shape();
    std::vector<int> strides = B.get_strides();
    // normalize A -> B, b -> d
    for (int i = 0; i < shape[0]; i++) {
        int diag_idx = i * strides[0] + i * strides[1];
        float a_ii = B[diag_idx];
        B[diag_idx] = 0;
        d[i] /= a_ii;
        for (int j = 0; j < shape[1]; j++) {
            B[i * strides[0] + j * strides[1]] *= -1 / a_ii;
        }
    }

    Tensor<float> current_x(x0.get_shape());
    std::memcpy(current_x.get_data(), x0.get_data(), x0.get_size() * sizeof(float));
    Tensor<float> x;
    float diff;
    do {
        x = matmul2D(B, current_x) + d;
        diff = dist(x, current_x);
        current_x = x;
    } while (diff > accuracy);
    return x;
}


Tensor<float> solve_parallel(const LinearSystem& sys, const float accuracy) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        size = std::min(size, static_cast<int>(sys.n_dims));
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Comm NEW_WORLD;
    int color = (rank < size) ? 0 : MPI_UNDEFINED;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &NEW_WORLD);
    if (NEW_WORLD == MPI_COMM_NULL) {
        return Tensor<float>();
    }

    int d1, d2;
    if (rank == 0) {
        d2 = sys.n_dims;
    }

    MPI_Barrier(NEW_WORLD);
    MPI_Bcast(&d2, 1, MPI_INT, 0, NEW_WORLD);

    Tensor<float> B_local;
    Tensor<float> d_local;
    Tensor<float> x({d2, 1});

    if (rank == 0) {
        std::memcpy(x.get_data(), sys.x0.get_data(), x.get_size() * sizeof(float));
    }

    MPI_Barrier(NEW_WORLD);
    MPI_Bcast(x.get_data(), x.get_size(), MPI_FLOAT, 0, NEW_WORLD);

    int diag = 0;
    std::vector<int> recvcounts(0);
    std::vector<int> displs(0);

    if (rank == 0) {
        recvcounts.resize(size);
        displs.resize(size);

        int row_per_process = sys.n_dims / size;
        int last_rows = sys.n_dims % size;

        d1 = (last_rows == 0) ? row_per_process : row_per_process + 1;
        recvcounts[0] = d1;
        displs[0] = 0;
        int _diag_idx = d1;
        B_local.resize({d1, d2});
        d_local.resize({d1, 1});

        std::memcpy(B_local.get_data(), sys.A.get_data(), B_local.get_size() * sizeof(float));
        std::memcpy(d_local.get_data(), sys.b.get_data(), d_local.get_size() * sizeof(float));

        int offset_A = B_local.get_size();
        int offset_b = d_local.get_size();
        for (int pid = 1; pid < size; pid++) {
            int _d1 = (pid < last_rows) ? row_per_process + 1 : row_per_process;
            int count = _d1 * d2;
            MPI_Send(&_d1, 1, MPI_INT, pid, 0, NEW_WORLD);
            MPI_Send(sys.A.get_data() + offset_A, count, MPI_FLOAT, pid, 1, NEW_WORLD);
            MPI_Send(sys.b.get_data() + offset_b, _d1, MPI_FLOAT, pid, 2, NEW_WORLD);
            MPI_Send(&_diag_idx, 1, MPI_INT, pid, 3, NEW_WORLD);
            recvcounts[pid] = _d1;
            displs[pid] = displs[pid - 1] + recvcounts[pid - 1];
            offset_A += count;
            offset_b += _d1;
            _diag_idx += _d1;
        }
    } else {
        MPI_Status status;
        MPI_Recv(&d1, 1, MPI_INT, 0, 0, NEW_WORLD, &status);
        B_local.resize({d1, d2});
        d_local.resize({d1, 1});
        MPI_Recv(B_local.get_data(), B_local.get_size(), MPI_FLOAT, 0, 1, NEW_WORLD, &status);
        MPI_Recv(d_local.get_data(), d_local.get_size(), MPI_FLOAT, 0, 2, NEW_WORLD, &status);
        MPI_Recv(&diag, 1, MPI_INT, 0, 3, NEW_WORLD, &status);
    }

    int stride1 = B_local.get_strides()[0], stride2 = B_local.get_strides()[1];
    // normalize A -> B, b -> d
    for (int i = 0; i < d1; i++) {
        int diag_idx = i * stride1 + i * stride2 + diag;
        float a_ii = B_local[diag_idx];
        d_local[i] /= a_ii;
        B_local[diag_idx] = 0;
        for (int j = 0; j < d2; j++) {
            B_local[i * stride1 + j * stride2] *= - 1 / a_ii;
        }
    }

    Tensor<float> x_tmp;
    if (rank == 0) {
        x_tmp.resize(x.get_shape());
    }
    uint8_t solved = 0;
    while (!solved) {
        Tensor<float> x_local = matmul2D(B_local, x) + d_local;
        MPI_Gatherv(x_local.get_data(), x_local.get_size(), MPI_FLOAT,
                    x_tmp.get_data(), recvcounts.data(), displs.data(),
                    MPI_FLOAT, 0, NEW_WORLD);
        if (rank == 0) {
            float diff = dist(x_tmp, x);
            if (diff <= accuracy) {
                solved = 1;
            }
            std::memcpy(x.get_data(), x_tmp.get_data(), x_tmp.get_size() * sizeof(float));
        }
        MPI_Barrier(NEW_WORLD);
        MPI_Bcast(&solved, 1, MPI_UNSIGNED_CHAR, 0, NEW_WORLD);
        MPI_Bcast(x.get_data(), x.get_size(), MPI_FLOAT, 0, NEW_WORLD);
    }

    MPI_Comm_free(&NEW_WORLD);

    return x;
}
