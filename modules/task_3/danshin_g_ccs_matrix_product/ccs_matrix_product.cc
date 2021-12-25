// Copyright 2021 Gleb "belgad" Danshin
#include <mpi.h>

#include <random>
#include <iostream>

#include "../../../modules/task_3/danshin_g_ccs_matrix_product/ccs_matrix_product.hh"

CCSMatrix::CCSMatrix() : row_count(0), column_count(0), values(std::vector<double>(0)), rows(std::vector<int>(0)), pointers(std::vector<int>(0)) {}

CCSMatrix::CCSMatrix(const int& row_count, const int& column_count, const std::vector<double>& values, const std::vector<int>& rows, const std::vector<int>& pointers) : row_count(row_count), column_count(column_count), values(values), rows(rows), pointers(pointers) {}

bool CCSMatrix::operator==(const CCSMatrix& other) {
  bool a = this->row_count == other.row_count
        && this->column_count == other.column_count
        && this->pointers == other.pointers
        && this->rows == other.rows
        && this->values.size() == other.values.size(),
       b = true;
  if (a)
    for (size_t i = 0; i < this->values.size() && (b &= fabs(this->values[i] - other.values[i]) < 0.00001); ++i) {}
  return (a && b);
}

CRSMatrix::CRSMatrix(const int& row_count, const int& column_count, const std::vector<double>& values, const std::vector<int>& columns, const std::vector<int>& pointers) : row_count(row_count), column_count(column_count), values(values), columns(columns), pointers(pointers) {}

CRSMatrix::CRSMatrix(const CCSMatrix& matrix) {
  this->row_count = matrix.row_count;
  this->column_count = matrix.column_count;
  this->values.resize(matrix.values.size());
  this->columns.resize(matrix.values.size());
  this->pointers.assign(this->row_count + 1, 0);
  std::vector<std::vector<double>> n_values(this->row_count);
  std::vector<std::vector<int>> n_columns(this->row_count);
  for (int i = 1; i <= matrix.column_count; ++i) {
    for (int j = matrix.pointers[i - 1]; j < matrix.pointers[i]; ++j) {
      n_values[matrix.rows[j]].push_back(matrix.values[j]);
      n_columns[matrix.rows[j]].push_back(i - 1);
    }
  }
  for (int i = 0; i < this->row_count; ++i) {
    std::copy(n_values[i].begin(), n_values[i].end(), this->values.begin() + this->pointers[i]);
    std::copy(n_columns[i].begin(), n_columns[i].end(), this->columns.begin() + this->pointers[i]);
    this->pointers[i + 1] = this->pointers[i] + n_values[i].size();
  }
}

CRSMatrix::CRSMatrix(const int& row_count, const int& column_count, const int& length) : row_count(row_count), column_count(column_count), values(length), columns(length), pointers(row_count + 1) {}

CCSMatrix* GenerateRandomMatrix(double g, int row_count, int column_count) {
  CCSMatrix* random_matrix = new CCSMatrix;
  std::random_device random_device;
  std::mt19937 random_engine(random_device());
  std::uniform_real_distribution<double> prob_distr(0.0, 1.0);
  std::uniform_real_distribution<double> values_distr(1.0, 1000.0);

  random_matrix->row_count = row_count;
  random_matrix->column_count = column_count;
  random_matrix->pointers.resize(column_count + 1);

  for (int i = 0; i < column_count;) {
    int point = 0;
    for (int j = 0; j < row_count; ++j) {
      if (prob_distr(random_engine) < g) {
        random_matrix->values.push_back(values_distr(random_engine));
        random_matrix->rows.push_back(j);
        ++point;
      }
    }
    random_matrix->pointers[i + 1] = point + random_matrix->pointers[i];
    ++i;
  }

  return random_matrix;
}

CCSMatrix* MatrixProduct(CCSMatrix* matrix1, CCSMatrix* matrix2) {
  if (matrix1 && matrix2) {
    auto tmp = CRSMatrix(*matrix1);
    auto ans = MatrixProduct(&tmp, matrix2);
    return ans;
  } else {
    return nullptr;
  }
}

CCSMatrix* MatrixProduct(CRSMatrix* matrix1, CCSMatrix* matrix2) {
  if (matrix1 && matrix2) {
    if (matrix1->column_count != matrix2->row_count)
      return nullptr;
    int row_count = matrix1->row_count;
    int column_count = matrix2->column_count;
    std::vector<double> values;
    std::vector<int> rows;
    std::vector<int> pointers {0};

    int point = 0;
    for (int i = 0; i < column_count; ++i) {
      for (int j = 0; j < row_count; ++j) {
        int border_1_left = matrix1->pointers[j];
        int border_1_right = matrix1->pointers[j + 1];
        int border_2_left = matrix2->pointers[i];
        int border_2_right = matrix2->pointers[i + 1];
        double tmp = 0.0;
        while (border_1_left < border_1_right
        && border_2_left < border_2_right) {
          if (matrix1->columns[border_1_left] == matrix2->rows[border_2_left])
            tmp += matrix1->values[border_1_left++] * matrix2->values[border_2_left++];
          while (border_1_left < border_1_right && border_2_left < border_2_right && matrix1->columns[border_1_left] < matrix2->rows[border_2_left])
            ++border_1_left;
          while (border_1_left < border_1_right && border_2_left < border_2_right && matrix2->rows[border_2_left] < matrix1->columns[border_1_left])
            ++border_2_left;
        }
        if (tmp) {
          values.push_back(tmp);
          rows.push_back(j);
          ++point;
        }
      }
      pointers.push_back(point);
    }

    return new CCSMatrix(row_count, column_count, values, rows, pointers);
  } else {
    return nullptr;
  }
}

CCSMatrix* MatrixProductParallel(CCSMatrix* matrix1, CCSMatrix* matrix2) {
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int m2_len;
  int m2_row_count, m2_column_count;
  int m1_row_count, m1_column_count, m1_length;
  if (world_rank == 0) {
    m2_row_count = matrix2->row_count;
    m2_column_count = matrix2->column_count;
    m2_len = matrix2->pointers[m2_column_count];
    m1_row_count = matrix1->row_count;
    m1_column_count = matrix1->column_count;
    m1_length = matrix1->pointers[matrix1->column_count];
  }
  MPI_Bcast(&m2_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (m2_len < 4 * world_size) {
    return MatrixProduct(matrix1, matrix2);
  }

  MPI_Bcast(&m2_row_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&m1_row_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&m1_column_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&m1_length, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int* values_send_counts = nullptr;
  int* values_send_disps = nullptr;
  int* pointers_send_counts = nullptr;
  int* column_divide_count = nullptr;
  std::vector<int> pointers_sends = {0};
  int* pointers_send_disps = nullptr;
  CRSMatrix* matrix1_to_crs = nullptr;
  if (world_rank == 0) {
    matrix1_to_crs = new CRSMatrix(*matrix1);
  } else {
    matrix1_to_crs = new CRSMatrix(m1_row_count, m1_column_count, m1_length);
  }

  MPI_Bcast(matrix1_to_crs->values.data(), m1_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(matrix1_to_crs->columns.data(), m1_length, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(matrix1_to_crs->pointers.data(), m1_row_count + 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (world_rank == 0) {
    values_send_counts = new int[world_size];
    pointers_send_counts = new int[world_size];
    column_divide_count = new int[m2_column_count];

    values_send_disps = new int[world_size + 1];
    values_send_disps[0] = 0;
    pointers_send_disps = new int[world_size];
    pointers_send_disps[0] = 0;

    std::fill(pointers_send_counts, pointers_send_counts + world_size, 1);
    std::fill(column_divide_count, column_divide_count + m2_column_count, 0);

    int v_c_a = 1 + m2_len / world_size, v_c_r = m2_len % world_size;
    for (int i = 0; i < v_c_r; ++i) {
      values_send_counts[i] = v_c_a;
    }
    --v_c_a;
    for (int i = v_c_r; i < world_size; ++i) {
      values_send_counts[i] = v_c_a;
    }

    for (int i = 1; i < world_size; ++i) {
      values_send_disps[i] = values_send_counts[i - 1] + values_send_disps[i - 1];
    }

    int v = 0, p = 0;
    int v_tmp = values_send_counts[0], p_tmp = matrix2->pointers[1];
    while (v < world_size && p < m2_column_count) {
      ++column_divide_count[p];
      ++pointers_send_counts[v];
      if (v_tmp < p_tmp) {
        // column has more elements than to send
        pointers_sends.push_back(v_tmp + pointers_sends.back());
        pointers_sends.push_back(0);
        p_tmp -= v_tmp;
        if (++v < world_size)
          v_tmp = values_send_counts[v];
      } else if (p_tmp < v_tmp) {
        // column has less elements than to send
        pointers_sends.push_back(p_tmp + pointers_sends.back());
        v_tmp -= p_tmp;
        if (++p < m2_column_count)
          p_tmp = matrix2->pointers[p + 1] - matrix2->pointers[p];
      } else {
        // column has exact number of elements to send
        pointers_sends.push_back(v_tmp + pointers_sends.back());
        pointers_sends.push_back(0);
        if (++v < world_size)
          v_tmp = values_send_counts[v];
        if (++p < m2_column_count)
          p_tmp = matrix2->pointers[p + 1] - matrix2->pointers[p];
      }
    }

    for (int i = 1; i < world_size; ++i) {
      pointers_send_disps[i] = pointers_send_disps[i - 1] + pointers_send_counts[i - 1];
    }
  }

  int values_count_atomic, pointers_count_atomic;

  MPI_Scatter(values_send_counts, 1, MPI_INT, &values_count_atomic, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(pointers_send_counts, 1, MPI_INT, &pointers_count_atomic, 1, MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<double> values_atomic(values_count_atomic);
  std::vector<int> rows_atomic(values_count_atomic);
  std::vector<int> pointers_atomic(pointers_count_atomic);

  if (world_rank == 0) {
    MPI_Scatterv(matrix2->values.data(), values_send_counts, values_send_disps, MPI_DOUBLE, values_atomic.data(), values_count_atomic, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(matrix2->rows.data(), values_send_counts, values_send_disps, MPI_INT, rows_atomic.data(), values_count_atomic, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(pointers_sends.data(), pointers_send_counts, pointers_send_disps, MPI_INT, pointers_atomic.data(), pointers_count_atomic, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatterv(nullptr, nullptr, nullptr, MPI_DOUBLE, values_atomic.data(), values_count_atomic, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(nullptr, nullptr, nullptr, MPI_INT, rows_atomic.data(), values_count_atomic, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(nullptr, nullptr, nullptr, MPI_INT, pointers_atomic.data(), pointers_count_atomic, MPI_INT, 0, MPI_COMM_WORLD);
  }

  CCSMatrix* matrix2_atomic = new CCSMatrix(m2_row_count, pointers_count_atomic - 1, values_atomic, rows_atomic, pointers_atomic);

  CCSMatrix* answer_atomic = MatrixProduct(matrix1_to_crs, matrix2_atomic);

  int answer_atomic_length = answer_atomic->values.size();

  MPI_Gather(&answer_atomic_length, 1, MPI_INT, values_send_counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

  double* answer_all_values = nullptr;
  int* answer_all_rows = nullptr;
  double* answer_values_column = nullptr;

  if (world_rank == 0) {
    for (int i = 1; i <= world_size; ++i) {
      values_send_disps[i] = values_send_disps[i - 1] + values_send_counts[i - 1];
    }
    answer_all_values = new double[values_send_disps[world_size]];
    answer_all_rows = new int[values_send_disps[world_size]];
    answer_values_column = new double[m1_row_count];
  }

  MPI_Gatherv(answer_atomic->values.data(), answer_atomic_length, MPI_DOUBLE, answer_all_values, values_send_counts, values_send_disps, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Gatherv(answer_atomic->rows.data(), answer_atomic_length, MPI_INT, answer_all_rows, values_send_counts, values_send_disps, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Gatherv(answer_atomic->pointers.data(), pointers_count_atomic, MPI_INT, pointers_sends.data(), pointers_send_counts, pointers_send_disps, MPI_INT, 0, MPI_COMM_WORLD);

  delete matrix1_to_crs;
  delete matrix2_atomic;

  if (world_rank == 0) {
    std::vector<double> answer_values;
    std::vector<int> answer_rows;
    std::vector<int> answer_pointers;
    answer_pointers.assign(m2_column_count + 1, 0);
    int p = 1, c = 0, v = 0;
    bool check = false;
    std::fill(answer_values_column, answer_values_column + m1_row_count, 0.0);
    for (int i = 0; i < world_size; ++i, ++p) {
        for (int j = 1; j < pointers_send_counts[i]; ++j, ++p) {
            for (int k = pointers_sends[p - 1]; k < pointers_sends[p]; ++k, ++v) {
                answer_values_column[answer_all_rows[v]] += answer_all_values[v];
                check = true;
            }
            --column_divide_count[c];
            if (column_divide_count[c] == 0) {
                ++c;
                answer_pointers[c] = answer_pointers[c - 1];
                if (check) {
                  for (int t = 0; t < m1_row_count; ++t) {
                      if (answer_values_column[t] != 0.0) {
                          answer_values.push_back(answer_values_column[t]);
                          answer_rows.push_back(t);
                          ++answer_pointers[c];
                      }
                  }
                  std::fill(answer_values_column, answer_values_column + m1_row_count, 0.0);
                  check = false;
                }
            }
        }
    }

    for (; c < m2_column_count; ++c) {
      answer_pointers[c + 1] = answer_pointers[c];
    }

    delete [] values_send_counts;
    delete [] pointers_send_counts;
    delete [] column_divide_count;
    delete [] values_send_disps;
    delete [] pointers_send_disps;
    delete [] answer_all_values;
    delete [] answer_all_rows;
    delete [] answer_values_column;

    return new CCSMatrix(m1_row_count, m2_column_count, answer_values, answer_rows, answer_pointers);
  } else {
    return nullptr;
  }
}
