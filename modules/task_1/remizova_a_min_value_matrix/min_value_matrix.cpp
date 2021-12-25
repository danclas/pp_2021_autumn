// Copyright 2021 Remizova Antonina
#include "../../../modules/task_1/remizova_a_min_value_matrix/min_value_matrix.h"

std::vector<int> getRandMatrix(int cols, int rows) {
    std::random_device dev;
    std::mt19937 gen(dev());

    const int m_size = cols * rows;
    std::vector<int> m(m_size);
    for (int i = 0; i < m_size; i++) {
        m[i] = gen() % 301 - 150;
    }

    return m;
}

int minElemSequentialOperations(std::vector<int> m, int cols, int rows) {
    int min_el = m[0];

    for (int i = 1; i < (cols * rows); i++) {
        min_el = std::min(m[i], min_el);  // search min
    }

    return min_el;
}

int minElemParallelOperations(std::vector<int> m, int cols, int rows) {
    int size = 0, rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int m_size = 0;  // matrix size (vector size)
    int whole = 0;  // whole division
    int rem = 0;  // remainder division

    m_size = cols * rows;
    if (m_size < size) {
        throw "number of proc exceeds number of array elem";
    } else {
        whole = m_size / size;
        rem = m_size % size;
    }

    std::vector<int> loc_m(whole);

    int min_el = 0;  // min elem on rank
    int min_el_glob = 0;  // min elem in mas
    int start_send = 0;  // index send start
    int start_acc = 0;  // index accept start

    if (rank == 0) {
        for (int pr_r = 1; pr_r < size; pr_r++) {
            loc_m = std::vector<int>(m.begin(), m.begin() + whole);
            start_send = pr_r * whole + rem;
            MPI_Send(&m[start_send], whole, MPI_INT, pr_r, 0, MPI_COMM_WORLD);
        }
    }

    if (rank != 0) {
        MPI_Status status;
        MPI_Recv(&loc_m[start_acc], whole,
            MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        min_el = minElemSequentialOperations(loc_m, 1, whole);
    } else {
        for (int i = 0; i < (whole + rem); i++) {
            min_el = std::min(m[i], min_el);
        }
    }

    MPI_Reduce(&min_el, &min_el_glob, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    return min_el_glob;
}
