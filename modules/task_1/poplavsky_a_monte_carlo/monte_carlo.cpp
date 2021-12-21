// Copyright 2021 Poplavsky Anton
#include "../../../modules/task_1/poplavsky_a_monte_carlo/monte_carlo.h"

double MonteCarlo(double low, double up, int points, double (*func)(double)) {
  if (points <= 0) {
    throw "ERROR";
  }

  int ProcRank, ProcNum;
  double integration = 0;
  double monte = 0;
  double delta;
  double count;
  double loc_up;
  double loc_low;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  delta = (up - low) / ProcNum;
  count = points / ProcNum;
  loc_low = low + delta * ProcRank;
  if (ProcRank == ProcNum - 1) {
    loc_up = up;
  } else {
    loc_up = loc_low + delta;
  }

  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<double> urd(loc_low, loc_up);
  for (int i = 0; i < count; i++) {
    integration += func(urd(gen));
  }

  MPI_Reduce(&integration, &monte, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  return (monte * (up - low) / points);
}
