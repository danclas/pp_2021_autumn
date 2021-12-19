// Copyright 2021 Vanyushkov Maxim
#include "../../../modules/task_2/vanyushkov_m_producer_consumer/producer_consumer.h"
#include <chrono>  // NOLINT [build/c++11]
#include <thread>  // NOLINT [build/c++11]
#include <random>

Consumer::Consumer() {
    MPI_Scatter(nullptr, 0, MPI_INT, &resourcesCount, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

void Consumer::Run() {
    // std::random_device dev;
    // std::mt19937 gen(dev());
    int resource;
    for (int i = 0; i < resourcesCount; i++) {
        MPI_Ssend(nullptr, 0, MPI_INT, 0, CONSUMER, MPI_COMM_WORLD);
        MPI_Recv(&resource, 1, MPI_INT, 0, MANAGER, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Get(data)
        // std::this_thread::sleep_for(std::chrono::milliseconds(gen() % 20)); // UseData()
    }
}

Producer::Producer() {
    MPI_Scatter(nullptr, 0, MPI_INT, &resourcesCount, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

void Producer::Run() {
    std::random_device dev;
    std::mt19937 gen(dev());
    int resource;
    for (int i = 0; i < resourcesCount; i++) {
        resource = gen() % 1000;  // PrepareData()
        // std::this_thread::sleep_for(std::chrono::milliseconds(gen() % 20));  // PrepareData()
        MPI_Ssend(&resource, 1, MPI_INT, 0, PRODUCER, MPI_COMM_WORLD);  // Put(data)
    }
}

Manager::Manager(int resNumber, int bufSize) {
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    resourceNumber = resNumber;
    bufferSize = bufSize;
    cyclicBuffer.resize(bufferSize);
    lBorder = rBorder = 0;
    full = false; empty = true;
    producerCount = (procCount - 1) / 2;
    consumerCount = procCount - producerCount - 1;
    recvData.resize(producerCount);
}

void Manager::producer(int index, MPI_Request* requests, MPI_Request* cRequests) {
    if (full) {
        // wait free entry
        int cIndex;
        MPI_Waitany(consumerCount, cRequests, &cIndex, MPI_STATUS_IGNORE);
        int tmpIndex = cIndex + producerCount + 1;
        MPI_Ssend(&(cyclicBuffer[lBorder]), 1, MPI_INT, tmpIndex, MANAGER, MPI_COMM_WORLD);
        MPI_Irecv(nullptr, 0, MPI_INT, tmpIndex, CONSUMER, MPI_COMM_WORLD, &(cRequests[cIndex]));
        // write value to buffer
        lBorder = next(lBorder);
        cyclicBuffer[rBorder] = recvData[index];
        rBorder = next(rBorder);
    } else {
        empty = false;
        // write value to buffer
        cyclicBuffer[rBorder] = recvData[index];
        rBorder = next(rBorder);
        // check full
        if (lBorder == rBorder) {
            full = true;
        }
    }
    MPI_Irecv(&(recvData[index]), 1, MPI_INT, index + 1, PRODUCER, MPI_COMM_WORLD, &(requests[index]));
}

void Manager::consumer(int index, MPI_Request* requests, MPI_Request* pRequests) {
    if (empty) {
        // wait values
        int pIndex;
        MPI_Waitany(producerCount, pRequests, &pIndex, MPI_STATUS_IGNORE);
        MPI_Ssend(&(recvData[pIndex]), 1, MPI_INT, index + 1, MANAGER, MPI_COMM_WORLD);
        MPI_Irecv(&(recvData[pIndex]), 1, MPI_INT, pIndex + 1, PRODUCER, MPI_COMM_WORLD, &(pRequests[pIndex]));
    } else {
        full = false;
        MPI_Ssend(&(cyclicBuffer[lBorder]), 1, MPI_INT, index + 1, MANAGER, MPI_COMM_WORLD);
        lBorder = next(lBorder);
        // check empty
        if (lBorder == rBorder) {
            empty = true;
        }
    }
    MPI_Irecv(nullptr, 0, MPI_INT, index + 1, CONSUMER, MPI_COMM_WORLD, &(requests[index]));
}

int Manager::next(int border) {
    return (border + 1) % bufferSize;
}

void Manager::Run() {
    MPI_Request* requests = new MPI_Request[procCount - 1];
    MPI_Request* pRequests = requests;  // { requests[0], requests[producerCount-1] }
    MPI_Request* cRequests = requests + producerCount;  // { requests[producerCount], requests[procCount-1] }
    for (int i = 0; i < procCount - 1; i++) {
        requests[i] = MPI_REQUEST_NULL;
    }

    int resourcesCount;
    std::vector<int> resources = { 0 };
    for (int i = 0; i < producerCount; i++) {
        resources.push_back(resourceNumber / producerCount +
            ((resourceNumber % producerCount - i > 0) ? 1 : 0));
    }
    for (int i = 0; i < consumerCount; i++) {
        resources.push_back(resourceNumber / consumerCount +
            ((resourceNumber % consumerCount - i > 0) ? 1 : 0));
    }
    MPI_Scatter(&(resources[0]), 1, MPI_INT, &resourcesCount, 1, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < producerCount; i++) {
        MPI_Irecv(&(recvData[i]), 1, MPI_INT, i + 1, PRODUCER, MPI_COMM_WORLD, &(pRequests[i]));
    }
    for (int i = 0; i < consumerCount; i++) {
        MPI_Irecv(nullptr, 0, MPI_INT, producerCount + i + 1, CONSUMER, MPI_COMM_WORLD, &(cRequests[i]));
    }

    for (int i = 0; i < resourceNumber * 2; i++) {
        int index;
        MPI_Waitany(procCount - 1, requests, &index, MPI_STATUS_IGNORE);
        if (index < producerCount) {
            if (full) {
                i++;
            }
            producer(index, requests, cRequests);
        } else {
            if (empty) {
                i++;
            }
            consumer(index, requests, pRequests);
        }
    }

    for (int i = 0; i < procCount - 1; i++) {
        MPI_Cancel(&(requests[i]));
        MPI_Request_free(&(requests[i]));
    }
    delete[] requests;
}
