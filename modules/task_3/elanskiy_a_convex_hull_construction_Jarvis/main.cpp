// Copyright 2021 Elanskiy Akexandr
#include "./convex_hull_construction_Jarvis.h"

TEST(Parallel_Operations_MPI, Test_Size_31) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::pair<int, int>> dots;
    int size = 31;
    dots = gen_dots(size);
    std::vector<std::pair<int, int>> par_res;
    std::vector<std::pair<int, int>> seq_res;
    par_res = jarvis_par(dots, size);
    if (rank == 0) {
        seq_res = jarvis_seq(dots);
        ASSERT_EQ(par_res, seq_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_32) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::pair<int, int>> dots;
    int size = 31;
    dots = gen_dots(size);
    std::vector<std::pair<int, int>> par_res;
    std::vector<std::pair<int, int>> seq_res;
    par_res = jarvis_par(dots, size);
    if (rank == 0) {
        seq_res = jarvis_seq(dots);
        ASSERT_EQ(par_res, seq_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::pair<int, int>> dots;
    int size = 100;
    dots = gen_dots(size);
    std::vector<std::pair<int, int>> par_res;
    std::vector<std::pair<int, int>> seq_res;
    par_res = jarvis_par(dots, size);
    if (rank == 0) {
        seq_res = jarvis_seq(dots);
        ASSERT_EQ(par_res, seq_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_123) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::pair<int, int>> dots;
    int size = 123;
    dots = gen_dots(size);
    std::vector<std::pair<int, int>> par_res;
    std::vector<std::pair<int, int>> seq_res;
    par_res = jarvis_par(dots, size);
    if (rank == 0) {
        seq_res = jarvis_seq(dots);
        ASSERT_EQ(par_res, seq_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_222) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::pair<int, int>> dots;
    int size = 222;
    dots = gen_dots(size);
    std::vector<std::pair<int, int>> par_res;
    std::vector<std::pair<int, int>> seq_res;
    par_res = jarvis_par(dots, size);
    if (rank == 0) {
        seq_res = jarvis_seq(dots);
        ASSERT_EQ(par_res, seq_res);
    }
}

TEST(Parallel_Operations_MPI, Test_Size_256) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::pair<int, int>> dots;
    int size = 256;
    dots = gen_dots(size);
    std::vector<std::pair<int, int>> par_res;
    std::vector<std::pair<int, int>> seq_res;
    par_res = jarvis_par(dots, size);
    if (rank == 0) {
        seq_res = jarvis_seq(dots);
        ASSERT_EQ(par_res, seq_res);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
