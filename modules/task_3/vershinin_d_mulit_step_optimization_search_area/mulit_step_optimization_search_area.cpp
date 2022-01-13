// Copyright 2021 Vershinin Daniil
#include "../../../modules/task_3/vershinin_d_mulit_step_optimization_search_area/mulit_step_optimization_search_area.h"

#include <mpi.h>

#include <vector>

Point3d getSequentialOperations(double start_x, double finish_x, double start_y,
                                double finish_y,
                                double (*F)(double x, double y),
                                int max_step_count, double r, double eps) {
  std::vector<Point3d> elems;
  Point3d res = calcNewY(start_y, finish_y, start_x, F, max_step_count, r, eps);
  if (r <= 1) {
    return res;
  }
  elems.push_back(res);
  Point3d new_res =
      calcNewY(start_y, finish_y, finish_x, F, max_step_count, r, eps);
  elems.push_back(new_res);

  if (new_res.z < res.z) {
    res = new_res;
  }

  int step_count = 2;
  bool continueCalc = true;

  while (step_count < max_step_count && continueCalc) {
    double M = calcM(elems, r);

    int j = calcNumMaxR(elems, M);

    double new_x = (elems[j].x + elems[j - 1].x) / 2 -
                   (elems[j].z - elems[j - 1].z) / (2 * M);

    Point3d new_res =
        calcNewY(start_y, finish_y, new_x, F, max_step_count, r, eps);

    if (new_res.z < res.z) {
      res = new_res;
    }

    if (elems[j].x - elems[j - 1].x <= eps) {
      continueCalc = false;
    }

    elems.push_back(new_res);
    std::sort(elems.begin(), elems.end(), sortPoint3d_x);

    step_count++;
  }

  return res;
}

Point3d calcNewY(double start_y, double finish_y, double new_x,
                 double (*F)(double x, double y), int max_step_count, double r,
                 double eps) {
  std::vector<Point2d> elems;

  Point3d res = {new_x, 0, 0};
  if (r <= 1) {
    return res;
  }

  double F_res = F(new_x, start_y);
  elems.push_back(Point2d(start_y, F_res));
  res.y = start_y;
  res.z = F_res;

  double F_new_res = F(new_x, finish_y);
  elems.push_back(Point2d(finish_y, F_new_res));

  if (F_new_res < F_res) {
    res.y = finish_y;
    res.z = F_new_res;
  }

  int step_count = 2;
  bool continueCalc = true;

  while (step_count < max_step_count && continueCalc) {
    double M = calcM(elems, r);

    int j = calcNumMaxR(elems, M);

    double new_y = (elems[j].x + elems[j - 1].x) / 2 -
                   (elems[j].y - elems[j - 1].y) / (2 * M);

    F_new_res = F(new_x, new_y);

    if (F_new_res < res.z) {
      res.y = new_y;
      res.z = F_new_res;
    }

    if (elems[j].x - elems[j - 1].x <= eps) {
      continueCalc = false;
    }

    elems.push_back(Point2d(new_y, F_new_res));

    std::sort(elems.begin(), elems.end(), sortPoint2d_x);

    step_count++;
  }

  return res;
}

Point3d getParallelOperations(double start_x, double finish_x, double start_y,
                              double finish_y, double (*F)(double x, double y),
                              int max_step_count, double r, double eps) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Point3d res;
  if (r <= 1) {
    return res;
  }
  double step_x = (finish_x - start_x) / size;

  std::vector<Point3d> res_list(size);

  double local_start_x = start_x + step_x * rank;
  double local_finish_x = local_start_x + step_x;

  Point3d local_res =
      getSequentialOperations(local_start_x, local_finish_x, start_y, finish_y,
                              F, max_step_count, r, eps);

  MPI_Aint offsets[] = {(MPI_Aint)offsetof(struct Point3d, x),
                        (MPI_Aint)offsetof(struct Point3d, y),
                        (MPI_Aint)offsetof(struct Point3d, z)};
  MPI_Datatype types[] = {MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
  int blockcounts[] = {1, 1, 1};

  MPI_Datatype MPI_Point3d;
  MPI_Type_create_struct(3, blockcounts, offsets, types, &MPI_Point3d);
  MPI_Type_commit(&MPI_Point3d);
  MPI_Gather(&local_res, 1, MPI_Point3d, res_list.data(), 1, MPI_Point3d, 0,
             MPI_COMM_WORLD);
  MPI_Type_free(&MPI_Point3d);

  if (rank == 0) {
    double min_z = res_list[0].z;
    int k = 0;
    for (int i = 1; i < size; i++) {
      if (res_list[i].z < min_z) {
        min_z = res_list[i].z;
        k = i;
      }
    }
    res = res_list[k];
  }

  return res;
}

double calcM(const std::vector<Point2d> &elems, double r) {
  double u = -1;
  double M = 0;
  int N = static_cast<int>(elems.size());
  for (int i = 1; i < N; i++) {
    double current_u =
        std::abs(elems[i].y - elems[i - 1].y) / (elems[i].x - elems[i - 1].x);
    if (current_u > u) {
      u = current_u;
    }
  }
  if (u > 0) {
    M = u * r;
  } else {
    M = 1;
  }
  return M;
}

double calcM(const std::vector<Point3d> &elems, double r) {
  double u = -1;
  double M = 0;
  int N = static_cast<int>(elems.size());
  for (int i = 1; i < N; i++) {
    double current_u =
        std::abs(elems[i].z - elems[i - 1].z) / (elems[i].x - elems[i - 1].x);
    if (current_u > u) {
      u = current_u;
    }
  }
  if (u > 0) {
    M = u * r;
  } else {
    M = 1;
  }
  return M;
}

int calcNumMaxR(const std::vector<Point2d> &elems, double M) {
  double max_R = 2 * (elems[1].x - elems[0].x) - 4 * elems[1].y / M;
  int num_max_R = 1;
  int N = static_cast<int>(elems.size());
  for (int i = 2; i < N - 1; i++) {
    double R = elems[i].x - elems[i - 1].x +
               ((elems[i].y - elems[i - 1].y) * (elems[i].y - elems[i - 1].y)) /
                   (M * M * elems[i].x - elems[i - 1].x) -
               2 * (elems[i].y + elems[i - 1].y) / M;
    if (R > max_R) {
      max_R = R;
      num_max_R = i;
    }
  }
  double R = 2 * (elems[N - 1].x - elems[N - 2].x) - 4 * elems[N - 2].y / M;
  if (R > max_R) {
    max_R = R;
    num_max_R = N - 1;
  }
  return num_max_R;
}

int calcNumMaxR(const std::vector<Point3d> &elems, double M) {
  double max_R = 2 * (elems[1].x - elems[0].x) - 4 * elems[1].z / M;
  int num_max_R = 1;
  int N = static_cast<int>(elems.size());
  for (int i = 2; i < N; i++) {
    double R = elems[i].x - elems[i - 1].x +
               ((elems[i].z - elems[i - 1].z) * (elems[i].z - elems[i - 1].z)) /
                   (M * M * elems[i].x - elems[i - 1].x) -
               2 * (elems[i].z + elems[i - 1].z) / M;
    if (R > max_R) {
      max_R = R;
      num_max_R = i;
    }
  }
  double R = 2 * (elems[N - 1].x - elems[N - 2].x) - 4 * elems[N - 2].z / M;
  if (R > max_R) {
    max_R = R;
    num_max_R = N - 1;
  }
  return num_max_R;
}

bool sortPoint2d_x(const Point2d &left, const Point2d &right) {
  return left.x < right.x;
}
bool sortPoint3d_x(const Point3d &left, const Point3d &right) {
  return left.x < right.x;
}
