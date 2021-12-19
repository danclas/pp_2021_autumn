// Copyright 2021 Vanyushkov Maxim
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./producer_consumer.h"

void test(int resourceCount, int bufSize) {
    int procCount, procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);

    if (procCount < 3) {
        return;
    }

    if (procRank == 0) {
        Manager manager(resourceCount, bufSize);
        manager.Run();
    } else if (procRank <= (procCount - 1) / 2) {
        Producer producer;
        producer.Run();
    } else {
        Consumer consumer;
        consumer.Run();
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Producer_consumer, Resource_count_10_BufSize_10) {
    int resourceCount = 10;
    int bufSize = 10;
    ASSERT_NO_THROW(test(resourceCount, bufSize));
}

TEST(Producer_consumer, Resource_count_10_BufSize_5) {
    int resourceCount = 10;
    int bufSize = 5;
    ASSERT_NO_THROW(test(resourceCount, bufSize));
}

TEST(Producer_consumer, Resource_count_87_BufSize_15) {
    int resourceCount = 87;
    int bufSize = 15;
    ASSERT_NO_THROW(test(resourceCount, bufSize));
}

TEST(Producer_consumer, Resource_count_163_BufSize_20) {
    int resourceCount = 163;
    int bufSize = 20;
    ASSERT_NO_THROW(test(resourceCount, bufSize));
}

TEST(Producer_consumer, Resource_count_501_BufSize_27) {
    int resourceCount = 501;
    int bufSize = 27;
    ASSERT_NO_THROW(test(resourceCount, bufSize));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    std::srand(std::time(nullptr));

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
