// Copyright 2021 Utyugov Denis
#ifndef MODULES_TASK_3_UTYUGOV_D_MURES_ALGORITHM_MURES_ALGORITHM_H_
#define MODULES_TASK_3_UTYUGOV_D_MURES_ALGORITHM_MURES_ALGORITHM_H_

#include <vector>

struct edge {
  int x, y, w;
};
const int INF = 1000;

void add_edge(std::vector<edge> *e, int x, int y, int w);
std::vector<std::vector<int>> mure(std::vector<edge> e, int v, int rootv);

#endif  // MODULES_TASK_3_UTYUGOV_D_MURES_ALGORITHM_MURES_ALGORITHM_H_