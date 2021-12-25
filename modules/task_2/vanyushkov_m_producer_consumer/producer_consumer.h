// Copyright 2021 Vanyushkov Maxim
#ifndef MODULES_TASK_2_VANYUSHKOV_M_PRODUCER_CONSUMER_PRODUCER_CONSUMER_H_
#define MODULES_TASK_2_VANYUSHKOV_M_PRODUCER_CONSUMER_PRODUCER_CONSUMER_H_

#include <mpi.h>
#include <vector>

#define PRODUCER 1
#define CONSUMER 2
#define MANAGER 3

class Consumer {
 private:
    int resourcesCount;

 public:
    Consumer();
    void Run();
};

class Producer {
 private:
    int resourcesCount;

 public:
    Producer();
    void Run();
};

class Manager {
 private:
    int procCount;
    int resourceNumber;
    int bufferSize;
    std::vector<int> cyclicBuffer;
    int lBorder, rBorder;
    bool full, empty;
    int producerCount;
    int consumerCount;
    std::vector<int> recvData;

 private:
    void producer(int index, MPI_Request* requests, MPI_Request* cRequests);
    void consumer(int index, MPI_Request* requests, MPI_Request* pRequests);
    int next(int border);

 public:
    Manager(int resNumber, int bufSize);
    void Run();
};

#endif  // MODULES_TASK_2_VANYUSHKOV_M_PRODUCER_CONSUMER_PRODUCER_CONSUMER_H_
