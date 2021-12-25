// Copyright 2021 Bakalina Darya
#ifndef MODULES_TASK_3_BAKALINA_D_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_METHOD_H_
#define MODULES_TASK_3_BAKALINA_D_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_METHOD_H_

#include <vector>
std::vector<double> create_random_vector(int size_n);
std::vector<double> create_random_matrix(int size_n);
double scalar_multiply(const std::vector<double>& vctr_x, const std::vector<double>& vctr_y);
std::vector<double> multiply_mtrx_to_v(const std::vector<double>& mtrx, const std::vector<double>& v);
std::vector<double> lin_gradient_method(const std::vector<double>& mtrx, const std::vector<double>& v, int size_n);
std::vector<double> parall_gradient_method(const std::vector<double>& mtrx, const std::vector<double>& v, int size_n);
#endif  // MODULES_TASK_3_BAKALINA_D_CONJUGATE_GRADIENT_METHOD_CONJUGATE_GRADIENT_METHOD_H_
