// Copyright 2021 Vorobyova Anna
#ifndef MODULES_TASK_3_VOROBYOVA_A_COMPONENT_LABELING_COMPONENT_LABELING_H_
#define MODULES_TASK_3_VOROBYOVA_A_COMPONENT_LABELING_COMPONENT_LABELING_H_

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

using std::max;
using std::min;
using std::pair;
using std::vector;

vector<int> getRandomImage(int width, int height);
pair<vector<int>, pair<vector<int>, int> > partOne(vector<int> image, int width,
                                                   int height,
                                                   int first_label = 0);
vector<int> partTwo(vector<int> map, vector<int> disjoint_sets, int width,
                    int height);
pair<vector<int>, int> ComponentLabeling(vector<int> image, int width,
                                         int height);
pair<vector<int>, int> ComponentLabelingParallel(vector<int> image, int width,
                                                 int height);

#endif  // MODULES_TASK_3_VOROBYOVA_A_COMPONENT_LABELING_COMPONENT_LABELING_H_
