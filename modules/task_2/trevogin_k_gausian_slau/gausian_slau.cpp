// Copyright 2021 Trevogin Kirill
#include "../../../modules/task_2/trevogin_k_gausian_slau/gausian_slau.h"

int ProcNum;
int ProcRank;
int pParallelPivotPos = 0;
std::vector<int> pProcPivotIter;

void out_mat(std::vector<double> mat, std::vector<double> vec)
{
    int line = vec.size();
    int column = mat.size() / line;
    int k = 0;
    for (int i = 0; i < line; i++) {
        for (int j = 0; j < column; j++) {
            std::cout << mat[k] << " ";
            k++;
        }
        std::cout << "| " << vec[i] << std::endl;
    }
    std::cout << std::endl;
}

std::vector<double> triangulation(std::vector<double> mat, int line, std::vector<double>& vec)
{
    double numerator, denominator;
    for (int i = 0; i < line - 1; i++) {
        for (int j = i + 1; j < line; j++) {
            denominator = mat[i * line + i];
            numerator = mat[j * line + i];
            for (int k = 0; k < line; k++) {
                mat[k + j * line] = mat[j * line + k] - (mat[k + i * line] * numerator / denominator);
            }
            vec[j] = vec[j] - (vec[i] * numerator / denominator);
        }
    }
    return mat;
}

std::vector<double> triangulation_parall(std::vector<double> mat, int line, std::vector<double>& vec)
{
    double numerator, denominator;
    int size = pProcPivotIter.size();
    int rank1;
    double t;
    std::vector<double> mat1(line);
    for (int i = 0; i < line - 1; i++) {
        int rank = -1;
        for (int j = 0; j < size; j++) {
            if (pProcPivotIter[j] == i) {
                rank = ProcRank;
                for (int k = 0; k < line; k++) {
                    mat1[k] = mat[j * line + k];
                }
                pParallelPivotPos++;
                t = vec[j];
            }
        }
        std::vector<double> vec1(1);
        MPI_Allreduce(&rank, &rank1, 1, MPI_2INT,
            MPI_MAXLOC, MPI_COMM_WORLD);
        MPI_Bcast(mat1.data(), line, MPI_DOUBLE, rank1, MPI_COMM_WORLD);
        MPI_Bcast(&t, 1, MPI_DOUBLE, rank1, MPI_COMM_WORLD);
        vec1[0] = t;

        for (int j = pParallelPivotPos; j < size; j++) {
            denominator = mat1[i];
            numerator = mat[j * line + i];
            for (int k = 0; k < line; k++) {
                mat[k + j * line] = mat[j * line + k] - (mat1[k] * numerator / denominator);
            }
            vec[j] = vec[j] - (t * numerator / denominator);
        }
    }
    return mat;
}

std::vector<double> res_parall(std::vector<double> mat, int line, std::vector<double> vec, std::vector<double> vec1)
{

    int size = pProcPivotIter.size();
    int rank1;
    double g;
    pParallelPivotPos = 0;
    for (int i = line - 1; i >= 0; i--) {
        int rank = -1;
        std::vector<double> mat1(line);
        for (int j = 0; j < size; j++) {
            if (pProcPivotIter[j] == i) {
                rank = ProcRank;
                g = vec1[j] / mat[j * line + i];
                pParallelPivotPos++;
            }
        }
        MPI_Allreduce(&rank, &rank1, 1, MPI_2INT,
            MPI_MAXLOC, MPI_COMM_WORLD);
        MPI_Bcast(&g, 1, MPI_DOUBLE, rank1, MPI_COMM_WORLD);
        for (int j = size - pParallelPivotPos - 1; j >= 0; j--) {

            vec1[j] = vec1[j] - mat[j * line + i] * g;
        }
        vec[i] = g;
    }
    return vec;
}

std::vector<double> getRandomVector(std::vector<double>& mat, int line)
{
    std::random_device rn;
    std::mt19937 gen(rn());
    std::uniform_int_distribution<> dist(1, 10);
    std::vector<double> mat1(line * line);
    std::vector<double> vec1(line);
    for (int i = 0; i < line * line; i++) {
        mat1[i] = dist(gen);
    }
    mat = mat1;
    for (int i = 0; i < line; i++) {
        vec1[i] = dist(gen);
    }
    return vec1;
}

std::vector<double> res(std::vector<double> mat, std::vector<double> vec)
{
    int column = vec.size();
    int line = mat.size() / column;
    std::vector<double> vec1(line);

    int a = line - 1;
    int k = 0;
    for (int i = line - 1; i >= 0; i--) {
        for (int j = column - 1; j >= a; j--) {

            if (j == a) {
                vec[i] = vec[i] / mat[j + i * line];
            } else {
                vec[i] -= vec[j] * mat[j + i * line];
            }
        }
        a--;
    }
    return vec;
}
void DataDistribution(std::vector<double>& mat, std::vector<double>& vec, int& line, std::vector<double>& mat1)
{
    int* pSendNum;
    int pSendInd;
    int RestRows = line;
    int RowNum;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(vec.data(), line, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(mat.data(), line * line, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    RowNum = (line / ProcNum);
    pSendInd = line - RowNum * ProcNum;
    if (ProcRank < pSendInd) {
        RowNum++;
    }
    std::vector<int> pProcPivotIter1(RowNum);
    std::vector<double> mat2(RowNum * line);
    std::vector<double> vec1(RowNum);
    int k = 0;
    for (int i = ProcRank; i < line; i += ProcNum) {
        for (int j = 0; j < line; j++) {
            mat2[k] = mat[i * line + j];
            k++;
        }
        vec1[k / line - 1] = vec[i];
        pProcPivotIter1[k / line - 1] = i;
    }
    vec = vec1;
    pProcPivotIter = pProcPivotIter1;
    RowNum = line;
    mat1 = mat2;
}
std::vector<double> gaus_metod_parall(std::vector<double> mat, std::vector<double> vec)
{
    int line = sqrt(mat.size());
    pParallelPivotPos = 0;
    int line1 = line;
    if (line == 0) {
        return vec;
    }
    std::vector<double> mat1;
    std::vector<double> mat2;
    std::vector<double> vec1 = vec;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    DataDistribution(mat, vec1, line1, mat1);
    mat1 = triangulation_parall(mat1, line, vec1);

    MPI_Barrier(MPI_COMM_WORLD);

    vec = res_parall(mat1, line, vec, vec1);

    pProcPivotIter.clear();
    return vec;
}

std::vector<double> gaus_metod(std::vector<double> mat, std::vector<double> vec)
{
    int line = sqrt(mat.size());
    if (line == 0) {
        return vec;
    }
    std::vector<double> mat1;
    mat1 = triangulation(mat, line, vec);
    return res(mat1, vec);
}
