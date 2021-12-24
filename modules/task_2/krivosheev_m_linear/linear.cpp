// Copyright 2021 Krivosheev Miron
#include "../../../modules/task_2/krivosheev_m_linear/linear.h"

MPI_Comm linear_topology(MPI_Comm comm) {
    int ProcNum;
    MPI_Comm line_comm;
    MPI_Comm_size(comm, &ProcNum);
    if (ProcNum == 1) {
        return comm;
    }
    std::vector<int> index(ProcNum);
    index[0] = 1;
    int count = 1;
    for (int i = 1; i < ProcNum - 1; i++) {
        count += 2;
        index[i] = count;
    }
    index[ProcNum - 1] = ++count;
    std::vector<int> edges(count);
    edges[0] = 1;
    for (int i = 1, j = 1; i < count - 1; i += 2, j++) {
        edges[i] = j - 1;
        edges[i + 1] = j + 1;
    }
    edges[count - 1] = ProcNum - 2;
    MPI_Graph_create(comm, ProcNum, index.data(), edges.data(), 1, &line_comm);
    return line_comm;
}


bool is_linear_topology(MPI_Comm comm) {
    if (comm == MPI_COMM_WORLD) {
        return false;
    }
    int ProcRank;
    int degree;
    MPI_Comm_rank(comm, &ProcRank);
    MPI_Graph_neighbors_count(comm, ProcRank, &degree);
    std::vector<int> neighbors(degree);
    MPI_Graph_neighbors(comm, ProcRank, degree, neighbors.data());
    for (int i = 0; i < degree; i++) {
        int recv = 0;
        MPI_Status Status;
        MPI_Sendrecv(&ProcRank, 1, MPI_INT, neighbors[i],
          1, &recv, 1, MPI_INT, neighbors[i], 1, comm, &Status);
        if (abs(recv - ProcRank) != 1)
            return false;
    }
    return true;
}
