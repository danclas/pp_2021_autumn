// Copyright 2021 Votyakova Daria

#include "../../../modules/task_2/votyakova_d_jacoby_method/jacoby_method.h"

#include <mpi.h>

#include <random>
#include <vector>

std::vector<double> getDiagonallyDominantMatrix(int size) {
  if (size <= 0) {
    return std::vector<double>(0);
  }

  std::random_device rd;
  std::mt19937 gen(rd());

  std::vector<double> A(size * size);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (i != j) {
        A[i * size + j] =
            static_cast<double>(static_cast<int>(gen() % 1000) - 500);
      }
    }
  }

  for (int i = 0; i < size; i++) {
    double abs_sum = 0;
    for (int j = 0; j < size; j++) {
      if (i != j) abs_sum += std::fabs(A[i * size + j]);
    }
    A[i * size + i] = abs_sum + static_cast<double>(gen() % 500);
  }
  return A;
}

std::vector<double> getB(int size) {
  if (size <= 0) {
    return std::vector<double>();
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<double> b(size);

  for (int i = 0; i < size; i++) {
    b[i] = static_cast<double>(static_cast<int>(gen() % 1000) - 500);
  }
  return b;
}

double getError(std::vector<double> A, std::vector<double> x,
                std::vector<double> b) {
  int size = static_cast<int>(x.size());
  std::vector<double> Ax(size, 0.);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      Ax[i] += A[i * size + j] * x[j];
    }
  }

  double error = 0.;
  for (unsigned int i = 0; i < Ax.size(); i++) {
    double dist = std::fabs(Ax[i] - b[i]);
    if (dist > error) {
      error = dist;
    }
  }

  return error;
}

std::vector<double> getJacobiSequential(std::vector<double> A,
                                        std::vector<double> b, int size) {
  int iteration = 0;

  std::vector<double> x(size);
  std::vector<double> x_prev(b);
  double error = 0.;
  do {
    for (int i = 0; i < size; i++) {
      x[i] = b[i];
      for (int j = 0; j < size; j++)
        if (i != j) x[i] -= A[i * size + j] * x_prev[j];
      x[i] /= A[i * size + i];
    }
    for (int i = 0; i < size; i++) x_prev[i] = x[i];
    iteration++;
    error = getError(A, x, b);
  } while (error > eps && iteration < max_iterations);
  return x;
}

std::vector<double> getJacobiParallel(std::vector<double> A,
                                      std::vector<double> b, int size) {
  int old_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &old_rank);
  int old_world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &old_world_size);

  MPI_Comm comm;
  if (old_rank < size) {
    MPI_Comm_split(MPI_COMM_WORLD, 0, old_rank, &comm);
  } else {
    MPI_Comm_split(MPI_COMM_WORLD, MPI_UNDEFINED, old_rank, &comm);
  }

  if (comm != MPI_COMM_NULL) {
    int rank;
    MPI_Comm_rank(comm, &rank);
    int world_size;
    MPI_Comm_size(comm, &world_size);

    int *b_counts = nullptr, *b_displs = nullptr, *A_counts = nullptr,
        *A_displs = nullptr;

    if (rank == 0) {
      int delta = size / world_size;
      int remainder = size % world_size;

      b_counts = new int[world_size];
      b_displs = new int[world_size];
      A_counts = new int[world_size];
      A_displs = new int[world_size];

      for (int i = 0; i < world_size; i++) {
        b_counts[i] = delta;
        b_displs[i] = 0;
      }
      for (int i = 0; i < remainder; i++) {
        b_counts[i]++;
      }
      for (int i = 1; i < world_size; i++) {
        b_displs[i] += b_displs[i - 1] + b_counts[i - 1];
      }
      for (int i = 0; i < world_size; i++) {
        A_counts[i] = b_counts[i] * size;
        A_displs[i] = b_displs[i] * size;
      }
    }

    double error = 0.;
    int iteration = 0;

    int local_size;
    MPI_Scatter(b_counts, 1, MPI_INT, &local_size, 1, MPI_INT, 0, comm);

    std::vector<double> global_x;
    std::vector<double> local_x(local_size);
    std::vector<double> prev_x(size);

    if (old_rank == 0) {
      global_x.resize(size);
      prev_x = b;
    }

    std::vector<double> local_b(local_size);
    MPI_Scatterv(b.data(), b_counts, b_displs, MPI_DOUBLE, local_b.data(),
                 local_size, MPI_DOUBLE, 0, comm);

    std::vector<double> A_local(local_size * size);
    MPI_Scatterv(A.data(), A_counts, A_displs, MPI_DOUBLE, A_local.data(),
                 local_size * size, MPI_DOUBLE, 0, comm);

    int row_index;
    MPI_Scatter(b_displs, 1, MPI_INT, &row_index, 1, MPI_INT, 0, comm);

    do {
      MPI_Bcast(prev_x.data(), size, MPI_DOUBLE, 0, comm);

      for (int i = 0; i < local_size; i++) {
        local_x[i] = local_b[i];
        for (int j = 0; j < size; j++) {
          if (j != row_index + i) {
            local_x[i] -= A_local[i * size + j] * prev_x[j];
          }
        }
        local_x[i] /= A_local[i * size + row_index + i];
      }
      iteration++;

      MPI_Gatherv(local_x.data(), local_size, MPI_DOUBLE, global_x.data(),
                  b_counts, b_displs, MPI_DOUBLE, 0, comm);

      if (old_rank == 0) {
        for (int i = 0; i < size; i++) {
          prev_x[i] = global_x[i];
        }
        error = getError(A, global_x, b);
      }

      MPI_Bcast(&error, 1, MPI_DOUBLE, 0, comm);
    } while (error > eps && iteration < max_iterations);

    MPI_Comm_free(&comm);
    return global_x;
  } else {
    return std::vector<double>();
  }
}
