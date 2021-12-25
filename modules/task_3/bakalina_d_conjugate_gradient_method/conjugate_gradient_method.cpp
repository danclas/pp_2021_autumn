// Copyright 2021 Bakalina Darya
#include <mpi.h>
#include <iostream>
#include <vector>
#include <random>
#include "../../../modules/task_3/bakalina_d_conjugate_gradient_method/conjugate_gradient_method.h"

std::vector<double> create_random_vector(int size_n) {
    if (size_n <= 0) {
        throw "Wrong size";
    }
    std::vector <double> v(size_n);
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < size_n; ++i) {
        v[i] = gen() % 100;
    }
    return v;
}

std::vector<double> create_random_matrix(int size_n) {
    if (size_n <= 0) {
        throw " Wrong size";
    }
    std::vector <double> mtrx(size_n * size_n);
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < size_n; ++i) {
        for (int j = i; j < size_n; ++j) {
            mtrx[i * size_n + j] = mtrx[j * size_n + i] = gen() % 100;
        }
    }
    return mtrx;
}
double scalar_multiply(const std::vector<double>& vctr_x, const std::vector<double>& vctr_y) {
    double sclr_res = 0;
    for (size_t i = 0; i < vctr_x.size(); ++i) {
        sclr_res += vctr_x[i] * vctr_y[i];
    }
    return sclr_res;
}

std::vector<double> multiply_mtrx_to_v(const std::vector<double>& mtrx, const std::vector<double>& v) {
    std::vector<double> lin_res(mtrx.size() / v.size());
    for (size_t i = 0; i < mtrx.size() / v.size(); ++i) {
        lin_res[i] = 0;
        for (size_t j = 0; j < v.size(); ++j) {
            lin_res[i] += mtrx[i * v.size() + j] * v[j];
        }
    }
    return lin_res;
}

std::vector<double> lin_gradient_method(const std::vector<double>& mtrx, const std::vector<double>& v, int size_n) {
    std::vector<double> r_cur(size_n);
    std::vector<double> r_nxt(size_n);
    int flag = 0;
    double prov_sqrt = 0;
    double epsilon = 0.01, b = 0.0, alfa = 0.0;
    std::vector<double> lin_res(size_n);
    for (int i = 0; i < size_n; i++) {
        lin_res[i] = 1;
    }
    std::vector<double> A = multiply_mtrx_to_v(mtrx, lin_res);
    for (int i = 0; i < size_n; i++) {
        r_cur[i] = v[i] - A[i];
    }
    std::vector<double> p(r_cur);
    do {
        flag++;
        A = multiply_mtrx_to_v(mtrx, p);
        alfa = scalar_multiply(r_cur, r_cur) / scalar_multiply(p, A);
        for (int i = 0; i < size_n; i++) {
            lin_res[i] += alfa * p[i];
            r_nxt[i] = r_cur[i] - alfa * A[i];
        }
        b = scalar_multiply(r_nxt, r_nxt) / scalar_multiply(r_cur, r_cur);
        prov_sqrt = sqrt(scalar_multiply(r_nxt, r_nxt));
        for (int j = 0; j < size_n; j++) {
            p[j] = r_nxt[j] + b * p[j];
        }
        r_cur = r_nxt;
    } while ((prov_sqrt > epsilon) && (flag <= size_n));
    return lin_res;
}

std::vector<double> parall_gradient_method(const std::vector<double>& mtrx, const std::vector<double>& v, int size_n) {
    int ProcNum = 0, ProcRank = 0;
    int flag = 0;
    double prov_sqrt = 0;
    double epsilon = 0.01, b = 0.0, alfa = 0.0;
    double r_nxtSclr;
    double rcvbufF_curr, rcvbufF_nxt, r_currSclr, znmtl;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<double> mtrx_parl = mtrx;
    std::vector<double> v_parl = v;
    std::vector<double> parl_res(size_n);
    for (int i = 0; i < size_n; i++) {
        parl_res[i] = 1;
    }
    int quotient = size_n / ProcNum;
    int resd = size_n % ProcNum;
    MPI_Bcast(mtrx_parl.data(), size_n * size_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(v_parl.data(), size_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::vector<double> mtrx_A0(quotient * size_n);
    if (ProcRank == 0) {
        if (resd != 0) {
            mtrx_A0.resize(size_n * quotient + resd * size_n);
        }
        if (quotient != 0) {
            for (int ProcCount = 1; ProcCount < ProcNum; ProcCount++) {
                MPI_Send(&mtrx_parl[0] + ProcCount * quotient * size_n + resd * size_n,
                    quotient * size_n, MPI_DOUBLE, ProcCount, 1, MPI_COMM_WORLD);
            }
        }
    }
    if (ProcRank == 0) {
        if (resd != 0) {
            for (int i = 0; i < size_n * quotient + size_n * resd; i++) {
                mtrx_A0[i] = mtrx_parl[i];
            }
        } else {
            for (int i = 0; i < size_n * quotient; i++) {
                mtrx_A0[i] = mtrx_parl[i];
            }
        }
    } else {
        MPI_Status status;
        if (quotient != 0) {
            MPI_Recv(&mtrx_A0[0], quotient * size_n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        }
    }
    std::vector<double> A = multiply_mtrx_to_v(mtrx_A0, parl_res);
    std::vector<double> r_cur(quotient), r_nxt(quotient);
    std::vector<double> p(size_n);
    if (ProcRank == 0) {
        if (resd != 0) {
            r_cur.resize(quotient + resd);
            r_nxt.resize(quotient + resd);
        }
        for (int i = 0; i < quotient + resd; i++)
            r_cur[i] = v_parl[i] - A[i];
    } else {
        for (int i = 0; i < quotient; i++)
            r_cur[i] = v_parl[ProcRank * quotient + resd + i] - A[i];
    }
    if (ProcRank == 0) {
        if (resd != 0) {
            for (int i = 0; i < quotient + resd; i++) {
                p[i] = r_cur[i];
            }
        } else {
            for (int i = 0; i < quotient; i++) {
                p[i] = r_cur[i];
            }
        }
        if (quotient != 0) {
            MPI_Status status;
            for (int ProcCount = 1; ProcCount < ProcNum; ProcCount++) {
                MPI_Recv(&p[0] + ProcCount * quotient + resd,
                    quotient, MPI_DOUBLE, ProcCount, 2, MPI_COMM_WORLD, &status);
            }
        }
    } else {
        if (quotient != 0) {
            MPI_Send(&r_cur[0], quotient, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        }
    }
    MPI_Bcast(p.data(), size_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::vector<double> p_part(quotient);
    std::vector<double> p_res(quotient);
    if (ProcRank == 0) {
        if (resd != 0) {
            p_part.resize(quotient + resd);
        }
    }
    do {
        flag++;
        A = multiply_mtrx_to_v(mtrx_A0, p);
        if (ProcRank == 0) {
            for (int i = 0; i < quotient + resd; i++) {
                p_part[i] = p[i];
            }
        } else {
            for (int i = 0; i < quotient; i++) {
                p_part[i] = p[ProcRank * quotient + resd + i];
            }
        }
        rcvbufF_curr = scalar_multiply(r_cur, r_cur);
        rcvbufF_nxt = scalar_multiply(p_part, A);
        MPI_Allreduce(&rcvbufF_curr, &r_currSclr, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(&rcvbufF_nxt, &znmtl, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        alfa = r_currSclr / znmtl;
        for (int i = 0; i < size_n; i++) {
            parl_res[i] += alfa * p[i];
        }
        if (ProcRank == 0) {
            for (int i = 0; i < quotient + resd; i++) {
                r_nxt[i] = r_cur[i] - alfa * A[i];
            }
        } else {
            for (int i = 0; i < quotient; i++) {
                r_nxt[i] = r_cur[i] - alfa * A[i];
            }
        }
        rcvbufF_curr = scalar_multiply(r_nxt, r_nxt);
        MPI_Allreduce(&rcvbufF_curr, &r_nxtSclr, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        b = r_nxtSclr / r_currSclr;
        prov_sqrt = sqrt(r_nxtSclr);
        if (ProcRank == 0) {
            for (int i = 0; i < quotient + resd; i++) {
                p[i] = r_nxt[i] + b * p[i];
            }
            if (quotient != 0) {
                MPI_Status status;
                for (int ProcCount = 1; ProcCount < ProcNum; ProcCount++) {
                    MPI_Recv(&p[0] + ProcCount * quotient + resd,
                        quotient, MPI_DOUBLE, ProcCount, 3, MPI_COMM_WORLD, &status);
                }
            }
        } else {
            for (int i = 0; i < quotient; i++) {
                p_res[i] = r_nxt[i] + b * p[ProcRank * quotient + resd + i];
            }
            if (quotient != 0) {
                MPI_Send(&p_res[0], quotient, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
            }
        }
        MPI_Bcast(p.data(), size_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        r_cur = r_nxt;
    } while ((prov_sqrt > epsilon) && (flag <= size_n));
    return parl_res;
}
