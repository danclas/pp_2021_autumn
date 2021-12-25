// Copyright 2021 Gordeev Viktor
#include "./strongin_mpi.h"
#include <mpi.h>
#include <algorithm>

bool operator<(const block& i1, const block& i2) {
    return (i1.R < i2.R) ? true : false;
}

bool operator>(const block& i1, const block& i2) {
    return (i1.R > i2.R) ? true : false;
}

double R(const double& _m_small, const double& _z_curr,
    const double& _z_prev, const double& _x_curr, const double& _x_prev) {
    return _m_small * (_x_curr - _x_prev) + (pow(_z_curr - _z_prev, 2)
        / (_m_small * (_x_curr - _x_prev))) - (2 * (_z_curr + _z_prev));
}

double M(const double& _z_curr, const double& _z_prev,
    const double& _x_curr, const double& _x_prev) {
    return abs((_z_curr - _z_prev) / (_x_curr - _x_prev));
}

void StronginSeq(double left, double right, double r, int procs, int rank) {
    double m_small = 1., M_big;
    block blockInit;

    double len = right - left;
    double li = len / procs;
    double tmp = 0.;

    tmp = (-len) + (rank * li);
    blockInit.x_left = tmp;
    blockInit.x_right = tmp + li;
    blockInit.z_left = func(tmp);
    blockInit.z_right = func(tmp + li);
    blockInit.M = M(blockInit.z_right, blockInit.z_left,
        blockInit.x_right, blockInit.x_left);

    MPI_Allreduce(&blockInit.M, &M_big, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    m_small = (M_big == 0) ? 1 : r * M_big;

    blockInit.R = R(m_small, blockInit.z_right, blockInit.z_left,
        blockInit.x_right, blockInit.x_left);

    double outBuffer[1] = { 0 };
    outBuffer[0] = blockInit.x_left;
    MPI_Send(outBuffer, 1, MPI_DOUBLE, 0, XLEFT, MPI_COMM_WORLD);
    outBuffer[0] = blockInit.x_right;
    MPI_Send(outBuffer, 1, MPI_DOUBLE, 0, XRIGHT, MPI_COMM_WORLD);
    outBuffer[0] = blockInit.z_left;
    MPI_Send(outBuffer, 1, MPI_DOUBLE, 0, ZLEFT, MPI_COMM_WORLD);
    outBuffer[0] = blockInit.z_right;
    MPI_Send(outBuffer, 1, MPI_DOUBLE, 0, ZRIGHT, MPI_COMM_WORLD);
    outBuffer[0] = blockInit.M;
    MPI_Send(outBuffer, 1, MPI_DOUBLE, 0, MSEND, MPI_COMM_WORLD);
    outBuffer[0] = blockInit.R;
    MPI_Send(outBuffer, 1, MPI_DOUBLE, 0, RSEND, MPI_COMM_WORLD);
}

double StronginParallel(double left, double right, const double _Epsilon,
    const int _Steps, double r, int procs) {
    int k = 0;
    double eps = _Epsilon;
    double curr_left, curr_right;
    double m_small = 1., M_big;
    double* x_new = new double[procs];
    double solutionZ = 0;
    MPI_Status status;
    double inBuffer[1] = { 0 };
    int number;

    curr_left = left;
    curr_right = right;

    std::vector<block> InitVector(procs);
    std::vector<block> WorkVector(procs * 2);
    std::priority_queue<block> Queue;
    std::list<block> Intervals;

    int size_bank_intervals;

    std::list<block>::iterator Pointer;
    double len = right - left;
    double li = len / procs;
    double tmp = 0.;

    tmp = (-len);
    InitVector[0].x_left = tmp;
    InitVector[0].x_right = tmp + li;
    InitVector[0].z_left = func(tmp);
    InitVector[0].z_right = func(tmp + li);
    InitVector[0].M = M(InitVector[0].z_right, InitVector[0].z_left,
        InitVector[0].x_right, InitVector[0].x_left);

    MPI_Allreduce(&InitVector[0].M, &M_big, 1,
        MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    m_small = (M_big == 0) ? 1 : r * M_big;

    InitVector[0].R = R(m_small, InitVector[0].z_right, InitVector[0].z_left,
        InitVector[0].x_right, InitVector[0].x_left);

    Intervals.push_back(InitVector[0]);
    for (int i = 0; i < (procs - 1) * 6; i++) {
        MPI_Recv(inBuffer, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
            MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        number = status.MPI_SOURCE;
        if (status.MPI_TAG == XRIGHT) {
            InitVector[number].x_right = inBuffer[0];
        }
        if (status.MPI_TAG == XLEFT) {
            InitVector[number].x_left = inBuffer[0];
        }
        if (status.MPI_TAG == ZRIGHT) {
            InitVector[number].z_right = inBuffer[0];
        }
        if (status.MPI_TAG == ZLEFT) {
            InitVector[number].z_left = inBuffer[0];
        }
        if (status.MPI_TAG == MSEND) {
            InitVector[number].M = inBuffer[0];
        }
        if (status.MPI_TAG == RSEND) {
            InitVector[number].R = inBuffer[0];
        }
    }
    for (int i = 0; i < procs; i++) {
        Intervals.push_back(InitVector[i]);
    }
    Intervals.sort();

    curr_left = Intervals.back().x_left;
    curr_right = Intervals.back().x_right;

    ++k;
    while (k < _Steps && (abs(curr_left - curr_right) > eps)) {
        Pointer = --Intervals.end();
        for (int i = 0; i < procs * 2 - 2; i += 2) {
            WorkVector[i] = *Pointer;
            Intervals.erase(Pointer--);
        }
        WorkVector[procs * 2 - 2] = *Pointer;
        Intervals.erase(Pointer);
        size_bank_intervals = Intervals.size();
        for (int i = 0; i < procs; i++) {
            x_new[i] = 0.5 * (WorkVector[i * 2].x_right
                + WorkVector[i * 2].x_left)
                - (WorkVector[i * 2].z_right - WorkVector[i * 2].z_left)
                / (2 * m_small);
            WorkVector[i * 2 + 1].z_left = func(x_new[i]);
        }
        for (int i = 0; i < procs * 2; i += 2) {
            WorkVector[i + 1].x_right = WorkVector[i].x_right;
            WorkVector[i + 1].z_right = WorkVector[i].z_right;
            WorkVector[i + 1].x_left = WorkVector[i].x_right = x_new[i / 2];
            WorkVector[i].z_right = WorkVector[i + 1].z_left;
        }
        for (int i = 0; i < procs * 2; i++) {
            WorkVector[i].M = M(WorkVector[i].z_right, WorkVector[i].z_left,
                WorkVector[i].x_right, WorkVector[i].x_left);
        }

        double M_max_array = WorkVector[0].M;
        {
            {
                for (int i = 1; i < procs * 2; i++) {
                    if (M_max_array < WorkVector[i].M)
                        M_max_array = WorkVector[i].M;
                }
            }
            {
                if (size_bank_intervals != 0) {
                    Pointer = Intervals.begin();
                    double tmp_M;
                    double M_max_list = (*Pointer++).M;
                    for (; Pointer != Intervals.end(); ++Pointer) {
                        tmp_M = (*Pointer).M;
                        if (M_max_list < tmp_M)
                            M_max_list = tmp_M;
                    }
                    M_big = (M_max_array < M_max_list)
                        ? M_max_list : M_max_array;
                } else {
                    M_big = M_max_array;
                }
            }
        }
        m_small = (M_big == 0) ? 1 : r * M_big;
        for (int i = 0; i < procs * 2; i++) {
            WorkVector[i].R = R(m_small, WorkVector[i].z_right,
                WorkVector[i].z_left, WorkVector[i].x_right,
                WorkVector[i].x_left);
        }
        if (size_bank_intervals != 0) {
            std::vector<block> R_vec(Intervals.begin(), Intervals.end());
            for (int i = 0; i < size_bank_intervals; i++) {
                R_vec[i].R = R(m_small, R_vec[i].z_right, R_vec[i].z_left,
                    R_vec[i].x_right, R_vec[i].x_left);
            }
            int j = 0;
            for (auto i = Intervals.begin(); i != Intervals.end(); i++) {
                i->R = R_vec[j++].R;
            }
            R_vec.clear();
        }
        for (int i = 0; i < procs * 2; i++) {
            Intervals.push_back(WorkVector[i]);
        }
        Intervals.sort();
        ++k;
        curr_left = Intervals.back().x_left;
        curr_right = Intervals.back().x_right;
    }
    solutionZ = Intervals.back().z_right;
    WorkVector.clear();
    InitVector.clear();
    delete[] x_new;

    return solutionZ;
}
double func(const double& _x) {
    return 2 * sin(3 * _x) + 3 * cos(5 * _x);
}
