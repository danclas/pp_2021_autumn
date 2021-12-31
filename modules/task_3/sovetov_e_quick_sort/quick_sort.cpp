// Copyright 2021 Sovetov Egor
#include <mpi.h>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

#include "../../../modules/task_3/sovetov_e_quick_sort/quick_sort.h"

std::vector<int> GetWektorRandom(int razmerWektor) {
  if (razmerWektor < 1) {
    throw "ErrorDlin";
  }
  std::mt19937 genepi;
  genepi.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> wektor(razmerWektor);
  for (int number = 0; number < razmerWektor; number++) {
    wektor[number] = genepi() % 100000000;
  }
  return wektor;
}

bool SortWektor(const std::vector<int> wektor) {
  for (uint32_t number = 0; number < wektor.size() - 1; number++)
    if (wektor[number] > wektor[number + 1]) return false;
  return true;
}

void QuickSortNO_MPI(std::vector<int>* wektor, int levy, int prav) {
  if (wektor->size() < 1) {
    throw "ErrorDLin";
  }
  int lev = levy, pra = prav;
  int root = (*wektor)[(levy + prav) / 2];
  int trut;
  do {
    while ((*wektor)[lev] < root) lev++;
    while ((*wektor)[pra] > root) pra--;
    if (lev <= pra) {
      trut = (*wektor)[lev];
      (*wektor)[lev] = (*wektor)[pra];
      (*wektor)[pra] = trut;
      lev++;
      pra--;
    }
  } while (lev <= pra);

  if (lev < prav) {
    QuickSortNO_MPI(&(*wektor), lev, prav);
  }
  if (levy < pra) {
    QuickSortNO_MPI(&(*wektor), levy, pra);
  }
}

void QuickSortMPI(std::vector<int>* wektor) {
  int razmerWektor = wektor->size();
  if (razmerWektor < 1) {
    throw "ErrorDlin";
  }
  int beesProd, razmerProd;
  MPI_Comm_size(MPI_COMM_WORLD, &razmerProd);
  MPI_Comm_rank(MPI_COMM_WORLD, &beesProd);

  int danBloka = razmerWektor / razmerProd;
  std::vector<int> wektorLocaly(danBloka);

  MPI_Scatter(&(*wektor)[0], danBloka, MPI_INT, &wektorLocaly[0], danBloka,
              MPI_INT, 0, MPI_COMM_WORLD);

  QuickSortNO_MPI(&wektorLocaly, 0, danBloka - 1);

  MPI_Gather(&wektorLocaly[0], danBloka, MPI_INT, &(*wektor)[0], danBloka,
             MPI_INT, 0, MPI_COMM_WORLD);

  if (beesProd == 0) {
    QuickSortNO_MPI(&(*wektor), 0, razmerWektor - 1);
  }
}
