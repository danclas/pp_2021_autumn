// Copyright 2021 Podovinnikov Artyom
#include "../../../modules/task_1/podovinnikov_a_unic_symbols_of_two_strings/unic_symbols_of_two_strings.h"

#include <mpi.h>
#include <cstring>
#include <iostream>
#include <list>
#include <random>

const char SymforGen[36] = "ABCDEFGHIJKLMOPQRSTUVWXYZ0123456789";

int randInt(int min, int max) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<unsigned> d(min, max);
  return d(rng);
}

char* randString(int len) {
  char* arr = new char[len + 1];
  for (int i = 0; i < len; i++) {
    arr[i] = SymforGen[randInt(0, 36)];
  }
  arr[len] = '\0';
  return arr;
}

int char_compare(const char c1, const char c2) {
  int r = c2 - c1;
  if (r > 0 || r < 0)
    return -1;
  else
    return 0;
  return 0;
}
int seq_compare(const char* str1, const char* str2, int size) {
  int r = 0;
  int count = 0;
  for (int i = 0; i < size; ++i) {
    r = char_compare(str1[i], str2[i]);
    if (r != 0) count++;
  }
  return count;
}
int Mpi_compare(const char* str1, const char* str2, int len) {
  int root = 0;
  int rank = 0, commSize = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // init block_size
  int block_size;
  if (len % commSize != 0) {
    block_size = len / commSize + 1;
  } else {
    block_size = len / commSize;
  }
  int result = 0;
  int local_result = 0;
  char* buffer1 = new char[block_size];
  char* buffer2 = new char[block_size];

  // send parts of a sring
  MPI_Scatter(str1, block_size, MPI_CHAR, buffer1, block_size, MPI_CHAR, root,
              MPI_COMM_WORLD);

  MPI_Scatter(str2, block_size, MPI_CHAR, buffer2, block_size, MPI_CHAR, root,
              MPI_COMM_WORLD);

  // compare strings
  if (len % block_size == 0) {
    if (rank < len / block_size) {
      local_result = seq_compare(buffer1, buffer2, block_size);
      // cout << "local res " << local_result << " rank " << rank << endl;
    }
  } else if (rank < len / block_size + 1) {
    local_result = seq_compare(buffer1, buffer2, block_size);
    // cout << "local res " << local_result << " rank " << rank << endl;
  }
  // recive parts of a sring
  MPI_Reduce(&local_result, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == root) {
    return result;
  }
  return 0;
}
