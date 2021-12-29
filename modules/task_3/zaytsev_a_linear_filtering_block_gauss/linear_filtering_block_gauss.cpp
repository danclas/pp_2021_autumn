// Copyright 2021 Zaytsev Alexander
#include "../../../modules/task_3/zaytsev_a_linear_filtering_block_gauss/linear_filtering_block_gauss.h"

#include <mpi.h>

#include <random>

static vector<double> gauss_core(9, 0.000);
vector<double> RandomMatrix(int matrix_rows, int matrix_columns) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<> urd(0, 1);
  vector<double> image(matrix_rows * matrix_columns);
  for (int i = 0; i < matrix_rows; i++) {
    for (int j = 0; j < matrix_columns; j++) {
      image[i * matrix_columns + j] = urd(gen);
    }
  }
  return image;
}

void Gauss_Core(/*double sigma*/) {
  double norm = 0.0, sigma = 2.0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      gauss_core[i * 3.00 + j] =
          exp(-(i * 3.00 + j) * (i * 3.00 + j) / (2.00 * sigma * sigma));
      norm += gauss_core[i * 3 + j];
    }
  }
  for (int i = 0; i < 9; i++) {
    if ((i != 7) && (i != 4) && (i != 1)) gauss_core[i] = 0.f;
    gauss_core[i] /= norm;
  }
}

vector<double> SequentialOp(const vector<double>& image, int matrix_rows,
                            int matrix_columns) {
  vector<double> Sequential_op_resultat(image);
  vector<int> region(9, 2);
  for (int i = matrix_columns + 1;
       i < matrix_rows * matrix_columns - matrix_columns - 1; i++) {
    if (i % matrix_columns != 0 && (i + 1) % matrix_columns != 0) {
      region[0] = i - matrix_columns - 1;
      region[1] = i - matrix_columns;
      region[2] = i - matrix_columns + 1;
      region[3] = i - 1;
      region[4] = i;
      region[5] = i + 1;
      region[6] = i + matrix_columns - 1;
      region[7] = i + matrix_columns;
      region[8] = i + matrix_columns + 1;

      double Summ = 0;
      for (int j = 0; j < 9; j++) {
        Summ += image[region[j]] * gauss_core[j];
      }
      Sequential_op_resultat[i] = Summ / 9;
    }
  }
  return Sequential_op_resultat;
}
vector<double> ParallelOp(const vector<double>& image, int matrix_size) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int square_root = 0;
  for (square_root = static_cast<int>(sqrt(size));
       matrix_size % square_root != 0; square_root--) {
  }
  int block_size = square_root * square_root;
  if (block_size < 4 || matrix_size < 4) {
    return rank == 0 ? SequentialOp(image, matrix_size, matrix_size)
                     : vector<double>{};
  }
  MPI_Group BLOCKS, ACTUAL_BLOCKS;
  MPI_Comm_group(MPI_COMM_WORLD, &BLOCKS);
  vector<int> needed_vector(block_size);
  for (int i = 0; i < block_size; i++) {
    needed_vector[i] = i;
  }
  MPI_Comm ACTUAL_PROCESS;
  MPI_Group_incl(BLOCKS, block_size, needed_vector.data(), &ACTUAL_BLOCKS);
  MPI_Comm_create(MPI_COMM_WORLD, ACTUAL_BLOCKS, &ACTUAL_PROCESS);
  if (rank >= block_size) {
    return vector<double>{};
  }
  auto Blocks_Count = static_cast<int>(sqrt(block_size));
  int Local_Size = matrix_size / Blocks_Count;
  int local_left_shift = std::min(rank % Blocks_Count, 1);
  int local_right_shift = std::min((rank + 1) % Blocks_Count, 1);
  int local_up_shift = std::min(rank / Blocks_Count, 1);
  int local_down_shift = rank / Blocks_Count == Blocks_Count - 1 ? 0 : 1;
  int Rows_count = Local_Size + local_up_shift + local_down_shift;
  int Columns_count = Local_Size + local_left_shift + local_right_shift;
  if (rank == 0) {
    for (int process = 1; process < block_size; process++) {
      int shift = (process / Blocks_Count) * matrix_size * Local_Size +
                  process % Blocks_Count * Local_Size;
      int shift_left = std::min(process % Blocks_Count, 1);
      int shift_right = std::min((process + 1) % Blocks_Count, 1);
      int shift_up = std::min(process / Blocks_Count, 1);
      int shift_down = process / Blocks_Count == Blocks_Count - 1 ? 0 : 1;
      int process_row = Local_Size + shift_up + shift_down;
      int process_column = Local_Size + shift_left + shift_right;

      if (shift_left == 1) shift--;

      if (shift_up == 1) shift -= matrix_size;

      vector<double> Block(process_row * process_column, 0.0);

      for (int i = 0; i < process_row; ++i) {
        for (int j = 0; j < process_column; ++j) {
          int Global_Step = matrix_size * i;
          int Local_Step = j;

          Block[i * process_column + j] =
              image[shift + Local_Step + Global_Step];
        }
      }

      MPI_Send(Block.data(), static_cast<int>(process_row * process_column),
               MPI_DOUBLE, process, 1, ACTUAL_PROCESS);
    }
  }
  vector<double> local_image(Rows_count * Columns_count, 0);
  if (rank == 0) {
    for (int i = 0; i < Rows_count; ++i) {
      for (int j = 0; j < Columns_count; ++j) {
        int global_shift = matrix_size * i;
        int local_shift = j;

        local_image[i * Columns_count + j] = image[local_shift + global_shift];
      }
    }
  } else {
    MPI_Status My_Status;
    MPI_Recv(local_image.data(), static_cast<int>(Rows_count * Columns_count),
             MPI_DOUBLE, 0, 1, ACTUAL_PROCESS, &My_Status);
  }
  auto new_local_image = SequentialOp(local_image, Rows_count, Columns_count);
  vector<double> local_resultat(matrix_size * matrix_size, 0);
  for (int i = local_up_shift; i < Rows_count - local_down_shift; i++) {
    for (int j = local_left_shift; j < Columns_count - local_right_shift; j++) {
      int row = (rank / Blocks_Count) * Local_Size + (i - local_up_shift);
      int column = (rank % Blocks_Count) * Local_Size + (j - local_left_shift);

      local_resultat[row * matrix_size + column] =
          new_local_image[i * Columns_count + j];
    }
  }
  vector<double> Parallel_op_result(matrix_size * matrix_size, 0);
  MPI_Reduce(local_resultat.data(), Parallel_op_result.data(),
             static_cast<int>(matrix_size * matrix_size), MPI_DOUBLE, MPI_SUM,
             0, ACTUAL_PROCESS);
  MPI_Comm_free(&ACTUAL_PROCESS);
  return Parallel_op_result;
}
