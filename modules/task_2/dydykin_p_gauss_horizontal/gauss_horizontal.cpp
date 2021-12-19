// Copyright 2021 Dydykin Pavel
#include "../../../modules/task_2/dydykin_p_gauss_horizontal/gauss_horizontal.h"
#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>

std::vector<double> getRandomMatrix(int m, int n) {
    int size = m * n;
    std::vector<double> matrix(size);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::uniform_int_distribution<unsigned int> uid(1, 100);
    for (int i = 0; i < size; i++) {
        matrix[i] = uid(gen);
    }
    return matrix;
}

std::vector<double> GaussNotParallel(const std::vector<double>& matrix,
    int rows, int cols) {
    if ((rows < 0) || (cols < 0))
        throw "Correct the input data";

    std::vector <double> n(matrix);
    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < rows; j++) {
            double koef = n[i * cols + i] / n[j * cols + i];
            for (int k = i; k < cols; k++)
                n[j * cols + k] = n[j * cols + k] * koef - n[i * cols + k];
        }
    }

    std::vector<double> result(rows);
    for (int i = rows - 1; i >= 0; i--) {
        double b = n[i * cols + cols - 1];
        for (int j = rows - 1; j >= i + 1; j--)
            b -= n[i * cols + j] * result[j];
        result[i] = b / n[i * cols + i];
    }
    return result;
}


std::vector<double> GaussParallel(const std::vector<double>& matrix,
    int rows, int cols) {
    if ((rows < 0) || (cols < 0))
        throw "Correct the input data";

    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int size = rows / ProcNum;
    int remains = rows % ProcNum;

    int n = size;
    if (ProcRank < remains)
        n++;

    std::vector<double> local_result(n * cols);

    if (local_result.size() == 0)
        local_result.resize(1);

    std::vector<int> sendcounts(ProcNum);
    std::vector<int> displs(ProcNum);

    displs[0] = 0;

    for (int i = 0; i < ProcNum; i++) {
        if (i < (rows % ProcNum))
            sendcounts[i] = (size + 1) * cols;
        else
            sendcounts[i] = size * cols;
        if (i > 0)
            displs[i] = (displs[i - 1] + sendcounts[i - 1]);
    }

    MPI_Scatterv(matrix.data(), sendcounts.data(), displs.data(),
        MPI_DOUBLE, &local_result[0], n * cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> PiwRow(cols);

    for (int i = 0; i < displs[ProcRank] / cols; i++) {
        int root = 0;
        int sum = 0;
        for (int j = 0; j < ProcNum; j++, root++) {
            sum += sendcounts[j] / cols;
            if (i < sum) {
                root = j; break;
            }
        }
        MPI_Bcast(&PiwRow[0], cols, MPI_DOUBLE, root, MPI_COMM_WORLD);

        for (int j = 0; j < sendcounts[ProcRank] / cols; j++) {
            double s = PiwRow[i] / local_result[j * cols + i];
            for (int tmp = i; tmp < cols; tmp++)
                local_result[j * cols + tmp] = s * local_result[j * cols + tmp]
                - PiwRow[tmp];
        }
    }

    for (int i = 0; i < sendcounts[ProcRank] / cols; i++) {
        for (int j = 0; j < cols; j++)
            PiwRow[j] = local_result[i * cols + j];
        MPI_Bcast(&PiwRow[0], cols, MPI_DOUBLE, ProcRank, MPI_COMM_WORLD);
        for (int j = i + 1; j < sendcounts[ProcRank] / cols; j++) {
            double s = PiwRow[displs[ProcRank] / cols + i] /
                local_result[j * cols + i + displs[ProcRank] / cols];
            for (int tmp = i + displs[ProcRank] / cols; tmp < cols; tmp++)
                local_result[j * cols + tmp] = s * local_result[j * cols + tmp]
                - PiwRow[tmp];
        }
    }

    int root = 0;
    for (int i = displs[ProcRank] / cols + sendcounts[ProcRank] / cols;
        i < rows; i++) {
        int sum = 0;
        for (int j = 0; j < ProcNum; j++, root++) {
            sum += sendcounts[j] / cols;
            if (i < sum) {
                root = j; break;
            }
        }
        MPI_Bcast(&PiwRow[0], cols, MPI_DOUBLE, root, MPI_COMM_WORLD);
    }

    std::vector<double> res(0);
    if (ProcRank == 0) res.resize(rows * cols);
    MPI_Gatherv(local_result.data(), n * cols, MPI_DOUBLE, res.data(),
        sendcounts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    std::vector<double> x(0);
    if (ProcRank == 0) {
        x.resize(rows);
        for (int i = rows - 1; i >= 0; --i) {
            double b = res[i * cols + cols - 1];
            for (int j = rows - 1; j >= i + 1; --j)
                b -= res[i * cols + j] * x[j];
            x[i] = b / res[i * cols + i];
        }
    }

    return x;
}
