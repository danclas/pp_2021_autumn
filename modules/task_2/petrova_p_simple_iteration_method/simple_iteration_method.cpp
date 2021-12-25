// Copyright 2021 Petrova Polina
#include <mpi.h>
#include <random>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include"../../../modules/task_2/petrova_p_simple_iteration_method/simple_iteration_method.h"

std::vector<double> fillRandomVector(int len) {
    if (len < 0) {
        throw "error! lenght vector < 0!";
    } else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 10);
        std::vector<double> v(len);
        for (int i = 0; i < len; i++) {
            v[i] = dist(gen);
        }
        return v;
    }
}
std::vector< std::vector<double>> fillRandomMatrix(int n) {
    if (n < 0) {
        throw "error! lenght vector < 0!";
    } else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 10);  //  values 1-10
        std::uniform_int_distribution<> dist1(10, 20);  //  for diagonal
        std::vector<std::vector<double>> v(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j)
                    v[i].push_back(dist1(gen));
                v[i].push_back(dist(gen));
            }
        }
        return v;
    }
}
std::vector<double> Convert(std::vector<std::vector<double> > M, int n) {
    std::vector<double> V;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            V.push_back(M[i][j]);
        }
    }
    /*for (int i = 0; i < n; i++) {
        V.push_back(b[i]);
    }*/
    return V;
}
std::vector<double> seqMethod(std::vector< std::vector<double>> matrix,
    std::vector<double> b, int n) {

    std::vector<double> res(n);
    std::vector<double> x(n);
    for (int i = 0; i < n; i++) {
        res[i] = matrix[i][i] / b[i];
    }
    double eps = 0.0001;

    do {
        for (int i = 0; i < n; i++) {
            x[i] = b[i] / matrix[i][i];
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    continue;
                } else {
                    x[i] -= matrix[i][j] / matrix[i][i] * res[j];
                }
            }
        }
        bool f = true;
        for (int i = 0; i < n - 1; i++) {
            if (std::abs(x[i] - res[i]) > eps) {
                f = false;
                break;
            }
        }
        for (int i = 0; i < n; i++) {
            res[i] = x[i];
        }
        if (f)
            break;
    } while (1);
    return res;

   /* std::vector<double> firstSolv(n, 0.0);
    int it = 0;
    double eps = 0.0001;
    while (true) {
        it++;
        if (it == 1000)
            break;
        std::vector<double> currentSol(n, 0.0);
        for (int i = 0; i < n; i++) {
            currentSol[i] = mat[i][n] / mat[i][i];
            for (int j = 0; j < n; j++) {
                if (i != j)
                    currentSol[i] = currentSol[i] - mat[i][j]
                    / b[i] * firstSolv[i];  //  mat[i][i]
            }
        }
        double err = 0.0;
        for (int i = 0; i < n; i++) {
            err = err + std::abs(currentSol[i] - firstSolv[i]);
        }
        if (err < eps)
            break;
        firstSolv = currentSol;
    }
    return firstSolv;*/
}
std::vector<double> parallelMethod(const std::vector<std::vector <double> > &mat,
    std::vector<double> b, int n) {
    int procNum, procRank;
    std::vector<double> matV = Convert(mat, n);
    int chek = static_cast<int>(matV.size());
    if ((n < 0) || (chek != n * n)) {
        throw "error matrix";
    }
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int size = static_cast<int>(b.size());
    if (size < procNum) {
        return (seqMethod(mat, b, n));
    }

    int littleLen = size / procNum;
    int d = size % procNum;
    std::vector<int> matCount(procNum);
    std::vector<int> matExtra(procNum);
    std::vector<int> bCount(procNum);
    std::vector<int> bExtra(procNum);
    matCount.push_back((d + littleLen) * size);
    matExtra.push_back(0);
    bCount.push_back(littleLen + d);
    bExtra.push_back(0);
    for (int i = 1; i < procNum; i++) {
        matCount[i] = littleLen * size;
        bCount[i] = littleLen;
        matExtra[i] = (d + littleLen * i) * size;
        bExtra[i] = d + littleLen * i;
    }

    std::vector<double> firstSolv(size);
    std::vector<double> currentSolv(size);
    std::vector<double> locMat(matCount[procRank]);
    std::vector<double> locV(bCount[procRank]);

    MPI_Scatterv(matV.data(), matCount.data(), matExtra.data(), MPI_DOUBLE,
        locMat.data(), matCount[procRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b.data(), bCount.data(), bExtra.data(), MPI_DOUBLE,
        locV.data(), bCount[procRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double de;
    for (int i = 0; i < bCount[procRank]; i++) {
        de = locMat[bExtra[procRank] + i * size + i];
        locV[i] = locV[i] / de;
        for (int j = 0; j < size; j++) {
            if (j == bExtra[procRank] + i)
                locMat[size * i + j] = 0;
            else
                locMat[size * i + j] = locMat[size * i + j] / de;
        }
    }
    MPI_Gatherv(locV.data(), bCount[procRank], MPI_DOUBLE,
        firstSolv.data(), bCount.data(), bExtra.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(firstSolv.data(), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double locErr = 0;
    double err = 0;
    double eps = 0.0001;
    do {
        for (int i = 0; i < bCount[procRank]; i++) {
            currentSolv[i] = locV[i];
            for (int j = 0; j < size; j++)
                currentSolv[i] = currentSolv[i] - locMat[i * size + j] * firstSolv[j];
            locErr = std::abs(currentSolv[i] - firstSolv[bExtra[procRank] + i]);
        }
        MPI_Gatherv(currentSolv.data(), bCount[procRank], MPI_DOUBLE, firstSolv.data(),
            bCount.data(), bExtra.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(firstSolv.data(), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Allreduce(&locErr, &err, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        //  if (err < eps)
        //  break;
    } while (err > eps);
    return firstSolv;
    /*
    int startI;
    std::vector<double> locMat;
    int count = littleLen;
    if (procRank == 0) {
        locMat = matV;
        auto reSize = static_cast<int>(matV[0] + littleLen * n);
        locMat.resize(reSize);
        for (int32_t i = 1; i < procNum; i++) {
            startI = littleLen * i;
            if (i == procNum - 1)
                count = n - startI;
            MPI_Send(&matV[startI], n * count,
                MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        count = littleLen;
    } else {
        MPI_Status status;
        if (procRank == procNum - 1)
            count = n - littleLen * (procNum - 1);
        //  locMat = mat;
        //  locMat.resize(count);
        locMat.resize(count);
        MPI_Recv(& locMat[0], n * count,
            MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }
    std::vector<int> resCount(procNum);
    std::vector<int> extra(procNum);
    for (int32_t i = 0; i < procNum; i++) {
        if (i == procNum - 1)
            resCount[i] = n - littleLen * (procNum - 1);
        else
            resCount[i] = littleLen;
        if (i == 0)
            extra[i] = 0;
        else
            extra[i] = extra[i - 1] + littleLen;
    }
    int it = 0;
    double eps = 0.0001;
    auto locLen = static_cast<int>(locMat.size());
    while (true) {
        it++;
        if (it == 1000) {
            break;
        }
        std::vector<double> currentSolv(n, 0.0);
        for (int i = 0; i < locLen; i++) {
            currentSolv[i] = locMat[i * (locLen - 1) * n]
                / locMat[i * (i + procRank * littleLen * n)];  //  [i + procRank * littleLen] ;
            for (int j = 0; j < locLen - 1; j++) {
                if (i + procRank * littleLen != j)
                    currentSolv[i] = currentSolv[i] - (locMat[i*j*n]
                        / locMat[i*(i + procRank * littleLen)*n]) * firstSolv[j];
            }
        }
        double err = 0.0;
        for (int i = 0; i < locLen; i++) {
            err = err + std::abs(currentSolv[i] -
                firstSolv[i + procRank * littleLen]);
        }
        MPI_Allreduce(MPI_IN_PLACE, &err, 1,
            MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        if (err < eps) {
            break;
        }
        MPI_Allgatherv(currentSolv.data(), count, MPI_DOUBLE, firstSolv.data(),
            resCount.data(), extra.data(), MPI_DOUBLE, MPI_COMM_WORLD);
    }
    return firstSolv;*/
}
