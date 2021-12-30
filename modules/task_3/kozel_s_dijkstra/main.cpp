// Copyright 2021 Kozel Svetlana
#include <gtest/gtest.h>
#include "./dijkstra.h"
#include <gtest-mpi-listener.hpp>

bool test(int size) {
    bool flag = true;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    int* graf = gen_matrix(size, procRank);
    int* par = parallel(graf, size);

    if (procRank == 0) {
        int* seq = sequential(graf, size);
        flag = check_eq(par, seq, size * size);
        delete[] seq;
    }
    delete[] par;
    delete[] graf;

    return flag;
}

TEST(Dijkstra_algorithm, Graf_size_5) {
    ASSERT_TRUE(test(5));
}

TEST(Dijkstra_algorithm, Graf_size_7) {
    ASSERT_TRUE(test(7));
}

TEST(Dijkstra_algorithm, Graf_size_16) {
    ASSERT_TRUE(test(16));
}

TEST(Dijkstra_algorithm, Graf_size_23) {
    ASSERT_TRUE(test(23));
}

TEST(Dijkstra_algorithm, Graf_size_59) {
    ASSERT_TRUE(test(59));
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
