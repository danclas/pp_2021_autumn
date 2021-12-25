// Copyright 2021 Kozel Svetlana
#include <ctime>
#include <random>
#include <iostream>
#include "../../../modules/task_2/kozel_s_contrast_enhancement/contrast_enhancement.h"

#define CLR 256
#define vecI std::vector<int>

vecI randomIMG(int w, int h) {
    if (h < 0 || w < 0) {
        throw "Error";
    }
    std::default_random_engine random;
    random.seed(static_cast<unsigned int>(std::time(0)));
    int size = h * w;
    vecI img(size);
    for (int i = 0; i < size; i++)
        img[i] = random() % CLR;
    return img;
}

void printIMG(const vecI& img, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int val = img[y * h + x];
            if (val < 100)
                std::cout << ' ';
            if (val < 10)
                std::cout << ' ';
            std::cout << val << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

int barrier(int pixel) {
    if (pixel > 255) pixel = 255;
    if (pixel < 0) pixel = 0;
    return pixel;
}

vecI contrastEnhancementSeq(const vecI& img, int w, int  h, int corr) {
    if (corr == 0) return img;
    int size = h * w;
    // 1
    int mid = 0;
    for (int i = 0; i < size; i++)
        mid += img[i];
    mid /= size;
    // 2
    vecI pallet(CLR);
    double k = 1.0 + corr / 100.0;
    for (int i = 0; i < CLR; i++)
        pallet[i] = barrier(mid + k * (i - mid));
    // 3
    vecI result(size);
    for (int i = 0; i < size; i++)
        result[i] = static_cast<unsigned char>(pallet[img[i]]);

    return result;
}

vecI contrastEnhancementPar(vecI img, int w, int  h, int corr) {
    int procRank, procCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    if (procCount == 1)
        return contrastEnhancementSeq(img, h, w, corr);

    int size = h * w;
    MPI_Bcast(img.data(), size, MPI_INT, 0, MPI_COMM_WORLD);

    int procMid = 0;
    for (int i = procRank; i < size; i += procCount)
        procMid += img[i];
    // 1
    int mid;
    MPI_Reduce(&procMid, &mid, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(&mid, 1, MPI_INT, 0, MPI_COMM_WORLD);
    mid /= size;
    // 2
    int dist = CLR / procCount;
    int rem = CLR % procCount;

    vecI scount(procCount, dist);
    vecI displs(procCount, 0);
    scount[0] += rem;
    for (int i = 1; i < procCount; i++)
        displs[i] = displs[i - 1] + scount[i - 1];

    vecI palletForProc(scount[procRank]);
    double k = 1.0 + corr / 100.0;
    for (int i = 0; i < scount[procRank]; i++)
        palletForProc[i] = barrier(mid + k * (i + displs[procRank] - mid));

    vecI pallet(CLR);
    MPI_Gatherv(palletForProc.data(), scount[procRank], MPI_INT, pallet.data(),
        scount.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(pallet.data(), CLR, MPI_INT, 0, MPI_COMM_WORLD);
    // 3
    dist = size / procCount;
    rem = size % procCount;

    scount[0] = dist + rem;
    for (int i = 1; i < procCount; i++) {
        scount[i] = dist;
        displs[i] = displs[i - 1] + scount[i - 1];
    }

    vecI resultForProc(scount[procRank]);
    for (int i = 0; i < scount[procRank]; i++)
        resultForProc[i] = static_cast<unsigned int>(pallet[img[displs[procRank] + i]]);

    vecI result(size);
    MPI_Gatherv(resultForProc.data(), scount[procRank], MPI_INT, result.data(),
        scount.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

    return result;
}
