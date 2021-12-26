// Copyright 2021 Elanskiy Akexandr
#include "../../../modules/task_3/elanskiy_a_convex_hull_construction_Jarvis/convex_hull_construction_Jarvis.h"

std::vector<std::pair<int, int>> gen_dots(int vectorSize) {
    std::mt19937 rand_r(time(0));
    std::vector<std::pair<int, int>> vec(vectorSize);
    for (int i = 0; i < vectorSize; i++) {
        vec[i].first = rand_r() % 10000;
        vec[i].second = rand_r() % 10000;
    }
    return vec;
}

int rotation(const std::pair<int, int>& dot1, const std::pair<int, int>& dot2, const std::pair<int, int>& dot3) {
    return (dot2.first - dot1.first) * (dot3.second - dot2.second)
        - (dot2.second - dot1.second) * (dot3.first - dot2.first);
}

std::vector<std::pair<int, int>> jarvis_seq(const std::vector<std::pair<int, int>>& dots) {
    std::vector<int> _indxs(dots.size());
    int vec_size = dots.size();
    int tmp;
    for (int i = 0; i < vec_size; i++) {
        _indxs[i] = i;
    }
    for (int i = 1; i < vec_size; ++i) {
        if (dots[_indxs[i]].first < dots[_indxs[0]].first) {
            tmp = _indxs[i];
            _indxs[i] = _indxs[0];
            _indxs[0] = tmp;
        }
    }

    std::vector<int> res_indx = { _indxs[0] };
    _indxs.erase(_indxs.begin());
    _indxs.push_back(res_indx[0]);

    int _indxsSize;
    while (true) {
        int right = 0;
        _indxsSize = _indxs.size();
        for (int i = 1; i < _indxsSize; i++) {
            if (rotation(dots[res_indx[res_indx.size() - 1]],
                    dots[_indxs[right]],
                    dots[_indxs[i]])
                < 0) {
                right = i;
            }
        }
        if (_indxs[right] == res_indx[0]) {
            break;
        } else {
            res_indx.push_back(_indxs[right]);
            _indxs.erase(_indxs.begin() + right);
        }
    }

    std::vector<std::pair<int, int>> res(res_indx.size());
    int res_indxSize = res_indx.size();
    for (int i = 0; i < res_indxSize; i++) {
        res[i] = dots[res_indx[i]];
    }

    return res;
}

void Build_mpi_type(int* index_i, int* index_j, MPI_Datatype* mytype) {
    int array_of_blocklengths[2] = { 1, 1 };
    MPI_Datatype array_of_types[2] = { MPI_INT, MPI_INT };
    MPI_Aint i_addr, j_addr;
    MPI_Aint array_of_displacements[2] = { 0 };
    MPI_Get_address(index_i, &i_addr);
    MPI_Get_address(index_j, &j_addr);
    array_of_displacements[1] = j_addr - i_addr;
    MPI_Type_create_struct(2, array_of_blocklengths, array_of_displacements, array_of_types, mytype);
    MPI_Type_commit(mytype);
}

std::vector<std::pair<int, int>> jarvis_par(const std::vector<std::pair<int, int>>& dots, int size) {
    int num_of_proc;
    int proc_num;
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_num);

    std::vector<int> num_of_dots(num_of_proc);
    std::vector<int> displs(num_of_proc);
    std::vector<int> displs1(num_of_proc);
    std::vector<int> size_of_parts(num_of_proc);
    int sum = 0;

    int* index_i = NULL;
    int* index_j = NULL;
    std::pair<int, int> bla;
    index_i = &(bla.first);
    index_j = &(bla.second);
    MPI_Datatype mytype;
    Build_mpi_type(index_i, index_j, &mytype);

    for (int i = 0; i < num_of_proc; i++) {
        num_of_dots[i] = size / num_of_proc;
        displs[i] = sum;
        sum += num_of_dots[i];
    }
    if (size % num_of_proc > 0) {
        num_of_dots[num_of_proc - 1] += size % num_of_proc;
    }
    std::vector<std::pair<int, int>> vec_part(num_of_dots[proc_num]);
    std::vector<std::pair<int, int>> part_res;
    std::vector<std::pair<int, int>> res;
    int res_size = 0;

    MPI_Scatterv(dots.data(), num_of_dots.data(), displs.data(), mytype, vec_part.data(),
        num_of_dots[proc_num], mytype, 0, MPI_COMM_WORLD);

    part_res = jarvis_seq(vec_part);

    int part_size = part_res.size();
    MPI_Gather(&part_size, 1, MPI_INT, size_of_parts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(size_of_parts.data(), size_of_parts.size(), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Reduce(&size_of_parts[proc_num], &res_size, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(&res_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    sum = 0;
    for (int i = 0; i < num_of_proc; i++) {
        displs1[i] = sum;
        sum += size_of_parts[i];
    }
    std::vector<std::pair<int, int>> tmp_res(res_size);
    MPI_Gatherv(part_res.data(), size_of_parts[proc_num], mytype, tmp_res.data(),
        size_of_parts.data(), displs1.data(), mytype, 0, MPI_COMM_WORLD);
    res = jarvis_seq(tmp_res);
    return res;
}
