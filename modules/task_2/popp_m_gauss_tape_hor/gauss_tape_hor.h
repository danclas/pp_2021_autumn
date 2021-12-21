// Copyright 2021 Popp Maksim
#ifndef MODULES_TASK_2_POPP_M_GAUSS_TAPE_HOR_GAUSS_TAPE_HOR_H_
#define MODULES_TASK_2_POPP_M_GAUSS_TAPE_HOR_GAUSS_TAPE_HOR_H_

#include <vector>

std::vector<double>  sleGenerate(std::vector<double>* sle, std::vector<double>* res, std::size_t num);
std::vector<double> sequential(const std::vector<double>& sle, const std::vector<double>& res, std::size_t num);
std::vector<double> parallel(const std::vector<double>& sle, const std::vector<double>& res, std::size_t num);

#endif  // MODULES_TASK_2_POPP_M_GAUSS_TAPE_HOR_GAUSS_TAPE_HOR_H_