// Copyright 2021 Ershov Alexey
#include "./matrix_min_line_mpi.h"

#include <mpi.h>
#include <algorithm>
#include <climits>
#include <random>
#include <vector>

std::vector<double> getRandomVector(const std::vector<int>::size_type h,
                                    const std::vector<int>::size_type w) {
  std::vector<double> tmp(h * w, 0);
  std::random_device dev;
  std::mt19937 gen(dev());
  for (size_t i = 0; i < h * w; ++i) {
    tmp[i] = gen() % 100;
  }
  return tmp;
}

std::vector<double> getSequentialMatrixMinLine(
    const std::vector<double>& matrix, std::vector<double>::size_type row_count,
    std::vector<double>::size_type column_count) {
  std::vector<double> result(row_count);

  for (std::vector<double>::size_type i = 0; i < row_count; i++) {
    double local_min = matrix[i * column_count];
    for (std::vector<double>::size_type j = 0; j < column_count; j++) {
      local_min = std::min(local_min, matrix[i * column_count + j]);
    }
    result[i] = local_min;
  }
  return result;
}

std::vector<double> getParallelMatrixMinLine(
    const std::vector<double>& matrix, std::vector<double>::size_type row_count,
    std::vector<double>::size_type column_count) {
  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double>::size_type local_size = row_count / size;
  std::vector<double>::size_type remains = row_count % size;

  if (rank == 0) {
    for (int proc = 1; proc < size; ++proc) {
      int step = (local_size * proc + remains) * column_count;
      MPI_Send(matrix.data() + step,
               static_cast<double>(local_size * column_count), MPI_DOUBLE, proc,
               1, MPI_COMM_WORLD);
    }
  }

  local_size = rank == 0 ? local_size + remains : local_size;
  std::vector<double> local_matrix(local_size * column_count, 0);

  if (rank == 0) {
    for (size_t i = 0; i < local_size * column_count; i++)
      local_matrix[i] = matrix[i];
  } else {
    MPI_Status status;
    MPI_Recv(local_matrix.data(),
             static_cast<double>(local_size * column_count), MPI_DOUBLE, 0, 1,
             MPI_COMM_WORLD, &status);
  }

  std::vector<double> local_res(row_count, 0);
  std::vector<double> global_res(row_count, 0);

  auto minValuesPart =
      getSequentialMatrixMinLine(local_matrix, local_size, column_count);
  std::vector<double>::size_type startIndex =
      rank == 0 ? 0 : rank * local_size + remains;
  for (std::vector<double>::size_type i = 0; i < local_size; i++) {
    local_res[startIndex + i] = minValuesPart[i];
  }

  MPI_Reduce(local_res.data(), global_res.data(), static_cast<int>(row_count),
             MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  return global_res;
}
