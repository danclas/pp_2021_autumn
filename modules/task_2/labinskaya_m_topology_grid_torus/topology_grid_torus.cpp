// Copyright 2021 Labinskaya Marina
#include <mpi.h>
#include "../../../modules/task_2/labinskaya_m_topology_grid_torus/topology_grid_torus.h"

MPI_Comm creating_topology_grid_torus(int n, int m) {
    int ProcNum;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    if (n > 0 && m > 0 && n * m <= ProcNum) {
        int dims[2] = { n, m };
        int periods[2] = { 1, 1 };

        MPI_Comm grid_torus_comm;

        MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, false, &grid_torus_comm);
        return grid_torus_comm;
    } else {
        return MPI_COMM_NULL;
    }
}

bool is_topology_grid_torus(const MPI_Comm grid_torus_comm, const int ndims, const int *dims) {
    int type = 0;
    bool res = true;

    MPI_Topo_test(grid_torus_comm, &type);
    if (type != MPI_CART) {
        res = false;
    } else {
        int ndims_real;
        MPI_Cartdim_get(grid_torus_comm, &ndims_real);
        if (ndims_real != ndims) {
            res = false;
        } else {
            std::vector<int>dims_real(ndims_real);
            std::vector<int>periods_real(ndims_real);
            std::vector<int>coords_real(ndims_real);

            MPI_Cart_get(grid_torus_comm, ndims_real, dims_real.data(), periods_real.data(), coords_real.data());

            for (int i = 0; i < ndims_real; ++i) {
                if (dims_real[i] != dims[i] || periods_real[i] != 1) {
                    res = false;
                }
            }
        }
    }
    return res;
}
