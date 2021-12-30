// Copyright 2021 Stepanov Alexander
#ifndef MODULES_TASK_2_STEPANOV_A_GAUSS_HORIZONTAL_SCHEME_GAUSS_HORIZONTAL_SCHEME_H_
#define MODULES_TASK_2_STEPANOV_A_GAUSS_HORIZONTAL_SCHEME_GAUSS_HORIZONTAL_SCHEME_H_

#include <vector>

std::vector<double> generateSLE(std::vector<double>* matrix_sle,
    std::vector<double>* vector_result,
    std::size_t number_unknows);
std::vector<double> sequentialGaussScheme(const std::vector<double>& matrix_sle,
    const std::vector<double>& vector_result,
    std::size_t number_unknows);
std::vector<double> parallelGaussScheme(const std::vector<double>& matrix_sle,
    const std::vector<double>& vector_result,
    std::size_t number_unknows);

#endif  // MODULES_TASK_2_STEPANOV_A_GAUSS_HORIZONTAL_SCHEME_GAUSS_HORIZONTAL_SCHEME_H_
