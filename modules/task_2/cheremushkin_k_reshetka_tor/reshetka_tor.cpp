// Copyright 2021 Chremushkin Kirill
#include "../../../modules/task_2/cheremushkin_k_reshetka_tor/reshetka_tor.h"

MPI_Comm CreateTor(int width, int height) {  // Cozdanie reshetki tor
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);  // Poluchenie razmera
  if (height > 0 && width > 0 && (height * width > ProcNum)) {
    return MPI_COMM_NULL;
  }
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);  // Poluchenie ranka 

  int rSize[2] = {width, height};  // Razmernost

  MPI_Comm tor_comm;
  int periods[2] = {1, 1};  // Period
  MPI_Cart_create(MPI_COMM_WORLD, 2, rSize, periods, false, &tor_comm);  // Sozdanie dekartovoi reshetki

  MPI_Comm_rank(tor_comm, &ProcRank);  // Poluchenie Ranka
  int coordinates[2];
  MPI_Cart_coords(tor_comm, ProcRank, 2, coordinates);  // Poluchenie coordinat tekushego proccesa
  return tor_comm;
}

bool ExaminationTor(const MPI_Comm tor_comm, const int expd_count, const int* expd_vals) {  // Proverka reshetki
  int t_type = 0;
  MPI_Topo_test(tor_comm, &t_type);  // Poluchenie tipa topologii
  if (t_type != MPI_CART) {  // Proverka tipa
    return false;
  }

  int reald_count;
  MPI_Cartdim_get(tor_comm, &reald_count); // Poluchenie razmernosti
  if (reald_count != expd_count) {
    return false;
  } else {
    int* t_dims = new int[reald_count];  // Razmernost 
    int* t_periods = new int[reald_count];  // Periodi
    int* p_coords = new int[reald_count];  // Coordinati

    MPI_Cart_get(tor_comm, 2, t_dims, t_periods, p_coords); // Periodi, razmeri A on B

    for (int i = 0; i < reald_count; i++) {
      if ((t_dims[i] != expd_vals[i]) ||
          (t_periods[i] != 1)) {
        return false;
      }
    }
  }

  return true;
}
