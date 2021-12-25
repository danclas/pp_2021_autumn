// Copyright 2021 Samoiluk Anastasiya
#ifndef MODULES_TASK_3_SAMOILUK_A_JARVIS_JARVIS_H_
#define MODULES_TASK_3_SAMOILUK_A_JARVIS_JARVIS_H_

#include<mpi.h>
#include<vector>

struct Point {
    int x;
    int y;

    Point(): x(0), y(0) {}
    Point(int _x, int _y) : x(_x), y(_y) {}
};

std::vector<Point> getRandomVec(int size);
std::vector<Point> JarvisMarch_seq(const std::vector<Point>& vec);
std::vector<Point> JarvisMarch_par(const std::vector<Point>& vec, int vec_size);

#endif  // MODULES_TASK_3_SAMOILUK_A_JARVIS_JARVIS_H_
