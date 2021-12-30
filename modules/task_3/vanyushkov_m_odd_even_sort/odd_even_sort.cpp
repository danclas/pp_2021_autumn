// Copyright 2021 Vanyushkov Maxim
#include "../../../modules/task_3/vanyushkov_m_odd_even_sort/odd_even_sort.h"
#include <random>
#include <vector>

// we will consider a vector with positive elements

void vec_gen(int* vec, int len) {
    // https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distInt(1, 0x0FFFFFFF);
    for (int i = 0; i < len; i++) {
        vec[i] = static_cast<int>(distInt(rng));
    }
}

void copy_vec(int* c_vec, int* vec, int len) {
    for (int i = 0; i < len; i++) {
        c_vec[i] = vec[i];
    }
}

// q_sort https://en.wikipedia.org/wiki/Quicksort

int partition(int* vec, int l, int r) {
    int pivot = vec[r];
    int i = (l - 1);
    for (int j = l; j <= r - 1; j++) {
        if (vec[j] <= pivot) {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }
    i++;
    std::swap(vec[i], vec[r]);
    return i;
}

void q_sort(int* vec, int l, int r) {
    if (l < r) {
        int p = partition(vec, l, r);
        q_sort(vec, l, p - 1);
        q_sort(vec, p + 1, r);
    }
}

// odd_even_sort https://habr.com/ru/post/275889/

std::vector<std::pair<int, int>> comparator;

void separation(int* odd, int* even, int* vec, int len) {
    for (int i = 0, j = 0, k = 0; i < len; i++) {
        if (i % 2) {
            odd[j++] = vec[i];
        } else {
            even[k++] = vec[i];
        }
    }
}

void _merge(int* up, int* down, int lenUp, int lenDown) {
    int len = lenUp + lenDown;
    if (len <= 1) {
        return;
    }
    if (len == 2) {
        comparator.push_back(std::pair<int, int>(*up, *down));
        return;
    }

    int* upOdd = new int[lenUp / 2];
    int* upEven = new int[lenUp - lenUp / 2];
    int* downOdd = new int[lenDown / 2];
    int* downEven = new int[lenDown - lenDown / 2];

    separation(upOdd, upEven, up, lenUp);
    separation(downOdd, downEven, down, lenDown);
    _merge(upOdd, downOdd, lenUp / 2, lenDown / 2);
    _merge(upEven, downEven, lenUp - lenUp / 2, lenDown - lenDown / 2);

    int* res = new int[len];
    copy_vec(res, up, lenUp);
    copy_vec(res + lenUp, down, lenDown);
    for (int i = 1; i < len - 1; i += 2) {
        comparator.push_back(std::pair<int, int>(res[i], res[i + 1]));
    }

    delete[] upOdd;
    delete[] upEven;
    delete[] downOdd;
    delete[] downEven;
    delete[] res;
}

void allocation(int* vec, int len);

void _alocation(int* vec, int len) {
    int* _vec = new int[len];
    copy_vec(_vec, vec, len);
    allocation(_vec, len);
    delete[] _vec;
}

void allocation(int* vec, int len) {
    if (len <= 1) {
        return;
    }
    int lenUp = len / 2;
    _alocation(vec, lenUp);
    _alocation(vec + lenUp, len - lenUp);
    _merge(vec, vec + lenUp, lenUp, len - lenUp);
}

void create_allocation(int procCount) {
    int* vec = new int[procCount];
    for (int i = 0; i < procCount; i++) {
        vec[i] = i;
    }
    allocation(vec, procCount);
    delete[] vec;
}

void odd_even_sort(int* _vec, int _len) {
    int procCount, procRank, len = _len;
    int* vec = nullptr;
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procCount >= _len) {
        if (procRank == 0) {
            q_sort(_vec, 0, _len - 1);
        }
        return;
    }

    create_allocation(procCount);
    if (procRank == 0) {
        if (_len % procCount) {
            len = _len - (_len % procCount) + procCount;
        }
        vec = new int[len];
        copy_vec(vec, _vec, _len);
        for (int i = _len; i < len; i++) {
            vec[i] = -1;
        }
    }

    MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int lenProc = len / procCount;
    int* partVecResult = new int[lenProc];
    int* partVecCurrect = new int[lenProc];
    int* partVecTemp = new int[lenProc];

    MPI_Scatter(reinterpret_cast<void*>(vec), lenProc, MPI_INT,
        reinterpret_cast<void*>(partVecResult), lenProc, MPI_INT, 0, MPI_COMM_WORLD);
    q_sort(partVecResult, 0, lenProc - 1);

    int posResult, posCurrect;
    for (unsigned int i = 0; i < comparator.size(); i++) {
        if (procRank == comparator[i].second) {
            MPI_Recv(reinterpret_cast<void*>(partVecCurrect), lenProc, MPI_INT,
                comparator[i].first, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            MPI_Send(reinterpret_cast<void*>(partVecResult), lenProc, MPI_INT,
                comparator[i].first, 0, MPI_COMM_WORLD);
            posResult = posCurrect = lenProc - 1;
            for (int t = lenProc - 1; t >= 0; t--) {
                int result = partVecResult[posResult];
                int current = partVecCurrect[posCurrect];
                if (result > current) {
                    partVecTemp[t] = result;
                    posResult--;
                } else {
                    partVecTemp[t] = current;
                    posCurrect--;
                }
            }
            copy_vec(partVecResult, partVecTemp, lenProc);
        } else if (procRank == comparator[i].first) {
            MPI_Send(reinterpret_cast<void*>(partVecResult), lenProc, MPI_INT,
                comparator[i].second, 0, MPI_COMM_WORLD);
            MPI_Recv(reinterpret_cast<void*>(partVecCurrect), lenProc, MPI_INT,
                comparator[i].second, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            posResult = posCurrect = 0;
            for (int t = 0; t < lenProc; t++) {
                int result = partVecResult[posResult];
                int current = partVecCurrect[posCurrect];
                if (result < current) {
                    partVecTemp[t] = result;
                    posResult++;
                } else {
                    partVecTemp[t] = current;
                    posCurrect++;
                }
            }
            copy_vec(partVecResult, partVecTemp, lenProc);
        }
    }

    MPI_Gather(reinterpret_cast<void*>(partVecResult), lenProc, MPI_INT,
        reinterpret_cast<void*>(vec), lenProc, MPI_INT, 0, MPI_COMM_WORLD);
    if (procRank == 0) {
        int t = 0;
        for (int i = 0; i < _len; i++) {
            while (vec[t] < 0) {
                t++;
            }
            _vec[i] = vec[t];
            t++;
        }
    }

    delete[] partVecResult;
    delete[] partVecCurrect;
    delete[] partVecTemp;
    delete[] vec;
}
