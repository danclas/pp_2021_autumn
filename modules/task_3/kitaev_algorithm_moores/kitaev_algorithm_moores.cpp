// Copyright 2021 Kitaev Pavel

#include "../../../modules/task_3/kitaev_algorithm_moores/kitaev_algorithm_moores.h"
#include <mpi.h>
#include <random>
#include <deque>
#include <algorithm>
#include <iostream>

const int inf = 1000000;

std::vector<int> GetRandomGraph(int size) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 50);

  int** mass = new int* [size];
  int g = 1;
  for (int i = 0; i < size; i++) {
    mass[i] = new int[size];
    for (int j = 0; j < size; j++) {
      if (i == j) {
        mass[i][j] = 0;
      } else {
        mass[i][j] = 1000000;
      }
    }

    for (int j = g; j < size; j++) {
      mass[i][j] = dist(rng);
    }
    g++;
  }

  std::vector<int> temp(size * size);
  int k = 0;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      temp[k] = mass[i][j];
      k++;
    }
  }

  std::random_device dev2;
  std::mt19937 rng2(dev2());
  std::uniform_int_distribution<std::mt19937::result_type> dist2
  (1, (size * size) - 1);

  for (int i = 0; i < (size * 2);) {
    int t = dist2(rng2);
    if (temp[t] != 0 && temp[t] != inf) {
      temp[t] = inf;
      i++;
    }
  }

  for (int i = 0; i < size; i++) {
    delete[] mass[i];
  }
  delete[] mass;

  return temp;
}

std::vector<int> SequentialMoores(std::vector<int> graph, int size) {
  int y = 0;
  std::deque<int> q;
  q.push_front(0);
  std::vector<int> S;

  std::vector<int> prev;
  prev.push_back(0);

  int* d_star = new int[size];

  d_star[0] = 0;
  for (int i = 1; i < size; i++) {
    d_star[i] = inf;
  }

  do {
    y = q.front();
    q.pop_front();

    for (int i = 0; i < size; i++) {
      if (graph[y * size + i] != inf && graph[y * size + i] != 0) {
        S.push_back(i);
      }
    }

    for (int i = 0; i < static_cast<int>(S.size()); i++) {
      int t = std::min(d_star[S[i]], (d_star[y] + graph[y * size + S[i]]));
      if (t < d_star[S[i]]) {
        bool ctrl = false;
        for (int j = 0; j < static_cast<int>(prev.size()); j++) {
          if (S[i] == prev[j]) {
            ctrl = true;
            break;
          }
        }

        if (ctrl) {
          bool z = false;
          for (std::deque<int>::iterator it = q.begin(); it < q.end(); ++it) {
            if (*it == S[i]) {
              q.erase(it);
              q.push_front(S[i]);
              z = true;
              break;
            }
          }
          if (!z) {
            q.push_front(S[i]);
          }

          d_star[S[i]] = t;
          prev.push_back(S[i]);
        } else {
          bool z = false;
          for (std::deque<int>::iterator it = q.begin(); it < q.end(); ++it) {
            if (*it == S[i]) {
              q.erase(it);
              q.push_back(S[i]);
              z = true;
              break;
            }
          }
          if (!z) {
            q.push_back(S[i]);
          }

          d_star[S[i]] = t;
          prev.push_back(S[i]);
        }
      }
    }

    S.clear();
  } while (!q.empty());

  std::vector<int> moors;
  int p = size - 1;

  do {
    for (int j = 0; j < size; j++) {
      if (graph[j * size + p] != inf && graph[j * size + p] != 0) {
        S.push_back(j);
      }
    }


    for (int j = 0; j < static_cast<int>(S.size()); j++) {
      int t = d_star[S[j]] + graph[S[j] * size + p];
      if (t == d_star[p]) {
        moors.insert(moors.begin(), S[j]);
        p = S[j];
        break;
      }
    }

    S.clear();
  } while (p != 0);
  moors.push_back(size - 1);
  delete[] d_star;
  return moors;
}

std::vector<int> ParallelMoores(std::vector<int> graph, int size_graph) {
  int source = 0;
  int* flag = 0;
  int ProcNum, ProcRank;
  std::vector<int> d(size_graph, inf);
  d[source] = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  const int delta = size_graph / ProcNum;
  const int rem = size_graph % ProcNum;
  int tmp = delta + (ProcRank < rem ? 1 : 0);
  std::vector<int> loc(tmp * size_graph);
  std::vector<int> loc_d(tmp, inf);

  if (loc.size() == 0) {
    loc.resize(1);
  }

  std::vector<int> sendcounts(ProcNum);
  std::vector<int> displs(ProcNum);
  std::vector<int> sendcounts_d(ProcNum);
  std::vector<int> displs_d(ProcNum);

  displs[0] = displs_d[0] = 0;
  for (int i = 0; i < ProcNum; ++i) {
    sendcounts[i] = (delta + (i < rem ? 1 : 0)) * size_graph;
    sendcounts_d[i] = delta + (i < rem ? 1 : 0);
    if (i > 0) {
      displs[i] = (displs[i - 1] + sendcounts[i - 1]);
      displs_d[i] = displs_d[i - 1] + sendcounts_d[i - 1];
    }
  }

  int root = -1;
  for (int i = 0; i < ProcNum - 1; ++i)
    if (source >= displs[i] / size_graph)
      root++;
  if (ProcRank == root)
    loc_d[source - displs[ProcRank] / size_graph] = 0;

  std::vector<int> send(size_graph * size_graph);
  if (ProcRank == 0) {
    for (int i = 0; i < size_graph; i++)
      for (int j = 0; j < size_graph; j++)
        send[i * size_graph + j] = graph[j * size_graph + i];
  }

  MPI_Scatterv(send.data(), sendcounts.data(), displs.data(),
    MPI_INT, &loc[0], tmp * size_graph, MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < size_graph - 1; i++) {
    for (int k = 0; k < sendcounts_d[ProcRank]; k++) {
      for (int j = 0; j < size_graph; j++)
        if ((d[j] < inf) && (loc[k * size_graph + j] < inf))
          if (loc_d[k] > d[j] + loc[k * size_graph + j])
            loc_d[k] = std::min(d[j] + loc[k * size_graph + j], inf);
    }
    MPI_Allgatherv(loc_d.data(), tmp, MPI_INT, d.data(),
      sendcounts_d.data(), displs_d.data(), MPI_INT, MPI_COMM_WORLD);
  }

  if (flag) {
    int flag2 = 0;
    *flag = 0;
    for (int k = 0; k < sendcounts_d[ProcRank]; k++)
      for (int j = 0; j < size_graph; j++)
        if ((d[j] < inf) && (loc[k * size_graph + j] < inf)) {
          if (loc_d[k] > d[j] + loc[k * size_graph + j]) {
            loc_d[k] = inf;
            flag2 = 1;
          }
        }
    MPI_Reduce(&flag2, flag, 1, MPI_INT, MPI_LOR, 0, MPI_COMM_WORLD);
    MPI_Allgatherv(loc_d.data(), tmp, MPI_INT, d.data(),
      sendcounts_d.data(), displs_d.data(), MPI_INT, MPI_COMM_WORLD);
  }

  std::vector<int> moors;

  if (ProcRank == 0) {
    int p = size_graph - 1;
    std::vector<int> S;

    do {
      for (int j = 0; j < size_graph; j++) {
        if (graph[j * size_graph + p] != inf
          && graph[j * size_graph + p] != 0) {
          S.push_back(j);
        }
      }

      for (int j = 0; j < static_cast<int>(S.size()); j++) {
        int t = d[S[j]] + graph[S[j] * size_graph + p];
        if (t == d[p]) {
          moors.insert(moors.begin(), S[j]);
          p = S[j];
          break;
        }
      }

      S.clear();
    } while (p != 0);
    moors.push_back(size_graph - 1);
  }
  return moors;
}
