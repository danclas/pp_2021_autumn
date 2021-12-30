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
                } else {
                    left = copyData[index - w];
                }
                if ((index < 1) || ((index - 1) / w != x)) {
                    top = 0;
                } else {
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
                    } else {
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
    return {copyData, {nonoverlapping, markMax}};
}

std::vector<int> step_second(std::vector<int> tmpMap, int w, int h, std::vector<int> nonoverlapping) {
    int size = w * h;
    int realSize = size - 1;

    std::vector<int> result(size);
    int resCountPix = size;

    int left = tmpMap[size - w];
    int top = tmpMap[size - 2];

    if ((left == 0) && (top == 0)) {
        resCountPix--;
        result[realSize] = tmpMap[resCountPix];
    }
    if ((left != 0) && (top == 0)) {
        result[realSize] = left;
    }
    if ((left == 0) && (top != 0)) {
        result[realSize] = top;
    }
    if ((left != 0) && (top != 0)) {
        if (left < top) {
            result[realSize] = left;
        } else {
            result[realSize] = top;
        }
    }

    for (int i = 0; i < resCountPix; i++) {
        int curPix = tmpMap[i];
        if (curPix != 0) {
            if (nonoverlapping[curPix] == curPix) {
                result[i] = curPix;
            } else {
                while (nonoverlapping[curPix] != curPix) {
                    curPix = nonoverlapping[curPix];
                }
                result[i] = curPix;
            }
        }
    }
    return result;
}

std::vector<int> set_mark(const std::vector<int>& data, int w, int h) {
    int size = w * h;
    int center = size / 2 + 1;
    int markMax = 0;

    std::vector<int> result(size);
    std::vector<int> badLabels(center);
    std::vector<int> correctlyLabels(center);

    for (int i = 0; i < size; i++) {
        int pixel = data[i];
        if (pixel != 0) {
            int idx = -1;
            for (int k = 1; badLabels[k] != 0; k++)
                if (badLabels[k] == pixel) {
                    idx = k;
                    break;
                }
            if (idx == -1) {
                markMax++;
                badLabels[markMax] = pixel;
                correctlyLabels[markMax] = markMax;
                result[i] = correctlyLabels[markMax];
            } else {
                result[i] = correctlyLabels[idx];
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
        } else {
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
    } else {
        MPI_Status status;
        MPI_Recv(localData.data(), sizeBlock, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int tmpW = 0;
    int tmpH = 0;
    if (commRank == 0) {
        tmpW = (elementsRemaining + sizeBlock) / w;
        tmpH = sizeBlock * commRank;
    } else {
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

    int sendCount = 0;
    if (commRank == 0)
        sendCount = sizeBlock + elementsRemaining;
    else
        sendCount = sizeBlock;

    std::vector<int> globalRastoyanie(size);
    MPI_Gatherv(rastoyanie.data(), sendCount, MPI_INT, globalRastoyanie.data(), recvCounts.data(),
        displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> globalMap(size);
    MPI_Gatherv(map.data(), sendCount, MPI_INT, globalMap.data(), recvCounts.data(),
        displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

    int globalMarkCount = 0;
    MPI_Reduce(&localMarkCount, &globalMarkCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (commRank == 0) {
        for (int i = 1; i < countProc; i++) {
            int beginOffsetTop = elementsRemaining + sizeBlock * i;
            int secondOffsetLeft = beginOffsetTop - w;

            for (int offset = 0; offset < w; offset++) {
                int left = globalMap[secondOffsetLeft + offset];
                int top = globalMap[beginOffsetTop + offset];

                if ((left != 0) && (top != 0)) {
                    int rastoyanieLeft = globalRastoyanie[left];
                    int rastoyanieTop = globalRastoyanie[top];

                    if (rastoyanieLeft != rastoyanieTop) {
                        int maxLT = std::max(left, top);
                        while (globalRastoyanie[maxLT] != maxLT) {
                            maxLT = globalRastoyanie[maxLT];
                        }
                        int minLT = std::min(left, top);
                        while (globalRastoyanie[minLT] != minLT) {
                            minLT = globalRastoyanie[minLT];
                        }
                        if (maxLT != minLT) {
                            globalRastoyanie[maxLT] = minLT;
                            globalMarkCount--;
                        }
                    }
                }
            }
        }
        result = step_second(globalMap, w, h, globalRastoyanie);
    }
    return { result, globalMarkCount };
}
