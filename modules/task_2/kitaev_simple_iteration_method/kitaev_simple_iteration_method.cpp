// Copyright 2021 Kitaev Pavel

#include "../../../modules/task_2/kitaev_simple_iteration_method/kitaev_simple_iteration_method.h"
#include <mpi.h>
#include <cmath>

std::vector<double> SequentialSimpleIterationMethod(std::vector<double> matrix,
  std::vector<double> vect, double prec) {
  if (matrix.size() != vect.size() * vect.size()) {
    throw "Error size";
  }

  int matrix_size = static_cast<int>(vect.size());
  double diagonal_elem;

  double spec_prec = 0;  // Specified precision

  // Matrix transformation

  for (int i = 0; i < matrix_size; i++) {
    diagonal_elem = matrix[i * matrix_size + i];

    vect[i] /= diagonal_elem;

    for (int j = 0; j < matrix_size; j++) {
      if (i == j) {
        matrix[i * matrix_size + j] = 0;
      } else {
        matrix[i * matrix_size + j] /= diagonal_elem;
      }
    }
  }

  std::vector<double> x_prev(vect);
  std::vector<double> x_new(matrix_size);

  do {
    x_prev = x_new;
    x_new = vect;

    for (int i = 0; i < matrix_size; i++) {
      for (int j = 0; j < matrix_size; j++) {
        x_new[i] -= matrix[i * matrix_size + j] * x_prev[j];
      }

      // Calculate the current precision
      spec_prec = std::abs(x_new[i] - x_prev[i]);
    }
  } while (spec_prec >= prec);

  return x_new;
}

std::vector<double> ParallelSimpleIterationMethod(std::vector<double> matrix,
  std::vector<double> vect, double prec) {
  if (matrix.size() != vect.size() * vect.size()) {
    throw "Error size";
  }

  int matrix_size = static_cast<int>(vect.size());
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (matrix_size < ProcNum) {
    return SequentialSimpleIterationMethod(matrix, vect, prec);
  }

  std::vector<int> counts_matrix(ProcNum), counts_vect(ProcNum);
  std::vector<int> displs_matrix(ProcNum), displs_vect(ProcNum);

  int len = matrix_size / ProcNum;  // length
  int rem = matrix_size % ProcNum;  // remainder

  counts_vect[0] = len + rem;
  counts_matrix[0] = (len + rem) * matrix_size;
  displs_vect[0] = 0;
  displs_matrix[0] = 0;

  for (int i = 1; i < ProcNum; i++) {
    counts_vect[i] = len;
    counts_matrix[i] = len * matrix_size;

    displs_vect[i] = rem + len * i;
    displs_matrix[i] = (rem + len * i) * matrix_size;
  }

  std::vector<double> pers_matrix(counts_matrix[ProcRank]),
    pers_vect(counts_vect[ProcRank]);
  std::vector<double> x_new(matrix_size), x_prev(matrix_size);

  MPI_Scatterv(&matrix[0], &counts_matrix[0], &displs_matrix[0], MPI_DOUBLE,
    &pers_matrix[0], counts_matrix[ProcRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatterv(&vect[0], &counts_vect[0], &displs_vect[0], MPI_DOUBLE,
    &pers_vect[0], counts_vect[ProcRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double diagonal_elem;

  for (int i = 0; i < counts_vect[ProcRank]; i++) {
    diagonal_elem = pers_matrix[displs_vect[ProcRank] + i * matrix_size + i];

    pers_vect[i] /= diagonal_elem;

    for (int j = 0; j < matrix_size; j++) {
      if (j == displs_vect[ProcRank] + i) {
        pers_matrix[matrix_size * i + j] = 0;
      } else {
        pers_matrix[matrix_size * i + j] /= diagonal_elem;
      }
    }
  }

  MPI_Gatherv(&pers_vect[0], counts_vect[ProcRank], MPI_DOUBLE,
    &x_prev[0], &counts_vect[0], &displs_vect[0], MPI_DOUBLE, 0, MPI_COMM_WORLD);

  MPI_Bcast(&x_prev[0], matrix_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double spec_prec = 0, all_prec = 0;  // Specified precision

  do {
    for (int i = 0; i < counts_vect[ProcRank]; i++) {
      x_new[i] = pers_vect[i];

      for (int j = 0; j < matrix_size; j++) {
        x_new[i] -= pers_matrix[i * matrix_size + j] * x_prev[j];
      }

      spec_prec = std::abs(x_new[i] - x_prev[displs_vect[ProcRank] + i]);
    }

    MPI_Gatherv(&x_new[0], counts_vect[ProcRank], MPI_DOUBLE, &x_prev[0],
      &counts_vect[0], &displs_vect[0], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Bcast(&x_prev[0], matrix_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Allreduce(&spec_prec, &all_prec, 1, MPI_DOUBLE,
      MPI_MAX, MPI_COMM_WORLD);
  } while (all_prec > prec);
  return x_prev;
}
