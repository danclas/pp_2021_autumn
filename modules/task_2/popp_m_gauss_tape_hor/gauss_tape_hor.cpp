// Copyright 2021 Popp Maksim
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/popp_m_gauss_tape_hor/gauss_tape_hor.h"


std::vector<double> sleGenerate(std::vector<double>* sle,  std::vector<double>*res, std::size_t num) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<> urd(-50, 50);
    std::vector<double> sln(num, 0);

    for (std::size_t i = 0; i < num; i++) {
        auto random = urd(gen);
        sln[i] = random == 0.0 ? 1.0 : random;
        for (std::size_t j = 0; j < num; j++) {
            (*sle)[i * num + j] = urd(gen);
        }
    }

    for (std::size_t i = 0; i < num; i++) {
        (*res)[i] = 0;
        for (std::size_t j = 0; j < num; j++) {
            (*res)[i] += (*sle)[i * num + j] * sln[j];
        }
    }

    return sln;
}

std::vector<double> sequential(const std::vector<double>& sle, const std::vector<double>& res, std::size_t num) {
    std::vector<double> mod_sle(sle);
    std::vector<double> mod_vec_res(res);
    std::vector<double> sln(num, 0);

    for (std::size_t i = 0; i < num; i++) {
        for (std::size_t j = i + 1; j < num; j++) {
            double coeff = mod_sle[j * num + i] / mod_sle[i * num + i];
            mod_vec_res[j] -= mod_vec_res[i] * coeff;
            for (std::size_t k = i + 1; k < num; k++) {
                mod_sle[j * num + k] -= mod_sle[i * num + k] * coeff;
            }
            mod_sle[j * num + i] = 0;
        }
    }

    for (auto i = num - 1; i > 0; i--) {
        double coeff = mod_vec_res[i];
        for (auto j = num - 1; j > i; j--) {
            coeff -= mod_sle[i * num + j] * sln[j];
        }
        sln[i] = coeff / mod_sle[i * num + i];
    }

    double coeff = mod_vec_res[0];
    for (std::size_t i = 1; i < num; i++) {
        coeff -= mod_sle[i] * sln[i];
    }

    sln[0] = coeff / mod_sle[0];

    return sln;
}

std::vector<double> parallel(const std::vector<double>& sle, const std::vector<double>& res, std::size_t num) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size > static_cast<int>(num) || size == 1) {
        return rank == 0 ? sequential(sle, res, num) :
            std::vector<double>();
    }

    std::size_t size_local = num / size;
    std::size_t proc0_remains = num % size;

    int m_tag = 0;
    int v_tag = 1;
    if (rank == 0) {
        for (int num_proc = 1; num_proc < size; num_proc++) {
            std::size_t offset_sle = (size_local * num_proc + proc0_remains) * num;
            std::size_t offset_vec_res = size_local * num_proc + proc0_remains;

            MPI_Send(sle.data() + offset_sle, static_cast<int>(size_local * num),
                MPI_DOUBLE, num_proc, m_tag, MPI_COMM_WORLD);
            MPI_Send(res.data() + offset_vec_res, static_cast<int>(size_local),
                MPI_DOUBLE, num_proc, v_tag, MPI_COMM_WORLD);
        }
    }

    std::vector<double>local_m_sle((size_local + proc0_remains) * num);
    std::vector<double>local_v_res(size_local + proc0_remains);

    if (rank == 0) {
        for (std::size_t i = 0; i < size_local + proc0_remains; i++) {
            local_v_res[i] = res[i];
            for (std::size_t j = 0; j < num; j++) {
                local_m_sle[i * num + j] = sle[i * num + j];
            }
        }
    } else {
        MPI_Status status;
        MPI_Recv(local_m_sle.data(), static_cast<int>(size_local * num),
            MPI_DOUBLE, 0, m_tag, MPI_COMM_WORLD, &status);
        MPI_Recv(local_v_res.data(), static_cast<int>(size_local),
            MPI_DOUBLE, 0, v_tag, MPI_COMM_WORLD, &status);
    }

    std::vector<double> recivied_sle((size_local + proc0_remains) * num);
    std::vector<double> recivied_vector(size_local + proc0_remains);

    for (int iter = 0; iter <= rank; iter++) {
        if (iter == rank) {
            auto offset = iter == 0 ? 0 : iter * size_local + proc0_remains;
            auto current_size = iter == 0 ? size_local + proc0_remains : size_local;
            for (std::size_t i = 0; i < current_size; i++) {
                for (std::size_t j = i + 1; j < current_size; j++) {
                    double coeff = local_m_sle[j * num + i + offset] /
                        local_m_sle[i * num + i + offset];
                    local_v_res[j] -= local_v_res[i] * coeff;
                    for (std::size_t k = i + offset + 1; k < num; k++) {
                        local_m_sle[j * num + k] -= local_m_sle[i * num + k] * coeff;
                    }
                    local_m_sle[j * num + i + offset] = 0;
                }
            }

            for (int num_proc = iter + 1; num_proc < size; num_proc++) {
                MPI_Send(local_m_sle.data(), static_cast<int>(local_m_sle.size()),
                    MPI_DOUBLE, num_proc, m_tag, MPI_COMM_WORLD);
                MPI_Send(local_v_res.data(), static_cast<int>(local_v_res.size()),
                    MPI_DOUBLE, num_proc, v_tag, MPI_COMM_WORLD);
            }
        }  else {
            MPI_Status status;
            MPI_Recv(recivied_sle.data(), static_cast<int>(recivied_sle.size()),
                MPI_DOUBLE, iter, m_tag, MPI_COMM_WORLD, &status);
            MPI_Recv(recivied_vector.data(), static_cast<int>(recivied_vector.size()),
                MPI_DOUBLE, iter, v_tag, MPI_COMM_WORLD, &status);

            auto offset = iter == 0 ? 0 : size_local + proc0_remains + iter * size_local - size_local;
            auto size_reciv_sle = iter == 0 ? size_local + proc0_remains : size_local;

            for (std::size_t i = 0; i < size_local; i++) {
                for (std::size_t j = 0; j < size_reciv_sle; j++) {
                    double coeff = local_m_sle[i * num + j + offset] /
                        recivied_sle[j * num + j + offset];
                    local_v_res[i] -= recivied_vector[j] * coeff;
                    for (std::size_t k = j + offset + 1; k < num; k++) {
                        local_m_sle[i * num + k] -= recivied_sle[j * num + k] * coeff;
                    }
                    local_m_sle[i * num + j + offset] = 0;
                }
            }
        }
    }
    std::vector<double> sln(num, 0);

    int sender = rank + 1;
    int receiver = rank - 1;
    int sln_tag = 2;
    if (rank != size - 1) {
        MPI_Status status;

        MPI_Recv(sln.data(), static_cast<int>(num),
            MPI_DOUBLE, sender, sln_tag, MPI_COMM_WORLD, &status);
    }

    auto offset = ((size - 1) - rank) * size_local;
    if (rank == 0) {
        size_local += proc0_remains;
    }

    for (std::size_t i = 0; i < size_local; i++) {
        double coeff = local_v_res[size_local - 1 - i];
        for (std::size_t j = 0; j < i + offset; j++) {
            coeff -= local_m_sle[size_local * num - 1 - i * num - j] *
                sln[num - 1 - j];
        }
        sln[num - 1 - i - offset] = coeff /
            local_m_sle[size_local * num - 1 - num * i - i - offset];
    }

    if (rank != 0) {
        MPI_Send(sln.data(), static_cast<int>(num),
            MPI_DOUBLE, receiver, sln_tag, MPI_COMM_WORLD);
    }

    return sln;
}
