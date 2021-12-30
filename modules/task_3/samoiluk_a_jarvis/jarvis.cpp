// Copyright 2021 Samoiluk Anastasiya
#include <mpi.h>
#include <random>
#include "../../modules/task_3/samoiluk_a_jarvis/jarvis.h"

int Rotate(const Point& first, const Point& second, const Point& third) {
    return (second.x - first.x) * (third.y - second.y)
        - (second.y - first.y) * (third.x - second.x);
}

std::vector<Point> JarvisMarch_seq(const std::vector<Point>& vec) {
    std::vector<int> v_num(vec.size());

    int v_num_size = v_num.size();
    for (int i = 0; i < v_num_size; i++) {
        v_num[i] = i;
    }

    for (std::vector<int>::size_type i = 1; i < vec.size(); i++) {
        if (vec[v_num[i]].x < vec[v_num[0]].x) {
            std::swap(v_num[i], v_num[0]);
        }
    }

    std::vector<int> h = { v_num[0] };
    v_num.erase(v_num.begin());
    v_num.push_back(h[0]);

    while (true) {
        int right = 0;
        for (std::vector<int>::size_type i = 1; i < v_num.size(); i++) {
            if (Rotate(vec[h[h.size() - 1]], vec[v_num[right]],
                vec[v_num[i]]) < 0) {
                right = i;
            }
        }
        if (v_num[right] == h[0]) {
            break;
        } else {
            h.push_back(v_num[right]);
            v_num.erase(v_num.begin() + right);
        }
    }
    std::vector<Point> res_vec(h.size());
    for (std::vector<int>::size_type i = 0; i < h.size(); i++) {
        res_vec[i] = vec[h[i]];
    }
    return res_vec;
}

void createStructPoint(MPI_Datatype* structPoint) {
    int num_f = 2;
    int blocklengths[2] = { 1, 1 };
    MPI_Datatype types[2] = { MPI_INT, MPI_INT };
    MPI_Aint offsets[2] = {offsetof(Point, x), offsetof(Point, y) };
    MPI_Type_create_struct(num_f, blocklengths, offsets, types, structPoint);
    MPI_Type_commit(structPoint);
}

std::vector<Point> JarvisMarch_par(const std::vector<Point>& vec,
    std::vector<int>::size_type vec_size) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype structPoint;
    createStructPoint(&structPoint);

    int p_num;
    int extra_p;
    int v_size = vec_size;
    if (size > v_size) {
        throw -1;
    } else {
        p_num = vec_size / size;
        extra_p = vec_size - size * p_num;
    }

    std::vector<Point> local_vec(p_num);
    std::vector<Point> local_jm, global_jm;

    MPI_Scatter(vec.data(), p_num, structPoint, local_vec.data(),
        p_num, structPoint, 0, MPI_COMM_WORLD);
    local_jm = JarvisMarch_seq(local_vec);

    if (rank != 0) {
        MPI_Send(local_jm.data(), local_jm.size(), structPoint, 0,
            rank, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < size; i++) {
            MPI_Status status;
            int count = 0;
            MPI_Probe(i, i, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, structPoint, &count);
            std::vector<int>::size_type loc_size = local_jm.size();
            local_jm.resize(loc_size + count);
            MPI_Recv(local_jm.data() + loc_size, count, structPoint, i,
                i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (size != 1) {
            if (extra_p != 0) {
                local_vec = std::vector<Point>(vec.begin() +
                    (vec.size() - extra_p), vec.end());
                global_jm = JarvisMarch_seq(local_vec);
                std::vector<int>::size_type loc_size = local_jm.size();
                local_jm.resize(loc_size + global_jm.size());
                for (std::vector<int>::size_type i = loc_size, j = 0;
                    i < local_jm.size(); i++, j++) {
                    local_jm[i] = global_jm[j];
                }
            }
        }
        global_jm = JarvisMarch_seq(local_jm);
    }
    return global_jm;
}

std::vector<Point> getRandomVec(std::vector<int>::size_type size) {
    std::vector<Point> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    for (std::vector<int>::size_type i = 0; i < size; i++) {
        vec[i] = Point(gen() % 10000, gen() % 10000);
    }
    return vec;
}
