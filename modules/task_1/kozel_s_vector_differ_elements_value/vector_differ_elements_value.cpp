// Copyright 2021 Kozel Svetlana
#include <mpi.h>
#include <random>
#include "../../../modules/task_1/kozel_s_vector_differ_elements_value/vector_differ_elements_value.h"

std::vector<int> fillVector(const int size_) {
    std::vector<int> vector(size_);
    std::random_device dev;
    std::mt19937 mt(dev());
    for (int i = 0; i < size_; i++) {
        vector[i] = 0  + mt() % 100;
    }
    return vector;
}

int differElementsValue(const std::vector<int>& vector) {
    if (vector.size() > 1) {
        int max = -1;
        for (int i = 0; i < static_cast<int>(vector.size() - 1); i++) {
            int val = abs(abs(vector[i]) - abs(vector[i + 1]));
            if (max <= val) {
                max = val;
            }
        }
        return max;
    }
    if (vector.size() == 1)
        return vector[0];
    else
        return 0;
}

int paralleldifferElementsValue(const std::vector<int>& vector, const int size) {
    int procCount, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int block = size / procCount;
    int ostatok = size % procCount;

    if (rank < ostatok) {
        block++;
    }

    int localMax;
    int globalMax = 0;

    // The block cannot be equal to 1
    if (block == 1 || ostatok == 1) {
        localMax = differElementsValue(vector);
        return localMax;
    }

    std::vector<int> blockVector(procCount);
    std::vector<int> sdvig(procCount);
    std::vector<int> resultData(block);
    if (rank == 0) {
        for (int i = 0; i < procCount; i++) {
            if (i < ostatok) {
                blockVector[i] = vector.size() / procCount + 1;
            } else {
                blockVector[i] = vector.size() / procCount;
            }
            if (i < procCount - 1) {
                sdvig[i + 1] = sdvig[i] + blockVector[i];
            }
        }
    }
    MPI_Scatterv(reinterpret_cast<const void*>(vector.data()), blockVector.data(), sdvig.data(),
        MPI_INT, resultData.data(), block, MPI_INT, 0, MPI_COMM_WORLD);

    localMax = differElementsValue(resultData);

    MPI_Reduce(reinterpret_cast<void*>(&localMax), reinterpret_cast<void*>(&globalMax),
        1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    return globalMax;
}
