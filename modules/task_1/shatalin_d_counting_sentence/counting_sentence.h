// Copyright 2021 Shatalin Dmitriy
#include <mpi.h>
#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <gtest-mpi-listener.hpp>

#ifndef MODULES_TASK_1_SHATALIN_D_COUNTING_SENTENCE_COUNTING_SENTENCE_H_
#define MODULES_TASK_1_SHATALIN_D_COUNTING_SENTENCE_COUNTING_SENTENCE_H_

std::string random_str(int size);
int num_of_sentences_par(std::string str);
int num_of_sentences_seq(std::string str);

#endif  // MODULES_TASK_1_SHATALIN_D_COUNTING_SENTENCE_COUNTING_SENTENCE_H_
