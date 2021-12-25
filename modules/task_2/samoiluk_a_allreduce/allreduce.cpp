// Copyright 2021 Samoiluk Anastasiya
#include <mpi.h>
#include <random>
#include <queue>
#include <stack>
#include "../../modules/task_2/samoiluk_a_allreduce/allreduce.h"

struct Node {
    int val;
    Node* l;
    Node* r;
    explicit Node(int _val = 0, Node* _l = nullptr, Node* _r = nullptr,
        Node* _prev = nullptr) : val(_val), l(_l), r(_r) {}
};

std::queue<Node*> procTree(int root, int size) {
    Node* r = new Node(root);
    std::stack<Node*> sl, sr;
    std::queue<Node*> q;
    std::queue<int> qnum;
    for (int i = 0; i < size; i++) {
        if (i != root) {
            qnum.push(i);
        }
    }
    q.push(r);
    int val;
    if (!qnum.empty()) {
        val = qnum.front();
        qnum.pop();
        Node* left = new Node(val);
        sl.push(left);
        r->l = left;
    }
    if (!qnum.empty()) {
        val = qnum.front();
        qnum.pop();
        Node* right = new Node(val);
        sr.push(right);
        r->r = right;
    }
    while (!qnum.empty()) {
        Node* tmp;
        if (!sl.empty()) {
            tmp = sl.top();
            val = qnum.front();
            qnum.pop();
            Node* left = new Node(val);
            sl.push(left);
            tmp->l = left;
            if (!qnum.empty()) {
                val = qnum.front();
                qnum.pop();
                Node* right = new Node(val);
                sr.push(right);
                tmp->r = right;
            }
        }
        if (!sr.empty()) {
            tmp = sr.top();
            if (!qnum.empty()) {
                val = qnum.front();
                qnum.pop();
                Node* left = new Node(val);
                sl.push(left);
                tmp->l = left;
            }
            if (!qnum.empty()) {
                val = qnum.front();
                qnum.pop();
                Node* right = new Node(val);
                sr.push(right);
                tmp->r = right;
            }
        }
    }
    while (!sl.empty()) {
        q.push(sl.top());
        sl.pop();
    }
    while (!sr.empty()) {
        q.push(sr.top());
        sr.pop();
    }
    return q;
}

int Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype type,
        MPI_Op Op, MPI_Comm comm) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int root;
    if (rank == 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        root = gen() % size;
    }
    MPI_Bcast(&root, 1, MPI_INT, 0, comm);
    std::queue<Node*> q;
    q = procTree(root, size);

    if (rank == root) {
        if (type == MPI_INT) {
            for (int i = 0; i < count; i++) {
                (reinterpret_cast<int*>(recvbuf))[i] =
                    (reinterpret_cast<int*>(sendbuf))[i];
            }
            int* tmp_recvbuf = new int[count];
            for (int i = 0; i < size - 1; i++) {
                MPI_Recv(tmp_recvbuf, count, MPI_INT, MPI_ANY_SOURCE,
                    MPI_ANY_TAG, comm, MPI_STATUS_IGNORE);
                if (Op == MPI_SUM) {
                    for (int j = 0; j < count; j++) {
                        (reinterpret_cast<int*>(recvbuf))[j] += tmp_recvbuf[j];
                    }
                } else if (Op == MPI_PROD) {
                    for (int j = 0; j < count; j++) {
                        (reinterpret_cast<int*>(recvbuf))[j] *= tmp_recvbuf[j];
                    }
                } else if (Op == MPI_MAX) {
                    for (int j = 0; j < count; j++) {
                        if ((reinterpret_cast<int*>(recvbuf))[j] <
                            tmp_recvbuf[j]) {
                            (reinterpret_cast<int*>(recvbuf))[j] =
                                tmp_recvbuf[j];
                        }
                    }
                } else if (Op == MPI_MIN) {
                    for (int j = 0; j < count; j++) {
                        if ((reinterpret_cast<int*>(recvbuf))[j] >
                            tmp_recvbuf[j]) {
                            (reinterpret_cast<int*>(recvbuf))[j] =
                                tmp_recvbuf[j];
                        }
                    }
                }
            }
        delete[] tmp_recvbuf;
        } else if (type == MPI_DOUBLE) {
            for (int i = 0; i < count; i++) {
                (reinterpret_cast<double*>(recvbuf))[i] =
                    (reinterpret_cast<double*>(sendbuf))[i];
            }
            double* tmp_recvbuf = new double[count];
            for (int i = 0; i < size - 1; i++) {
                MPI_Recv(tmp_recvbuf, count, MPI_DOUBLE, MPI_ANY_SOURCE,
                    MPI_ANY_TAG, comm, MPI_STATUS_IGNORE);
                if (Op == MPI_SUM) {
                    for (int j = 0; j < count; j++) {
                        (reinterpret_cast<double*>(recvbuf))[j] +=
                            tmp_recvbuf[j];
                    }
                } else if (Op == MPI_PROD) {
                    for (int j = 0; j < count; j++) {
                        (reinterpret_cast<double*>(recvbuf))[j] *=
                            tmp_recvbuf[j];
                    }
                } else if (Op == MPI_MAX) {
                    for (int j = 0; j < count; j++) {
                        if ((reinterpret_cast<double*>(recvbuf))[j] <
                            tmp_recvbuf[j]) {
                            (reinterpret_cast<double*>(recvbuf))[j] =
                                tmp_recvbuf[j];
                        }
                    }
                } else if (Op == MPI_MIN) {
                    for (int j = 0; j < count; j++) {
                        if ((reinterpret_cast<double*>(recvbuf))[j] >
                            tmp_recvbuf[j]) {
                            (reinterpret_cast<double*>(recvbuf))[j] =
                                tmp_recvbuf[j];
                        }
                    }
                }
            }
        delete[] tmp_recvbuf;
        } else if (type == MPI_FLOAT) {
            for (int i = 0; i < count; i++) {
                (reinterpret_cast<float*>(recvbuf))[i] =
                    (reinterpret_cast<float*>(sendbuf))[i];
            }
            float* tmp_recvbuf = new float[count];
            for (int i = 0; i < size - 1; i++) {
                MPI_Recv(tmp_recvbuf, count, MPI_FLOAT, MPI_ANY_SOURCE,
                    MPI_ANY_TAG, comm, MPI_STATUS_IGNORE);
                if (Op == MPI_SUM) {
                    for (int j = 0; j < count; j++) {
                        (reinterpret_cast<float*>(recvbuf))[j] +=
                            tmp_recvbuf[j];
                    }
                } else if (Op == MPI_PROD) {
                    for (int j = 0; j < count; j++) {
                        (reinterpret_cast<float*>(recvbuf))[j] *=
                            tmp_recvbuf[j];
                    }
                } else if (Op == MPI_MAX) {
                    for (int j = 0; j < count; j++) {
                        if ((reinterpret_cast<float*>(recvbuf))[j] <
                            tmp_recvbuf[j]) {
                            (reinterpret_cast<float*>(recvbuf))[j] =
                                tmp_recvbuf[j];
                        }
                    }
                } else if (Op == MPI_MIN) {
                    for (int j = 0; j < count; j++) {
                        if ((reinterpret_cast<float*>(recvbuf))[j] >
                            tmp_recvbuf[j]) {
                            (reinterpret_cast<float*>(recvbuf))[j] =
                                tmp_recvbuf[j];
                        }
                    }
                }
            }
        delete[] tmp_recvbuf;
        }
    } else {
        MPI_Send(sendbuf, count, type, root, rank, comm);
    }
    Node* tmp = q.front();
    while (!q.empty()) {
        tmp = q.front();
        if (tmp->val == rank)
            break;
        q.pop();
    }
    if (tmp->l != nullptr)
        MPI_Send(recvbuf, count, type, tmp->l->val, rank, comm);
    if (tmp->r != nullptr)
        MPI_Send(recvbuf, count, type, tmp->r->val, rank, comm);
    if (rank != root)
        MPI_Recv(recvbuf, count, type, MPI_ANY_SOURCE, MPI_ANY_TAG,
            comm, MPI_STATUS_IGNORE);
    return 0;
}

template<class T>
T* getRandomVec(int size, int border) {
    T* vec = new T[size];
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < size; i++) {
        vec[i] = static_cast<T>(gen() % border);
    }
    return vec;
}

template
int* getRandomVec<int>(int size, int border);

template
double* getRandomVec<double>(int size, int border);

template
float* getRandomVec<float>(int size, int border);
