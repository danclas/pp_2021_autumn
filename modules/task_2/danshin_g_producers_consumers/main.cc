// Copyright 2021 Gleb "belgad" Danshin
#include <gtest/gtest.h>
#include "../../../modules/task_2/danshin_g_producers_consumers/producers_consumers.h"
#include <gtest-mpi-listener.hpp>

TEST(DanshinGProducersConsumers, ProductNum10) {
    int product_number = 10;
    EXPECT_NO_THROW(producers_consumers(product_number));
}

TEST(DanshinGProducersConsumers, ProductNum25) {
    int product_number = 25;
    EXPECT_NO_THROW(producers_consumers(product_number));
}

TEST(DanshinGProducersConsumers, ProductNum50) {
    int product_number = 50;
    EXPECT_NO_THROW(producers_consumers(product_number));
}

TEST(DanshinGProducersConsumers, ProductNum100) {
    int product_number = 100;
    EXPECT_NO_THROW(producers_consumers(product_number));
}

TEST(DanshinGProducersConsumers, ProductNum500) {
    int product_number = 500;
    EXPECT_NO_THROW(producers_consumers(product_number));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
