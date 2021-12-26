// Copyright 2021 Mironova Ekaterina
#ifndef MODULES_TASK_3_MIRONOVA_E_JARVIS_PASS_JARVIS_PASS_H_
#define MODULES_TASK_3_MIRONOVA_E_JARVIS_PASS_JARVIS_PASS_H_

#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>

struct point {
    int x;
    int y;
};

std::vector<point> fillPoints(int size);
std::vector<point> jarvisPass(const std::vector<point>& points);
std::vector<point> parallelJarvisPass(const std::vector<point>& points, const int size);

#endif  // MODULES_TASK_3_MIRONOVA_E_JARVIS_PASS_JARVIS_PASS_H_
