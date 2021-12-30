// Copyright 2021 Kulikov Ilya
#ifndef MODULES_TASK_3_KULIKOV_I_HORIZONTAL_LINEAR_FILTERING_GAUSS_KERNEL_LINEAR_FILTERING_GAUSS_KERNEL_H_
#define MODULES_TASK_3_KULIKOV_I_HORIZONTAL_LINEAR_FILTERING_GAUSS_KERNEL_LINEAR_FILTERING_GAUSS_KERNEL_H_

double* get_random_img(int sz);
double* get_matrix(double sigma);
double* changing_img(double* strings, double* matrix,
int sz, int part, double sum);
double* horizontal_linear_filter_gauss(double* img, double* matrix, int sz);

#endif  // MODULES_TASK_3_KULIKOV_I_HORIZONTAL_LINEAR_FILTERING_GAUSS_KERNEL_LINEAR_FILTERING_GAUSS_KERNEL_H_
