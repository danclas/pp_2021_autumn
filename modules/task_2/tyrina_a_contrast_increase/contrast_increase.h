// Copyright 2021 Tyrina Anastasia
#ifndef MODULES_TASK_2_TYRINA_A_CONTRAST_INCREASE_CONTRAST_INCREASE_H_
#define MODULES_TASK_2_TYRINA_A_CONTRAST_INCREASE_CONTRAST_INCREASE_H_

double* getRandomImage(int width, int height);
double* getSequentialContrastIncrease(double* image, int width, int height,
                                      double contrast);
double* getParallelContrastIncrease(double* image, int width, int height,
                                    double contrast);

#endif  // MODULES_TASK_2_TYRINA_A_CONTRAST_INCREASE_CONTRAST_INCREASE_H_
