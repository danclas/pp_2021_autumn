// Copyright 2021 Shmanalov Alexander
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_3/shmanalov_a_radix_sort_with_batcher_merge/radix_sort_with_batcher_merge.h"

std::vector<int> generationRandomVector(int size) {
    std::random_device random;
    std::mt19937 generate(random());
    std::vector<int> array(size);
    for (int i = 0; i < size; i++) {
        array[i] = generate() % 100000;
    }
    return array;
}

std::vector<int> sequentialRadixSort(std::vector<int> array) {
    int maxElement = 0, position = 1;
    std::vector<int> result(array.size());
    for (size_t i = 1; i < array.size(); i++) {
        if (maxElement < array[i]) {
            maxElement = array[i];
        }
    }
    while (maxElement / position > 0) {
        int counter[10] = { 0 };
        for (size_t i = 0; i < array.size(); i++) {
            counter[array[i] / position % 10]++;
        }
        for (int i = 1; i < 10; i++) {
            counter[i] += counter[i - 1];
        }
        for (int i = array.size() - 1; i >= 0; i--) {
            result[--counter[array[i] / position % 10]] = array[i];
        }
        for (size_t i = 0; i < array.size(); i++) {
            array[i] = result[i];
        }
        position *= 10;
    }
    return array;
}

std::vector<int> vectorShuffle(std::vector<int> array) {
    std::vector<int> shuffleArr(array.size());
    for (size_t i = 0; i < array.size(); i++) {
        if (i % 2 == 0) {
            shuffleArr[i] = array[i];
        } else {
            shuffleArr[i] = array[i];
        }
    }
    return shuffleArr;
}

std::vector<int> OddMerge(const std::vector<int>& arr1,
    const std::vector<int>& arr2) {
    std::vector<int> result(arr1.size() / 2 + arr2.size());
    size_t i = arr1.size() / 2 + arr1.size() % 2, j = 0, k = 0, m = arr1.size();
    while ((i < m) && (j < arr2.size())) {
        if (arr1[i] < arr2[j]) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }
    while (i < m)
        result[k++] = arr1[i++];
    while (j < arr2.size())
        result[k++] = arr2[j++];
    return result;
}

std::vector<int> EvenMerge(const std::vector<int>& arr1,
    const std::vector<int>& arr2) {
    std::vector<int> result(arr1.size() / 2 + arr1.size() % 2 + arr2.size());
    size_t i = 0, j = 0, k = 0, m = arr1.size() / 2 + arr1.size() % 2;
    while ((i < m) && (j < arr2.size())) {
        if (arr1[i] < arr2[j]) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }
    while (i < m)
        result[k++] = arr1[i++];
    while (j < arr2.size())
        result[k++] = arr2[j++];
    return result;
}

std::vector<int> oddEvenMerge(std::vector<int> arr1, std::vector<int> arr2,
    int evenCount, int oddCount) {
    int generalCount = evenCount + oddCount;
    std::vector<int> result(generalCount);
    int i = 0, j = 0, k = 0;
    while ((i < evenCount) && (j < oddCount)) {
        if (arr1[i] < arr2[j]) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }
    while (i < evenCount)
        result[k++] = arr1[i++];
    while (j < oddCount)
        result[k++] = arr2[j++];
    return result;
}

std::vector<int> parallelRadixSortMergeBatcher(std::vector<int> globalArr,
    int sizeArr) {
    // MPI information
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (sizeArr < size || size == 1 || size > sizeArr / 2) {
        if (rank == 0) {
            globalArr = sequentialRadixSort(globalArr);
        }
        return globalArr;
    }
    // number of elems per process
    const int partOnProcess = sizeArr / size;
    const int remainPart = sizeArr % size;
    std::vector<int> localArr;
    // memory allocation for a local array
    if (rank == 0) {
        localArr.reserve(sizeArr);
        localArr.resize(partOnProcess + remainPart);
    } else {
        localArr.resize(partOnProcess);
    }
    // data distribution
    MPI_Status status;
    if (rank == 0) {
        for (int i = 0; i < partOnProcess + remainPart; i++) {
            localArr[i] = globalArr[i];
        }
        for (int i = 1; i < size; i++) {
            MPI_Send(globalArr.data() + i * partOnProcess + remainPart,
                     partOnProcess, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(localArr.data(), partOnProcess,
                 MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }
    // counting the number of mergers
    int mergeNumber = 1, pow = 2;
    while (pow < size) {
        pow *= 2;
        mergeNumber++;
    }
    // sorting and mixing of local parts
    localArr = sequentialRadixSort(localArr);
    localArr = vectorShuffle(localArr);
    // the Batcher merge
    std::vector<int> resultArr, oddArr, evenArr;
    int mergeCount = 2, shift = 1, sendCount, recvCount;
    for (int i = 0; i < mergeNumber; i++) {
        if ((rank % mergeCount == 0) && (rank + shift < size)) {
            sendCount = localArr.size() / 2;
            MPI_Sendrecv(&sendCount, 1, MPI_INT, rank + shift, 0,
                &recvCount, 1, MPI_INT, rank + shift, 0, MPI_COMM_WORLD,
                &status);
            resultArr.resize(recvCount / 2 + recvCount % 2);
            MPI_Sendrecv(&localArr[localArr.size() / 2 + localArr.size() % 2],
                sendCount, MPI_INT, rank + shift, 0, &resultArr.front(),
                recvCount / 2 + recvCount % 2, MPI_INT, rank + shift, 0,
                MPI_COMM_WORLD, &status);
            evenArr = EvenMerge(localArr, resultArr);
            oddArr.resize(recvCount / 2 + localArr.size() / 2);
            MPI_Recv(&oddArr.front(), recvCount / 2 + localArr.size() / 2,
                MPI_INT, rank + shift, 0, MPI_COMM_WORLD, &status);
            localArr = oddEvenMerge(evenArr, oddArr, evenArr.size(),
                oddArr.size());
        }
        if (((rank - shift) % mergeCount == 0) && (rank - shift >= 0)) {
            sendCount = localArr.size();
            MPI_Sendrecv(&sendCount, 1, MPI_INT, rank - shift, 0,
                &recvCount, 1, MPI_INT, rank - shift, 0, MPI_COMM_WORLD,
                &status);
                resultArr.resize(recvCount);
            MPI_Sendrecv(localArr.data(), sendCount / 2 + sendCount % 2,
                MPI_INT, rank - shift, 0, resultArr.data(), recvCount,
                MPI_INT, rank - shift, 0, MPI_COMM_WORLD, &status);
            oddArr = OddMerge(localArr, resultArr);
            MPI_Send(oddArr.data(), oddArr.size(), MPI_INT, rank - shift, 0,
                     MPI_COMM_WORLD);
        }
        mergeCount *= 2;
        shift *= 2;
    }
    return localArr;
}
