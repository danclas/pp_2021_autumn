// Copyright 2021 Khlyustov Ilya
#define _USE_MATH_DEFINES  // for C++
#include <cmath>
#include <ctime>
#include <random>
#include "../../../modules/task_3/khlyustov_i_image_filter/line_vertical_image_filter.h"

vectorF getKernel(float sigma) {
    float norm = 0;
    float _2sigma2 = 2 * sigma * sigma;
    vectorF kernel(9);

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            int idx = (x + 1) * 3 + (y + 1);
            kernel[idx] = exp(-(x * x + y * y) / _2sigma2) / (_2sigma2 * M_PI);
            norm += kernel[idx];
        }
    }
    for (int i = 0; i < 9; i++) {
        if ((i != 1) && (i != 4) && (i != 7)) {
            kernel[i] = 0.f;
        }
        kernel[i] /= norm;
    }
    return kernel;
}

vectorF getRandomImg(int w, int h) {
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 1);

    vectorF img(w * h);
    for (int i = 0; i < (w * h); i++) {
        img[i] = urd(gen);
    }
    return img;
}

int clamp(int value, int max, int min) {
    return (value > max) ? max : ((value < min) ? min : value);
}

int calcIdx(int x, int y, int w) {
    return y * w + x;
}

float newPixelColor(const vectorF& kernel, const vectorF& img, int h, int w, int x, int y) {
    float color = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int idx = (i + 1) * 3 + (j + 1);
            color += kernel[idx] * img[calcIdx(clamp(x + j, w - 1, 0), clamp(y + 1, h - 1, 0), w)];
        }
    }
    return color;
}

vectorF getSequential(const vectorF& kernel, const vectorF& img, int w, int h) {
    vectorF result(w * h);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            result[calcIdx(x, y, w)] = newPixelColor(kernel, img, h, w, x, y);
        }
    }
    return result;
}

vectorF getParallel(const vectorF& kernel, const vectorF& img, int w, int h) {
    int proc_count, proc_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_count == 1) {
        return getSequential(kernel, img, w, h);
    }
    int w_proc = w / proc_count;
    int remainder = w % proc_count;

    std::vector<int> displs(proc_count);
    std::vector<int> scnts(proc_count);
    scnts[0] = w_proc + remainder;
    displs[0] = 0;
    for (int i = 1; i < proc_count; i++) {
        scnts[i] = w_proc;
        displs[i] = scnts[i - 1] + displs[i - 1];
    }

    std::vector<vectorF> buffer(h);
    vectorF part;
    for (int r = 0; r < h; r++) {
        buffer[r].resize(w);
        MPI_Scatterv(img.data() + r * w, scnts.data(), displs.data(), MPI_FLOAT,
            buffer[r].data(), scnts[proc_rank], MPI_FLOAT, 0, MPI_COMM_WORLD);
        for (int c = 0; c < scnts[proc_rank]; c++) {
            part.push_back(buffer[r][c]);
        }
    }

    vectorF proc_res = getSequential(kernel, part, scnts[proc_rank], h);
    vectorF result;
    for (int r = 0; r < h; r++) {
        MPI_Gatherv(proc_res.data() + r * scnts[proc_rank], scnts[proc_rank], MPI_FLOAT,
            buffer[r].data(), scnts.data(), displs.data(), MPI_FLOAT, 0, MPI_COMM_WORLD);
        if (proc_rank == 0) {
            for (int c = 0; c < w; c++) {
                result.push_back(buffer[r][c]);
            }
        }
    }
    return result;
}
