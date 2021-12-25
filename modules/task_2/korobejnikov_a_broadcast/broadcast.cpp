// Copyright 2021 Korobejnikov Alexander
#include <mpi.h>
#include <vector>
#include <algorithm>
#include <queue>
#include "../../../modules/task_2/korobejnikov_a_broadcast/broadcast.h"

struct node {
    node* parent;
    node* left;
    node* rigth;
    int val;
    explicit node(node* p = nullptr,
        node* l = nullptr, node* r = nullptr, int v = -1) :
        parent(p), left(l), rigth(r), val(v) {}
};

void addNode(node* root, int val) {
    node* curr;
    std::queue<node*> q;
    if (root != 0) {
        q.push(root);
    }
    while (!q.empty()) {
        curr = q.front();
        q.pop();
        if (curr->left != nullptr) {
            q.push(curr->left);
        } else {
            curr->left = new node(curr, nullptr, nullptr, val);
            return;
        }
        if (curr->rigth != nullptr) {
            q.push(curr->rigth);
        } else {
            curr->rigth = new node(curr, nullptr, nullptr, val);
            return;
        }
    }
}

node* search(node* root, int val) {
    node* curr;
    std::queue<node*> q;
    if (root != 0) q.push(root);
    while (!q.empty()) {
        curr = q.front();
        q.pop();
        if (curr->val == val) {
            return curr;
        }
        if (curr->left != nullptr) {
            q.push(curr->left);
        }
        if (curr->rigth != nullptr) {
            q.push(curr->rigth);
        }
    }
    return nullptr;
}

int My_Bcast(void *buffer, int count,
        MPI_Datatype datatype, int root, MPI_Comm comm) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    node* head = new node(nullptr, nullptr, nullptr, root);
    for (int i = 0; i < size; ++i) {
        if (i != root) {
            addNode(head, i);
        }
    }

    if (rank == root) {
        if (head->left != nullptr) {
            MPI_Send(buffer, count,
                datatype, head->left->val, 0, MPI_COMM_WORLD);
        }
        if (head->rigth != nullptr) {
            MPI_Send(buffer, count,
                datatype, head->rigth->val, 0, MPI_COMM_WORLD);
        }
    } else {
        node* tmp = search(head, rank);
        MPI_Status status;
        MPI_Recv(buffer, count, datatype,
           tmp->parent->val, 0, MPI_COMM_WORLD, &status);
        if (tmp->left != nullptr) {
            MPI_Send(buffer, count,
                datatype, tmp->left->val, 0, MPI_COMM_WORLD);
        }
        if (tmp->rigth != nullptr) {
            MPI_Send(buffer, count,
                datatype, tmp->rigth->val, 0, MPI_COMM_WORLD);
        }
    }
    return 0;
}
