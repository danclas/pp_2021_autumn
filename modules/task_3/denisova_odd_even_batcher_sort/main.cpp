// Copyright 2021 Denisova Julia

#include "../../../modules/task_3/denisova_odd_even_batcher_sort/odd_even_batcher_sort.h"

TEST(func_batcher_sort, check_with_positive_values) {
    int n = 5;
    int procRank = 0;
    bool flag = 0;
    std::vector<int> v1(n);
    std::vector<int> v2(n);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    v1[0] = 9;
    v1[1] = 8;
    v1[2] = 7;
    v1[3] = 1;
    v1[4] = 5;
    v2[0] = 1;
    v2[1] = 5;
    v2[2] = 7;
    v2[3] = 8;
    v2[4] = 9;
    q_sort_batcher(&v1);
    if (procRank == 0) {
        if (v1 != v2) {
            flag = 1;
        }
        ASSERT_EQ(flag, 0);
    }
}

TEST(func_batcher_sort, check_with_negative_values) {
    int n = 5;
    int procRank = 0;
    bool flag = 0;
    std::vector<int> v1(n);
    std::vector<int> v2(n);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    v1[0] = -7;
    v1[1] = -8;
    v1[2] = -9;
    v1[3] = -1;
    v1[4] = -5;
    v2[0] = -9;
    v2[1] = -8;
    v2[2] = -7;
    v2[3] = -5;
    v2[4] = -1;
    q_sort_batcher(&v1);
    if (procRank == 0) {
        if (v1 != v2) {
            flag = 1;
        }
        ASSERT_EQ(flag, 0);
    }
}

TEST(func_batcher_sort, check_with_positive_and_negative_values) {
    int n = 5;
    int procRank = 0;
    bool flag = 0;
    std::vector<int> v1(n);
    std::vector<int> v2(n);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    v1[0] = -7;
    v1[1] = 8;
    v1[2] = 0;
    v1[3] = -1;
    v1[4] = 5;
    v2[0] = -7;
    v2[1] = -1;
    v2[2] = 0;
    v2[3] = 5;
    v2[4] = 8;
    q_sort_batcher(&v1);
    if (procRank == 0) {
        if (v1 != v2) {
            flag = 1;
        }
        ASSERT_EQ(flag, 0);
    }
}

TEST(func_batcher_sort, compare_batcher_and_qsort) {
    int n = 235;
    int procRank = 0;
    bool flag = 0;
    std::vector<int> v1(n);
    std::vector<int> v2(n);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    v1 = random_vector(n);
    v2 = v1;
    q_sort_batcher(&v1);
    if (procRank == 0) {
        q_sort(&v2, 0, v2.size() - 1);
    }
    if (procRank == 0) {
        if (v1 != v2) {
            flag = 1;
        }
        ASSERT_EQ(flag, 0);
    }
}

TEST(func_batcher_sort, compare_batcher_and_standart_sort) {
    int n = 235;
    int procRank = 0;
    bool flag = 0;
    std::vector<int> v1(n);
    std::vector<int> v2(n);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    v1 = random_vector(n);
    v2 = v1;
    q_sort_batcher(&v1);
    if (procRank == 0) {
        sort(v2.begin(), v2.end());
    }
    if (procRank == 0) {
        if (v1 != v2) {
            flag = 1;
        }
        ASSERT_EQ(flag, 0);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);  // check for MPI_SUCCESS?

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
