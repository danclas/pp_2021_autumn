// Copyright 2021 Lukashuk Diana
#include "../../../modules/task_2/lukashuk_d_gaus_horizon/gauss_horizontal.h"

std::vector<double> getRandomMatrix(int m, int n, int proc_rank,
                                    int proc_count) {
  int size = m * n;
  std::vector<double> matrix(size);
  if (proc_rank == 0) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::uniform_int_distribution<unsigned int> uid(1, 100);
    for (int i = 0; i < size; i++) {
      matrix[i] = uid(gen);
    }
  }
  if (proc_count > 1) {
    MPI_Bcast(matrix.data(), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }
  return matrix;
}

std::vector<double> seqGauss(const std::vector<double>& matrix, int rows,
                             int cols) {
  if (rows < 0 || cols < 0) {
    throw "Correct the input data";
  }
  std::vector<double> n(matrix);
  for (int i = 0; i < rows; i++) {
    for (int j = i + 1; j < rows; j++) {
      double coeff = n[i * cols + i] / n[j * cols + i];
      for (int k = i; k < cols; k++)
        n[j * cols + k] = n[j * cols + k] * coeff - n[i * cols + k];
    }
  }
  std::vector<double> result(rows);
  for (int i = rows - 1; i >= 0; i--) {
    double coeff = n[i * cols + cols - 1];
    for (int j = rows - 1; j >= i + 1; j--)
      coeff -= n[i * cols + j] * result[j];
    result[i] = coeff / n[i * cols + i];
  }
  return result;
}

std::vector<double> parGauss(const std::vector<double>& matrix, int rows,
                             int cols) {
  if (rows < 0 || cols < 0) {
    throw "Correct the input data";
  }
  int proc_count, proc_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

  int size = rows / proc_count;
  int remains = rows % proc_count;

  int n = size;
  if (proc_rank < remains) {
    n++;
  }
  std::vector<double> local_result(n * cols);

  if (local_result.size() == 0) {
    local_result.resize(1);
  }
  std::vector<int> scount(proc_count);
  std::vector<int> displs(proc_count);

  displs[0] = 0;
  for (int i = 0; i < proc_count; i++) {
    if (i < (rows % proc_count)) {
      scount[i] = (size + 1) * cols;
    } else {
      scount[i] = size * cols;
    }
    if (i > 0) {
      displs[i] = (displs[i - 1] + scount[i - 1]);
    }
  }

  MPI_Scatterv(matrix.data(), scount.data(), displs.data(), MPI_DOUBLE,
               local_result.data(), n * cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  std::vector<double> piw_row(cols);

  for (int i = 0; i < displs[proc_rank] / cols; i++) {
    int root = 0;
    int sum = 0;
    for (int j = 0; j < proc_count; j++, root++) {
      sum += scount[j] / cols;
      if (i < sum) {
        root = j;
        break;
      }
    }
    MPI_Bcast(piw_row.data(), cols, MPI_DOUBLE, root, MPI_COMM_WORLD);

    for (int j = 0; j < scount[proc_rank] / cols; j++) {
      double s = piw_row[i] / local_result[j * cols + i];
      for (int tmp = i; tmp < cols; tmp++) {
        local_result[j * cols + tmp] =
            s * local_result[j * cols + tmp] - piw_row[tmp];
      }
    }
  }

  for (int i = 0; i < scount[proc_rank] / cols; i++) {
    for (int j = 0; j < cols; j++) {
      piw_row[j] = local_result[i * cols + j];
    }
    MPI_Bcast(piw_row.data(), cols, MPI_DOUBLE, proc_rank, MPI_COMM_WORLD);
    for (int j = i + 1; j < scount[proc_rank] / cols; j++) {
      double s = piw_row[displs[proc_rank] / cols + i] /
                 local_result[j * cols + i + displs[proc_rank] / cols];
      for (int tmp = i + displs[proc_rank] / cols; tmp < cols; tmp++) {
        local_result[j * cols + tmp] =
            s * local_result[j * cols + tmp] - piw_row[tmp];
      }
    }
  }

  int root = 0;
  for (int i = displs[proc_rank] / cols + scount[proc_rank] / cols; i < rows;
       i++) {
    int sum = 0;
    for (int j = 0; j < proc_count; j++, root++) {
      sum += scount[j] / cols;
      if (i < sum) {
        root = j;
        break;
      }
    }
    MPI_Bcast(piw_row.data(), cols, MPI_DOUBLE, root, MPI_COMM_WORLD);
  }

  std::vector<double> res(0);
  if (proc_rank == 0) res.resize(rows * cols);
  MPI_Gatherv(local_result.data(), n * cols, MPI_DOUBLE, res.data(),
              scount.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  std::vector<double> result(0);
  if (proc_rank == 0) {
    result.resize(rows);
    for (int i = rows - 1; i >= 0; --i) {
      double coeff = res[i * cols + cols - 1];
      for (int j = rows - 1; j >= i + 1; --j)
        coeff -= res[i * cols + j] * result[j];
      result[i] = coeff / res[i * cols + i];
    }
  }

  return result;
}
