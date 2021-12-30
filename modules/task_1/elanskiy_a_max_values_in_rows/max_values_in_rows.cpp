  // Copyright 2021 Elanskiy Akexandr
#include "../../../modules/task_1/elanskiy_a_max_values_in_rows/max_values_in_rows.h"

std::vector<int> mat_gen(int rows, int cols) {
    std::mt19937 gen(time(0));
    int size = rows * cols;
    std::vector<int> matrix(size);
    for (int i = 0; i < size; i++) {
        matrix[i] = gen() % 100;
    }
    return matrix;
}

std::vector<int> max_in_rows_seq(std::vector<int> m, int cols) {
    std::vector<int> res;
    int max;
    int size = m.size();
    for (int i = 0; i < size; i += cols) {
        max = m[i];
        for (int j = 1; j < cols; j++) {
            if (m[i + j] > max) {
                max = m[i + j];
            }
        }
        res.push_back(max);
    }
    return res;
}

std::vector<int> max_in_rows_par(std::vector<int> m, int rows, int cols) {
    int procNum;
    int procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    int max = 0;
    std::vector<int> part_size_of_m(procNum);
    std::vector<int> displs(procNum);
    int sum = 0;
    for (int i = 0; i < procNum; i++) {
        part_size_of_m[i] = rows / procNum * cols;
        displs[i] = sum;
        sum += part_size_of_m[i];
    }
    if (rows % procNum > 0) {
        part_size_of_m[procNum - 1] += rows % procNum * cols;
    }
    std::vector<int> part_res;
    std::vector<int> part_of_m(part_size_of_m[procRank]);
    MPI_Scatterv(m.data(), part_size_of_m.data(), displs.data(), MPI_INT,
        part_of_m.data(), part_size_of_m[procRank], MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < part_size_of_m[procRank]; i += cols) {
        max = part_of_m[i];
        for (int j = 1; j < cols; j++) {
            if (part_of_m[i + j] > max) {
                max = part_of_m[i + j];
            }
        }
        part_res.push_back(max);
    }
    std::vector<int> res(rows);
    sum = 0;
    for (int i = 0; i < procNum; i++) {
        part_size_of_m[i] = rows / procNum;
        displs[i] = sum;
        sum += part_size_of_m[i];
    }
    if (rows % procNum > 0) {
        part_size_of_m[procNum - 1] += rows % procNum;
    }
    MPI_Gatherv(part_res.data(), part_size_of_m[procRank], MPI_INT,
        res.data(), part_size_of_m.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
    return res;
}
