// Copyright 2021 Vershinin Daniil
#ifndef MODULES_TASK_3_VERSHININ_D_MULIT_STEP_OPTIMIZATION_SEARCH_AREA_MULIT_STEP_OPTIMIZATION_SEARCH_AREA_H_
#define MODULES_TASK_3_VERSHININ_D_MULIT_STEP_OPTIMIZATION_SEARCH_AREA_MULIT_STEP_OPTIMIZATION_SEARCH_AREA_H_

#include <algorithm>
#include <vector>

struct Point2d {
  double x, y;
  Point2d(double x , double y) : x(x), y(y) {}
  Point2d() : x(0), y(0) {}
};
struct Point3d {
  double x, y, z;
  Point3d(double x, double y, double z) : x(x), y(y), z(z) {}
  Point3d() : x(0), y(0), z(0) {}
};

Point3d getSequentialOperations(double start_x, double finish_x, double start_y,
                                double finish_y,
                                double (*F)(double x, double y),
                                int max_step_count = 300, double r = 2,
                                double eps = 0.0001);

Point3d calcNewY(double start_y, double finish_y, double new_x,
                 double (*F)(double x, double y), int max_step_count,
                 double r = 2, double eps = 0.0001);

Point3d getParallelOperations(double start_x, double finish_x, double start_y,
                              double finish_y, double (*F)(double x, double y),
                              int max_step_count = 300, double r = 2,
                              double eps = 0.0001);

double calcM(const std::vector<Point2d> &elems, double r);
double calcM(const std::vector<Point3d> &elems, double r);
int calcNumMaxR(const std::vector<Point2d> &elems, double M);
int calcNumMaxR(const std::vector<Point3d> &elems, double M);

bool sortPoint2d_x(const Point2d &left, const Point2d &right);
bool sortPoint3d_x(const Point3d &left, const Point3d &right);

#endif  // MODULES_TASK_3_VERSHININ_D_MULIT_STEP_OPTIMIZATION_SEARCH_AREA_MULIT_STEP_OPTIMIZATION_SEARCH_AREA_H_
