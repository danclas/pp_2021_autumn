// Copyright 2021 Kazhaeva Anastasia
#ifndef MODULES_TASK_3_KAZHAEVA_A_HORIZONTAL_GAUSSIAN_FILTER_HORIZONTAL_GAUSSIAN_FILTER_H_
#define MODULES_TASK_3_KAZHAEVA_A_HORIZONTAL_GAUSSIAN_FILTER_HORIZONTAL_GAUSSIAN_FILTER_H_
#include <vector>
using std::vector;
template <class T>
T clamp(T value, T max, T min) {
  if (value > max)
    return max;
  if (value < min)
    return min;
  return value;
}
void Kernell(std::vector<float>* matrix, float sigma);
std::vector<float> RandomImg(int weight, int height);
std::vector<float> Sequential(const std::vector<float>& matrix,
  const std::vector<float>& img, int weight, int height);
std::vector<float> Parallel(const std::vector<float>& matrix,
  const std::vector<float>& img, int weight, int height);
#endif  // MODULES_TASK_3_KAZHAEVA_A_HORIZONTAL_GAUSSIAN_FILTER_HORIZONTAL_GAUSSIAN_FILTER_H_
