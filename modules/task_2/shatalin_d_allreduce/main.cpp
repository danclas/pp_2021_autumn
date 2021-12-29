// Copyright 2021 Shatalin Dmitriy
#include "./all_reduce.h"

TEST(MY_MPI_ALLREDUCE, mpi_sum_mpi_int) {
    std::vector<int> vec = vec_gen<int>(100);
    std::vector<int> my_mpi(vec.size());
    std::vector<int> _mpi(vec.size());

    MPI_allreduce_my(vec.data(), my_mpi.data(), vec.size(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(vec.data(), _mpi.data(), vec.size(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    ASSERT_TRUE(_mpi == my_mpi);
}

TEST(MY_MPI_ALLREDUCE, mpi_max_mpi_int) {
    std::vector<int> vec = vec_gen<int>(100);
    std::vector<int> my_mpi(vec.size());
    std::vector<int> _mpi(vec.size());

    MPI_allreduce_my(vec.data(), my_mpi.data(), vec.size(), MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(vec.data(), _mpi.data(), vec.size(), MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    ASSERT_TRUE(_mpi == my_mpi);
}

TEST(MY_MPI_ALLREDUCE, mpi_min_mpi_int) {
    std::vector<int> vec = vec_gen<int>(100);
    std::vector<int> my_mpi(vec.size());
    std::vector<int> _mpi(vec.size());

    MPI_allreduce_my(vec.data(), my_mpi.data(), vec.size(), MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(vec.data(), _mpi.data(), vec.size(), MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    ASSERT_TRUE(_mpi == my_mpi);
}

TEST(MY_MPI_ALLREDUCE, mpi_prod_mpi_int) {
    std::vector<int> vec = vec_gen<int>(100);
    std::vector<int> my_mpi(vec.size());
    std::vector<int> _mpi(vec.size());

    MPI_allreduce_my(vec.data(), my_mpi.data(), vec.size(), MPI_INT, MPI_PROD, MPI_COMM_WORLD);
    MPI_Allreduce(vec.data(), _mpi.data(), vec.size(), MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    ASSERT_TRUE(_mpi == my_mpi);
}

TEST(MY_MPI_ALLREDUCE, mpi_sum_mpi_float) {
    std::vector<float> vec = vec_gen<float>(100);
    std::vector<float> my_mpi(vec.size());
    std::vector<float> _mpi(vec.size());

    MPI_allreduce_my(vec.data(), my_mpi.data(), vec.size(), MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(vec.data(), _mpi.data(), vec.size(), MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    ASSERT_TRUE(_mpi == my_mpi);
}

TEST(MY_MPI_ALLREDUCE, mpi_max_mpi_double) {
    std::vector<double> vec = vec_gen<double>(1000);
    std::vector<double> my_mpi(vec.size());
    std::vector<double> _mpi(vec.size());

    MPI_allreduce_my(vec.data(), my_mpi.data(), vec.size(), MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(vec.data(), _mpi.data(), vec.size(), MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    ASSERT_TRUE(_mpi == my_mpi);
}

TEST(MY_MPI_ALLREDUCE, mpi_status_check) {
    std::vector<int> vec = vec_gen<int>(100);
    std::vector<int> my_mpi(vec.size());
    std::vector<int> _mpi(vec.size());
    int stat;
    stat = MPI_allreduce_my(vec.data(), my_mpi.data(), vec.size(), MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    int res = MPI_SUCCESS;
    ASSERT_EQ(stat, res);
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
