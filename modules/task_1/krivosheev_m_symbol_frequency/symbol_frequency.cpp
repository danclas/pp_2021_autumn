// Copyright 2021 Krivosheev Miron

#include "G:/GitHubTasks/Paralell_programming/pp_2021_autumn/modules/task_1/krivosheev_m_symbol_frequency/symbol_frequency.h"

double getFrequencyParallel(char s, std::string text) {
  int rank, tasks, answ_count = 0, res_count = 0, count = 0;
  double freq = 0.0;
  MPI_Comm_size(MPI_COMM_WORLD, &tasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = text.length();
  int data_per_rank = n / tasks;
  if (rank == tasks - 1) {
    for (int i = (rank - 1) * data_per_rank; i < n; ++i)
      if (text[i] == static_cast<char>(s) ||
        text[i] == static_cast<char>(s - 32))
        count++;
    res_count = count;
  } else if (rank != 0) {
    for (int i = (rank - 1) * data_per_rank; i < rank * data_per_rank; ++i)
      if (text[i] == static_cast<char>(s) ||
        text[i] == static_cast<char>(s - 32))
        count++;
    res_count = count;
  }
  MPI_Reduce(&res_count, &answ_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    freq = static_cast<double>(answ_count) / n;
    return freq;
  }
  return 0;
}

double getFrequencyNonParallel(char s, std::string text) {
  int n = text.length();
  int count = 0;
  double freq;
  for (int i = 0; i < n; ++i)
    if (text[i] == static_cast<char>(s)
      || text[i] == static_cast<char>(s - 32))
      count++;
  freq = static_cast<double>(count) / n;
  return freq;
}

