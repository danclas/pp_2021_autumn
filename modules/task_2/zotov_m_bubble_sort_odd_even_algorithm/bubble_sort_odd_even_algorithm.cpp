// Copyright 2021 Zotov Maksim
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include "../../../modules/task_2/zotov_m_bubble_sort_odd_even_algorithm/bubble_sort_odd_even_algorithm.h"


std::vector<int> BubbleSortSequential(std::vector<int> pData) {
    int size = pData.size();
    int upper_bound;
    if (size % 2 == 0) {
        upper_bound = size / 2 - 1;
    } else {
        upper_bound = size / 2;
    }
    for (int i = 0; i < size; i++) {
        if (i % 2 == 0) {
            for (int j = 0; j < size / 2; j++) {
                if (pData[2 * j] > pData[2 * j + 1])
                    std::swap(pData[2 * j], pData[2 * j + 1]);
            }
        } else {
            for (int j = 0; j < upper_bound; j++) {
                if (pData[2 * j + 1] > pData[2 * j + 2])
                    std::swap(pData[2 * j + 1], pData[2 * j + 2]);
            }
         }
    }
    return pData;
}

std::vector<int> getRandomVector(int size) {
    std::mt19937 generate;
    generate.seed(time(0));
    std::uniform_real_distribution<> uid(0, RAND_MAX);
    std::vector<int> tmp;
    for (int i = 0; i < size; i++) {
        tmp.push_back(uid(generate));
    }
    return tmp;
}

void mergAndSort(std::vector<int>* const firstArr, std::vector<int>* const secondArr) {
    std::vector<int> tmp;
    tmp.insert(tmp.end(), (*firstArr).begin(), (*firstArr).end());
    tmp.insert(tmp.end(), (*secondArr).begin(), (*secondArr).end());
    sort(tmp.begin(), tmp.end());
    for (int i = 0; i < static_cast<int>(tmp.size()); i++) {
        if (i < static_cast<int>((*firstArr).size())) {
            (*firstArr)[i] = tmp[i];
        } else {
            (*secondArr)[i - (*firstArr).size()] = tmp[i];
        }
    }
}


std::vector<int> BubbleSortParallel(std::vector<int> pData) {
    int ProcRank, ProcNum;
    int data_size = 0, my_size;
    int remain, sum;
    std::vector<int> my_data, right_neighbor_data;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcRank == 0) {
        data_size = pData.size();
    }

    MPI_Bcast(&data_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    my_size = data_size / ProcNum;
    remain = data_size % ProcNum;

    std::vector <int> sendCount(ProcNum);
    std::vector <int> displ;

    if (ProcRank == 0) {
        sum = 0;
        for (int i = 0; i < ProcNum; i++) {
            sendCount[i] = my_size;
            if (remain > 0) {
                sendCount[i]++;
                remain--;
            }
            displ.push_back(sum);
            sum += sendCount[i];
        }
    }

    MPI_Bcast(sendCount.data(), ProcNum, MPI_INT, 0, MPI_COMM_WORLD);

    my_data.resize(sendCount[ProcRank]);

    if (ProcRank != ProcNum - 1) {
        right_neighbor_data.resize(sendCount[ProcRank + 1]);
    }

    MPI_Scatterv(pData.data(), sendCount.data(), displ.data(), MPI_INT, my_data.data(),
        sendCount[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);

    my_data = BubbleSortSequential(my_data);

    for (int i = 0; i < ProcNum; i++) {
        if (i % 2 == 1) {
            if ((ProcRank % 2 == 1 && ProcRank < ProcNum - 1)) {
                MPI_Recv(right_neighbor_data.data(), sendCount[ProcRank + 1], MPI_INT,
                    ProcRank + 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                mergAndSort(&my_data, &right_neighbor_data);
                MPI_Send(right_neighbor_data.data(), sendCount[ProcRank + 1], MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD);
            } else if (ProcRank > 0 && ProcRank % 2 == 0) {
                MPI_Send(my_data.data(), sendCount[ProcRank], MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
                MPI_Recv(my_data.data(), sendCount[ProcRank], MPI_INT, ProcRank - 1,
                    0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            }
        } else if (i % 2 == 0) {
            if (ProcRank % 2 == 0 && ProcRank < ProcNum - 1) {
                MPI_Recv(right_neighbor_data.data(), sendCount[ProcRank + 1], MPI_INT,
                    ProcRank + 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                mergAndSort(&my_data, &right_neighbor_data);
                MPI_Send(right_neighbor_data.data(), sendCount[ProcRank + 1], MPI_INT,
                    ProcRank + 1, 0, MPI_COMM_WORLD);
            } else if (ProcRank > 0 && ProcRank % 2 == 1) {
                MPI_Send(my_data.data(), sendCount[ProcRank], MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
                MPI_Recv(my_data.data(), sendCount[ProcRank], MPI_INT, ProcRank - 1,
                    0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Gatherv(my_data.data(), sendCount[ProcRank], MPI_INT, pData.data(), sendCount.data(),
        displ.data(), MPI_INT, 0, MPI_COMM_WORLD);
    return pData;
}
