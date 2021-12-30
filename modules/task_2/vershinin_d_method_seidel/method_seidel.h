// Copyright 2021 Vershinin Daniil
#ifndef MODULES_TASK_2_VERSHININ_D_METHOD_SEIDEL_METHOD_SEIDEL_H_
#define MODULES_TASK_2_VERSHININ_D_METHOD_SEIDEL_METHOD_SEIDEL_H_

#include <vector>
#include <utility>

std::vector<double> getRandomVector(int sz);
std::vector<std::vector<double>> getRandomMatrix(int sz);
int getParallelOperations(std::vector<int> global_vec);

std::vector<double> getSequentialOperations(
    std::vector<std::vector<double>> matrix_A, std::vector<double> vector_b,
    double eps);

bool checkDiagDominance(std::vector<std::vector<double>> matrix);
std::vector<std::vector<double>> inversion(std::vector<std::vector<double>> A);

std::vector<std::vector<double>> multNumberOnMatrix(
    double num, std::vector<std::vector<double>> A);

std::vector<std::vector<double>> multMatrixOnMatrix(
    std::vector<std::vector<double>> A, std::vector<std::vector<double>> B);

std::vector<double> multMatrixOnVector(std::vector<std::vector<double>> A,
                                       std::vector<double> B);

std::vector<std::vector<double>> getLMatrix(std::vector<std::vector<double>> A);
std::vector<std::vector<double>> getUMatrix(std::vector<std::vector<double>> A);

std::vector<double> getParallelOperations(
    std::vector<std::vector<double>> matrix_A, std::vector<double> vector_b,
    double eps);

std::pair<std::vector<double>, bool> getLocalRes(
    std::vector<std::vector<double>> C1, std::vector<double> C2,
    std::vector<double> res_prev, double eps);

std::vector<double> matrixToVector(std::vector<std::vector<double>> A);
std::vector<std::vector<double>> vectorToMatrix(std::vector<double> v,
                                                int line);

#endif  // MODULES_TASK_2_VERSHININ_D_METHOD_SEIDEL_METHOD_SEIDEL_H_
