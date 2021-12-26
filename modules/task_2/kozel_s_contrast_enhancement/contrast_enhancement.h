// Copyright 2021 Kozel Svetlana
#ifndef MODULES_TASK_2_KOZEL_S_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_
#define MODULES_TASK_2_KOZEL_S_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_

#include <mpi.h>
#include <vector>

std::vector<int> randomIMG(int w, int h);
void printIMG(const std::vector<int>& img, int w, int h);
std::vector<int> contrastEnhancementSeq(const std::vector<int>& img, int w, int  h, int corr);
std::vector<int> contrastEnhancementPar(std::vector<int> img, int w, int  h, int corr);

#endif  // MODULES_TASK_2_KOZEL_S_CONTRAST_ENHANCEMENT_CONTRAST_ENHANCEMENT_H_
