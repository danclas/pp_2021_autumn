// Copyright 2021 Brazhnik Dmitry
#include "../../../modules/task_3/brazhnik_d_marking_binary_image/marking_binary_image.h"

std::vector<int> generate_binary_image(int w, int h) {
    if (w > 0 && h > 0) {
        int size = w * h;
        std::vector<int> data(size);
        std::random_device dev;
        std::mt19937 gen(dev());
        for (int i = 0; i < size; i++) {
            data[i] = 0 + gen() % 2;
        }
        return data;
    }
    throw "Error: Size is small!";
}

std::pair<std::vector<int>, std::pair<std::vector<int>, int>>
step_first(const std::vector<int>& data, int w, int h, int startMarked) {
    int size = w * h;
    int markMax = 0;
    std::vector<int> nonoverlapping(size);
    std::vector<int> copyData(data.begin(), data.begin() + size);

    for (int i = 0; i < size; i++) {
        nonoverlapping[i] = i + startMarked;
    }
    int index = 0;
    for (int x = 0; x < h; x++) {
        for (int y = 0; y < w; y++) {
            if (copyData[index] != 0) {
                int left, top;
                if (index < w) {
                    left = 0;
                }
                else {
                    left = copyData[index - w];
                }
                if ((index < 1) || ((index - 1) / w != x)) {
                    top = 0;
                }
                else {
                    top = copyData[index - 1];
                }
                if ((left == 0) && (top == 0)) {
                    copyData[index] = index + 1 + startMarked;
                    markMax++;
                }
                if ((left == 0) && (top != 0)) {
                    copyData[index] = top;
                }
                if ((left != 0) && (top == 0)) {
                    copyData[index] = left;
                }
                if ((left != 0) && (top != 0)) {
                    if (left == top) {
                        copyData[index] = left;
                    }
                    else {
                        int maxFS;
                        if (left > top)
                            maxFS = left;
                        else
                            maxFS = top;
                        while (nonoverlapping[maxFS - startMarked] != maxFS) {
                            maxFS = nonoverlapping[maxFS - startMarked];
                        }
                        int minFS;
                        if (left < top)
                            minFS = left;
                        else
                            minFS = top;

                        while (nonoverlapping[minFS - startMarked] != minFS) {
                            minFS = nonoverlapping[minFS - startMarked];
                        }
                        if (maxFS != minFS) {
                            nonoverlapping[maxFS - startMarked] = minFS;
                            markMax--;
                        }
                        copyData[index] = minFS;
                    }
                }
            }
            index++;
        }
    }
    return { copyData, {nonoverlapping, markMax} };
}

std::vector<int> step_second(std::vector<int> tmpMap, int w, int h, std::vector<int> nonoverlapping) {
    int size = w * h;
    //std::cout << "ASize: " << size << "\n";
    std::vector<int> result(size);
    //std::cout << "BSize: " << size << "\n";

    int i = 0;
    for (i = 0; i < size; i++) {
        if (tmpMap.size() < i) {
            int curPix = tmpMap[i];
            if (curPix != 0) {
                if (nonoverlapping[curPix] == curPix) {
                    result[i] = curPix;
                }
                else {
                    while (nonoverlapping[curPix] != curPix) {
                        curPix = nonoverlapping[curPix];
                    }
                    result[i] = curPix;
                }
            }
        }
    }
    return result;
}

std::pair<std::vector<int>, int> basic_marking_binary_image(const std::vector<int>& data, int w, int h) {
    std::pair<std::vector<int>, std::pair<std::vector<int>, int>> firstStep = step_first(data, w, h);
    std::vector<int> secondStep = step_second(firstStep.first, w, h, firstStep.second.first);
    std::pair<std::vector<int>, int> result = std::make_pair(secondStep, firstStep.second.second);

    return result;
}

std::pair<std::vector<int>, int> parallel_marking_binary_image(const std::vector<int>& data, int w, int h) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    // If starting one processing
    if (countProc == 1) {
        return basic_marking_binary_image(data, w, h);
    }

    int size = w * h;
    int sizeBlock = h / countProc * w;
    int elementsRemaining = (h % countProc) * w;
    std::vector<int> result(size);

    if (sizeBlock == 0 || countProc < sizeBlock) {
        if (commRank == 0) {
            return basic_marking_binary_image(data, w, h);
        }
        else {
            return std::make_pair(result, 0);
        }
    }

    if (commRank == 0) {
        for (int i = 1; i < countProc; i++) {
            int tmp = elementsRemaining + i * sizeBlock;
            if (sizeBlock % countProc == 0)
                tmp--;
            MPI_Send(data.data() + tmp, sizeBlock, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> localData(sizeBlock + elementsRemaining);
    if (commRank == 0) {
        localData = std::vector<int>(data.cbegin(), data.cbegin() + sizeBlock + elementsRemaining);
    }
    else {
        MPI_Status status;
        MPI_Recv(localData.data(), sizeBlock, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int tmpW = 0;
    int tmpH = 0;
    if (commRank == 0) {
        tmpW = (elementsRemaining + sizeBlock) / w;
        tmpH = sizeBlock * commRank;
    }
    else {
        tmpW = sizeBlock / w;
        tmpH = elementsRemaining + sizeBlock * commRank;
    }

    std::pair<std::vector<int>, std::pair<std::vector<int>, int>> stepFirst = step_first(localData, tmpW, tmpH);

    std::vector<int> map = stepFirst.first;
    std::vector<int> rastoyanie = stepFirst.second.first;
    int localMarkCount = stepFirst.second.second;

    std::vector<int> displs(countProc);
    displs[1] = elementsRemaining + sizeBlock;

    std::vector<int> recvCounts(countProc);
    recvCounts[0] = elementsRemaining + sizeBlock;
    recvCounts[1] = sizeBlock;

    for (int proc = 2; proc < countProc; proc++) {
        displs[proc] = displs[proc - 1] + sizeBlock;
        recvCounts[proc] = sizeBlock;
    }

    std::vector<int> globalRastoyanie(size);
    std::vector<int> globalMap(size);
    if (commRank == 0) {
        MPI_Gatherv(rastoyanie.data(), sizeBlock + elementsRemaining, MPI_INT,
            globalRastoyanie.data(), recvCounts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gatherv(map.data(), sizeBlock + elementsRemaining, MPI_INT, globalMap.data(), recvCounts.data(),
            displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Gatherv(rastoyanie.data(), sizeBlock, MPI_INT, globalRastoyanie.data(), recvCounts.data(),
            displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gatherv(map.data(), sizeBlock, MPI_INT, globalMap.data(), recvCounts.data(),
            displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
    }

    int globalMarkCount = 0;
    MPI_Reduce(&localMarkCount, &globalMarkCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (commRank == 0) {
        result = step_second(globalMap, w, h, globalRastoyanie);
    }
    return { result, globalMarkCount };
}
