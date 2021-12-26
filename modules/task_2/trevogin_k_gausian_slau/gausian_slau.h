// Copyright 2021 Trevogin Kirill
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <mpi.h>
#include <random>
#include <vector>

#ifndef MODULES_TASK_2_TREVOGIN_K_GAUSIAN_SLAU_GAUSIAN_SLAU_H_
#define MODULES_TASK_2_TREVOGIN_K_GAUSIAN_SLAU_GAUSIAN_SLAU_H_
void out_mat(std::vector<double> mat, std::vector<double> vec);
std::vector<double> triangulation(std::vector<double> mat, int line, std::vector<double>& vec);
std::vector<double> getRandomVector(std::vector<double>& mat, int line);
std::vector<double> gaus_metod(std::vector<double> mat, std::vector<double> vec);
std::vector<double> res(std::vector<double> mat, std::vector<double> vec);
void DataDistribution(std::vector<double>& mat, std::vector<double>& vec, int& line, std::vector<double>& mat1);
std::vector<double> gaus_metod_parall(std::vector<double> mat, std::vector<double> vec);
std::vector<double> triangulation_parall(std::vector<double> mat, int line, std::vector<double>& vec);
std::vector<double> res_parall(std::vector<double> mat, int line, std::vector<double> vec, std::vector<double> vec1);
#endif // MODULES_TASK_2_TREVOGIN_K_GAUSIAN_SLAU_GAUSIAN_SLAU_H_