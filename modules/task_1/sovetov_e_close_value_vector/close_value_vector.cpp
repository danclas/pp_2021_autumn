// Copyright 2021 Sovetov Egor
#include "../../../modules/task_1/sovetov_e_close_value_vector/close_value_vector.h"

#include <mpi.h>

#include <algorithm>
#include <climits>
#include <cmath>
#include <random>
#include <vector>

std::vector<int> getRandomVector(int raz) {
  std::random_device ran;
  std::vector<int> wektor(raz);
  std::mt19937 engine(ran());
  for (int j = 0; j < raz; j += 1) {
    wektor[j] = engine() % 100;
  }
  return wektor;
}

int getMinDifferenceOneProc(const std::vector<int> wektor) {
  int minDifference = INT_MAX;
  if (wektor.size() != 0) {
    for (int j = 0; j < static_cast<int>(wektor.size()) - 1; j += 1) {
      if (minDifference > abs(wektor[j] - wektor[j + 1]))
        minDifference = abs(wektor[j] - wektor[j + 1]);
    }
  }
  if (wektor.size() == 1) {
    return 0;
  }
  return minDifference;
}

int getMinDifference(const std::vector<int> wektor) {
  int minDifference = INT_MAX;
  int raz, bee;
  MPI_Comm_size(MPI_COMM_WORLD, &raz);
  MPI_Comm_rank(MPI_COMM_WORLD, &bee);
  MPI_Status status;

  int moduRaz = wektor.size() - 1;
  int moduTR = moduRaz / raz;
  int result = moduRaz % raz;

  if (bee == 0) {
    if (moduTR > 0) {
      for (int ji = 1; ji < raz; ji += 1)
        MPI_Send(&wektor[result] + moduTR * ji, moduTR + 1, MPI_INT, ji, 0,
                 MPI_COMM_WORLD);
    }
  } else {
    if (moduTR > 0) {
      std::vector<int> vam(moduTR + 1);
      MPI_Recv(&vam[0], moduTR + 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);
      int minDifference = getMinDifferenceOneProc(vam);
      MPI_Send(&minDifference, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }
  if (bee == 0) {
    int restji;
    std::vector<int> vam(wektor.begin(), wektor.begin() + 1 + moduTR + result);
    minDifference = getMinDifferenceOneProc(vam);
    if (moduTR > 0) {
      for (int ji = 1; ji < raz; ji += 1) {
        MPI_Recv(&restji, 1, MPI_INT, ji, 0, MPI_COMM_WORLD, &status);
        minDifference = std::min(minDifference, restji);
      }
    }
  }
  return minDifference;
}
