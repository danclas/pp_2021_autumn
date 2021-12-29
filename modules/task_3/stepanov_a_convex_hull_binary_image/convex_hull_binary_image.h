// Copyright 2021 Stepanov Alexander
#ifndef MODULES_TASK_3_STEPANOV_A_CONVEX_HULL_BINARY_IMAGE_CONVEX_HULL_BINARY_IMAGE_H_
#define MODULES_TASK_3_STEPANOV_A_CONVEX_HULL_BINARY_IMAGE_CONVEX_HULL_BINARY_IMAGE_H_

#include <vector>

void generateBinaryImage(std::vector<int>* image,
    std::size_t size_image);
std::vector<std::vector<int>> markingComponents(std::vector<int>* image,
    std::size_t size_image);
void markingNeighbors(std::vector<int>* image, std::size_t size_image,
    std::vector<int>* current_component, int marker, std::size_t start_index);
std::vector<int> createHullComponent(const std::vector<int>& component,
    std::size_t size_image);
std::vector<std::vector<int>> createHullImageSequential(const std::vector<int>& image,
    std::size_t size_image);
std::vector<std::vector<int>> createHullImageParallel(const std::vector<int>& image,
    std::size_t size_image);

#endif  // MODULES_TASK_3_STEPANOV_A_CONVEX_HULL_BINARY_IMAGE_CONVEX_HULL_BINARY_IMAGE_H_
