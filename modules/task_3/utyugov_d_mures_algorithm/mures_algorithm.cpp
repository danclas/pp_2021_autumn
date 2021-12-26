// Copyright 2021 Utyugov Denis
#include "../../../modules/task_3/utyugov_d_mures_algorithm/mures_algorithm.h"

#include <mpi.h>

#include <queue>
#include <string>
#include <vector>
#include <iostream>

void add_edge(std::vector<edge>* e, int x, int y, int w) {
  edge e_n;
  e_n.x = x;
  e_n.y = y;
  e_n.w = w;
  (*e).push_back(e_n);
}

std::vector<std::vector<int>> mure(std::vector<edge> e, int v, int rootv) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int var = 0;
  int proc_calc = 1;
  int* dist = new int[v];
  int* buff = new int[5];
  int* p = new int[v];
  int n = e.size();
  std::queue<int> Q;
  int act = rootv;
  int countS = 0;

  Q.push(rootv);

  if (rank == 0) {
    for (int i = 0; i < v; i++) {
      dist[i] = INF;
      p[i] = -1;
    }
    dist[rootv] = 0;
    p[rootv] = rootv;
    while (!(Q.empty())) {
      act = Q.front();
      Q.pop();
      proc_calc = 1;

      for (int i = 0; i < n; i++) {
        if (e[i].x == act) {
          if (size > 1) {
            buff[0] = e[i].y;
            buff[1] = dist[e[i].x];
            buff[2] = dist[e[i].y];
            buff[3] = e[i].w;
            buff[4] = e[i].x;
            MPI_Send(buff, 5, MPI_INT, proc_calc, 0, MPI_COMM_WORLD);
            proc_calc++;
            countS++;
            if (proc_calc >= size) {
              proc_calc = 1;
            }
          } else {
            if (dist[e[i].y] > dist[e[i].x] + e[i].w) {
              Q.push(e[i].y);
              dist[e[i].y] = dist[e[i].x] + e[i].w;
              p[e[i].y] = e[i].x;
            }
          }
        }
      }
      if (size > 1) {
        for (int i = 0; i < countS; i++) {  // count[act]
          MPI_Recv(buff, 5, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                   MPI_STATUS_IGNORE);
          if (buff[3] == 1) {
            Q.push(buff[0]);
            dist[buff[0]] = buff[2];
            p[buff[0]] = buff[4];
          }
        }
        countS = 0;
      }
    }
    if (size > 1) {
      buff[0] = -1;
      for (int i = 1; i < size; i++) {
        MPI_Send(buff, 5, MPI_INT, i, 0, MPI_COMM_WORLD);
      }
    }

  } else {  // rank!=0
    while (true) {
      MPI_Recv(buff, 5, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if (buff[0] == -1) {
        break;
      }
      var = buff[1] + buff[3];

      if (buff[2] > var) {
        buff[2] = var;
        buff[3] = 1;
        MPI_Send(buff, 5, MPI_INT, 0, 0, MPI_COMM_WORLD);
      } else {
        buff[3] = 0;
        MPI_Send(buff, 5, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
    }
  }
  std::vector<std::vector<int>> roads(v, std::vector<int>());
  // roads.clear();
  if (rank == 0) {
    for (int i = 0; i < v; i++) {
      var = i;
      while (true) {
        if (p[var] == -1) {
          roads[i].push_back(-1);
          break;
        }
        roads[i].push_back(p[var]);
        if (p[var] == rootv) {
          break;
        } else {
          var = p[var];
        }
      }
    }
  }

  delete[] dist;
  delete[] p;
  delete[] buff;

  return roads;
}
