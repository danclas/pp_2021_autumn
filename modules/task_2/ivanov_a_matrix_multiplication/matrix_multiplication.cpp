// Copyright 2021 Ivanov Arkadiy
#include <random>
#include "../../../modules/task_2/ivanov_a_matrix_multiplication/matrix_multiplication.h"

// can generate in range [-maxValue, -minValue] && [minValue, maxValue] IN: minValue, maxValue > 0
int generateRngValue(int minValue, int maxValue, int* feedback) {
    (void)feedback;  // to supress potential warnings
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type>
        dist(static_cast<unsigned int>(minValue), static_cast<unsigned int>(maxValue));
    return static_cast<int>(dist(rng)) * ((static_cast<int>(dist(rng)) & 0x01) ? -1 : 1);
}

// generates: {minValue + feedback, minValue + feedback + 1, minValue + feedback + 2, ...}
int generateRisingValue(int minValue, int empty, int* feedback) {
    (void)empty;
    return minValue + ((*feedback)++);
}

// generates: {maxValue - feedback, maxValue - feedback - 1, maxValue - feedback - 2, ...}
int generateFallingValue(int empty, int maxValue, int* feedback) {
    (void)empty;
    return maxValue - ((*feedback)++);
}

// generates v1, v2, v1, v2, v1, v2, v1, v2, ... with rule feedback % 2 == 0 -> v1 else v2
int generateJumpingValue(int v1, int v2, int* feedback) {
    return ((*feedback)++ & 0x01) ? v2 : v1;
}
