// Copyright 2021 Samoiluk Anastasiya
#include <gtest/gtest.h>
#include "../../modules/task_2/samoiluk_a_allreduce/allreduce.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Vector_int_sum) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 10, border = 100;
    int* vec = getRandomVec<int>(size, border);
    int* res_vec = new int[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    int* mpi_vec = new int[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_int_prod) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 10, border = 100;
    int* vec = getRandomVec<int>(size, border);
    int* res_vec = new int[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    int* mpi_vec = new int[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_INT, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_int_max) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 10, border = 100;
    int* vec = getRandomVec<int>(size, border);
    int* res_vec = new int[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    int* mpi_vec = new int[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_int_min) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 10, border = 100;
    int* vec = getRandomVec<int>(size, border);
    int* res_vec = new int[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    int* mpi_vec = new int[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_double_sum) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 15, border = 1000;
    double* vec = getRandomVec<double>(size, border);
    double* res_vec = new double[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    double* mpi_vec = new double[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_double_prod) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 15, border = 1000;
    double* vec = getRandomVec<double>(size, border);
    double* res_vec = new double[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    double* mpi_vec = new double[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_DOUBLE_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_double_max) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 15, border = 1000;
    double* vec = getRandomVec<double>(size, border);
    double* res_vec = new double[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    double* mpi_vec = new double[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_double_min) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 15, border = 1000;
    double* vec = getRandomVec<double>(size, border);
    double* res_vec = new double[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    double* mpi_vec = new double[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_float_sum) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 20, border = 10;
    float* vec = getRandomVec<float>(size, border);
    float* res_vec = new float[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    float* mpi_vec = new float[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_FLOAT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_float_prod) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 20, border = 10;
    float* vec = getRandomVec<float>(size, border);
    float* res_vec = new float[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_FLOAT, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    float* mpi_vec = new float[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_FLOAT, MPI_PROD, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_FLOAT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_float_max) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 20, border = 10;
    float* vec = getRandomVec<float>(size, border);
    float* res_vec = new float[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    float* mpi_vec = new float[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
}

TEST(Parallel_Operations_MPI, Vector_float_min) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time1, start_time2, time1, time2;

    int size = 20, border = 10;
    float* vec = getRandomVec<float>(size, border);
    float* res_vec = new float[size];

    if (rank == 0) {
        start_time1 = MPI_Wtime();
    }

    Allreduce(vec, res_vec, size, MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        time1 = MPI_Wtime() - start_time1;
        start_time2 = MPI_Wtime();
    }

    float* mpi_vec = new float[size];
    MPI_Allreduce(vec, mpi_vec, size, MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD);

    if (rank == 0) {
        time2 = MPI_Wtime() - start_time2;
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(res_vec[i], mpi_vec[i]);
        }
        ASSERT_LT(abs(time1 - time2), 1);
    }
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
