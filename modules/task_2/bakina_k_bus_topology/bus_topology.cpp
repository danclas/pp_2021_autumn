// Copyright 2021 Bakina Ksenia
#include "../../../modules/task_2/bakina_k_bus_topology/bus_topology.h"

MPI_Comm get_Bus_Comm(const MPI_Comm comm_old) {
    int proc_count;
    int proc_rank;
    MPI_Comm bus_comm;

    MPI_Comm_size(comm_old, &proc_count);
    MPI_Comm_rank(comm_old, &proc_rank);

    std::vector<int> index(proc_count);
    index[0] = index[proc_count - 1] = 1;
    for (int i = 1; i < proc_count - 1; i++) {
        index[i] = 2;
    }
    for (int i = 1; i < proc_count; i++) {
        index[i] += index[i-1];
    }
    std::vector<int> edges(proc_count == 1 ? 1 : (proc_count - 1) * 2);
    edges[0] = 1;
    if (proc_count > 1) {
        edges[(proc_count - 1) * 2 - 1] = proc_count - 2;
        for (int i = 1, j = 1; i < proc_count - 1; i++, j += 2) {
            edges[j] = i - 1;
            edges[j + 1] = i + 1;
        }
    }

    MPI_Graph_create(comm_old, proc_count, index.data(), edges.data(), false, &bus_comm);

    return bus_comm;
}
