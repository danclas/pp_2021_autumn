// Copyright 2021 Cheremushkin Kirill
#ifndef MODULES_TASK_2_CHEREMUSHKIN_K_RESHETKA_TOR_RESHETKA_TOR_H_
#define MODULES_TASK_2_CHEREMUSHKIN_K_RESHETKA_TOR_RESHETKA_TOR_H_
#include <mpi.h>

MPI_Comm CreateTor(int width, int height);

bool ExaminationTor(const MPI_Comm grid_torus_comm, const int exp_dims_count, const int* exp_dims_vals);

#endif  // MODULES_TASK_2_CHEREMUSHKIN_K_RESHETKA_TOR_RESHETKA_TOR_H_
