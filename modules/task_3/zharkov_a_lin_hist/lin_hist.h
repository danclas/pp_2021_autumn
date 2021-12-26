// Copyright 2021 Zharkov Andrey
#ifndef MODULES_TASK_3_ZHARKOV_A_LIN_HIST_LIN_HIST_H_
#define MODULES_TASK_3_ZHARKOV_A_LIN_HIST_LIN_HIST_H_

int* getRandomArr(int width, int height);
int* get_Sequential_lin_hist(int* image, int width, int height, int min, int max);
int* get_Parallel_lin_hist(int* image, int width, int height);
int get_local_min(int* local_img, int width, int height);
int get_local_max(int* local_img, int width, int height);

#endif  // MODULES_TASK_3_ZHARKOV_A_LIN_HIST_LIN_HIST_H_
