// Copyright 2021 Denisova Julia
#include "../../../modules/task_3/denisova_odd_even_batcher_sort/odd_even_batcher_sort.h"

std::vector<std::pair<int, int>> proc_allocation;

std::vector<int> random_vector(int size) {
    std::mt19937 rand_r(time(0));
    std::vector<int> v(size);
    for (int i = 0; i < size; i++) {
        v[i] = rand_r() % 1000;
    }
    return v;
}

int partition(std::vector<int>* v, int l, int r) {
    int pivot = (*v)[r];
    int i = (l - 1);
    int tmp = 0;
    for (int j = l; j <= r - 1; j++) {
        if ((*v)[j] <= pivot) {
            i++;
            tmp = (*v)[i];
            (*v)[i] = (*v)[j];
            (*v)[j] = tmp;
        }
    }
    std::swap((*v)[i + 1], (*v)[r]);
    return (i + 1);
}

void q_sort(std::vector<int>* v, int l, int r) {
    if (l < r) {
        int c = partition(v, l, r);
        q_sort(v, l, c - 1);
        q_sort(v, c + 1, r);
    }
}



void odd_even_merge(std::vector<int> l, std::vector<int> r) {
    int size = l.size() + r.size();
    int _i = 0;
    std::vector<int> l_odd;
    std::vector<int> l_even;
    std::vector<int> r_odd;
    std::vector<int> r_even;
    std::vector<int> res;
    if (size <= 1) {
        return;
    }
    if (size == 2) {
        proc_allocation.push_back(std::pair<int, int>(l[0], r[0]));
        return;
    }
    for (unsigned int i = 0; i < l.size(); i++) {
        if (i % 2) {
            l_odd.push_back(l[i]);
        } else {
            l_even.push_back(l[i]);
        }
    }
    for (unsigned int i = 0; i < r.size(); i++) {
        if (i % 2) {
            r_odd.push_back(r[i]);
        } else {
            r_even.push_back(r[i]);
        }
    }
    odd_even_merge(l_odd, r_odd);
    odd_even_merge(l_even, r_even);
    for (unsigned int i = 0; i < l.size(); i++) {
        res.push_back(l[i]);
    }
    for (unsigned int i = 0; i < r.size(); i++) {
        res.push_back(r[i]);
    }
    for (int i = 1; i < size - 1; i += 2) {
        _i = i + 1;
        proc_allocation.push_back(std::pair<int, int>(res[i], res[_i]));
    }
}

void allocation(std::vector<int> v) {
    int size = v.size();
    if (size <= 1)
        return;
    std::vector<int> l(v.begin(), v.begin() + size / 2);
    std::vector<int> r(v.begin() + size / 2, v.end());
    allocation(l);
    allocation(r);
    odd_even_merge(l, r);
}

void build_allocation(int procNum) {
    std::vector<int> v;
    for (int i = 0; i < procNum; i++) {
        v.push_back(i);
    }
    allocation(v);
}

void q_sort_batcher(std::vector<int>* v) {
    int procRank;
    int procCount;
    int count = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (static_cast<unsigned int>(procCount) >= v->size()) {
        if (procRank == 0) {
            q_sort(v, 0, v->size() - 1);
        }
        return;
    }
    build_allocation(procCount);
    if (procRank == 0) {
        while (true) {
            if (v->size() % procCount) {
                count++;
                v->push_back(1337);
            } else {
                break;
            }
        }
    }
    int enlarged_size = v->size();
    MPI_Bcast(&enlarged_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int size_of_part = enlarged_size / procCount;
    std::vector<int> part(size_of_part);
    std::vector<int> tmp(size_of_part);
    std::vector<int> near_part(size_of_part);
    MPI_Scatter(&(*v)[0], size_of_part, MPI_INT, &part[0], size_of_part, MPI_INT, 0, MPI_COMM_WORLD);
    q_sort(&part, 0, part.size() - 1);
    for (unsigned int i = 0; i < proc_allocation.size(); i++) {
        if (procRank == proc_allocation[i].second) {
            MPI_Status status;
            MPI_Recv(&near_part[0], size_of_part, MPI_INT, proc_allocation[i].first, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&part[0], size_of_part, MPI_INT, proc_allocation[i].first, 0, MPI_COMM_WORLD);
            int start = size_of_part - 1;
            int curr_pos = start;
            int nearby_pos = start;
            for (int j = start; j >= 0; j--) {
                int curr = part[curr_pos];
                int nearby = near_part[nearby_pos];
                if (curr > nearby) {
                    tmp[j] = curr;
                    curr_pos--;
                } else {
                    tmp[j] = nearby;
                    nearby_pos--;
                }
            }
            part = tmp;
        } else if (procRank == proc_allocation[i].first) {
            MPI_Status status;
            MPI_Send(&part[0], size_of_part, MPI_INT, proc_allocation[i].second, 0, MPI_COMM_WORLD);
            MPI_Recv(&near_part[0], size_of_part, MPI_INT, proc_allocation[i].second, 0, MPI_COMM_WORLD, &status);
            int curr_pos = 0;
            int nearby_pos = 0;
            for (int j = 0; j < size_of_part; j++) {
                int curr = part[curr_pos];
                int nearby = near_part[nearby_pos];
                if (curr < nearby) {
                    tmp[j] = curr;
                    curr_pos++;
                } else {
                    tmp[j] = nearby;
                    nearby_pos++;
                }
            }
            part = tmp;
        }
    }
    MPI_Gather(&part[0], size_of_part, MPI_INT, &(*v)[0], size_of_part, MPI_INT, 0, MPI_COMM_WORLD);
    if (procRank == 0 && count > 0)
        v->erase(v->begin() + enlarged_size - count, v->end());
    return;
}
