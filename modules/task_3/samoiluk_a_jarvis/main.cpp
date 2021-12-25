// Copyright 2021 Samoiluk Anastasiya
#include <gtest/gtest.h>
#include "../../task_3/samoiluk_a_jarvis/jarvis.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Vector_size_100) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    int vec_size = 100;

    if (rank == 0) {
        vec = getRandomVec(vec_size);
    }

    if (size > vec_size) {
        EXPECT_ANY_THROW(JarvisMarch_par(vec, vec_size));
    } else {
        std::vector<Point> par_jm = JarvisMarch_par(vec, vec_size);
        if (rank == 0) {
            std::vector<Point> seq_jm = JarvisMarch_seq(vec);

            ASSERT_EQ(par_jm.size(), seq_jm.size());
            for (std::vector<int>::size_type i = 0; i < par_jm.size(); i++) {
                ASSERT_EQ(par_jm[i].x, seq_jm[i].x);
                ASSERT_EQ(par_jm[i].y, seq_jm[i].y);
            }
        }
    }
}

TEST(Parallel_Operations_MPI, Vector_size_200) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    int vec_size = 200;

    if (rank == 0) {
        vec = getRandomVec(vec_size);
    }

    if (size > vec_size) {
        EXPECT_ANY_THROW(JarvisMarch_par(vec, vec_size));
    } else {
        std::vector<Point> par_jm = JarvisMarch_par(vec, vec_size);
        if (rank == 0) {
            std::vector<Point> seq_jm = JarvisMarch_seq(vec);

            ASSERT_EQ(par_jm.size(), seq_jm.size());
            for (std::vector<int>::size_type i = 0; i < par_jm.size(); i++) {
                ASSERT_EQ(par_jm[i].x, seq_jm[i].x);
                ASSERT_EQ(par_jm[i].y, seq_jm[i].y);
            }
        }
    }
}

TEST(Parallel_Operations_MPI, Vector_size_300) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    int vec_size = 300;

    if (rank == 0) {
        vec = getRandomVec(vec_size);
    }

    if (size > vec_size) {
        EXPECT_ANY_THROW(JarvisMarch_par(vec, vec_size));
    } else {
        std::vector<Point> par_jm = JarvisMarch_par(vec, vec_size);
        if (rank == 0) {
            std::vector<Point> seq_jm = JarvisMarch_seq(vec);

            ASSERT_EQ(par_jm.size(), seq_jm.size());
            for (std::vector<int>::size_type i = 0; i < par_jm.size(); i++) {
                ASSERT_EQ(par_jm[i].x, seq_jm[i].x);
                ASSERT_EQ(par_jm[i].y, seq_jm[i].y);
            }
        }
    }
}

TEST(Parallel_Operations_MPI, Vector_size_500) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    int vec_size = 500;

    if (rank == 0) {
        vec = getRandomVec(vec_size);
    }

    if (size > vec_size) {
        EXPECT_ANY_THROW(JarvisMarch_par(vec, vec_size));
    } else {
        std::vector<Point> par_jm = JarvisMarch_par(vec, vec_size);
        if (rank == 0) {
            std::vector<Point> seq_jm = JarvisMarch_seq(vec);

            ASSERT_EQ(par_jm.size(), seq_jm.size());
            for (std::vector<int>::size_type i = 0; i < par_jm.size(); i++) {
                ASSERT_EQ(par_jm[i].x, seq_jm[i].x);
                ASSERT_EQ(par_jm[i].y, seq_jm[i].y);
            }
        }
    }
}

TEST(Parallel_Operations_MPI, Vector_size_700) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    int vec_size = 700;

    if (rank == 0) {
        vec = getRandomVec(vec_size);
    }

    if (size > vec_size) {
        EXPECT_ANY_THROW(JarvisMarch_par(vec, vec_size));
    } else {
        std::vector<Point> par_jm = JarvisMarch_par(vec, vec_size);
        if (rank == 0) {
            std::vector<Point> seq_jm = JarvisMarch_seq(vec);

            ASSERT_EQ(par_jm.size(), seq_jm.size());
            for (std::vector<int>::size_type i = 0; i < par_jm.size(); i++) {
                ASSERT_EQ(par_jm[i].x, seq_jm[i].x);
                ASSERT_EQ(par_jm[i].y, seq_jm[i].y);
            }
        }
    }
}

TEST(Parallel_Operations_MPI, Vector_size_1000) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    int vec_size = 1000;

    if (rank == 0) {
        vec = getRandomVec(vec_size);
    }

    if (size > vec_size) {
        EXPECT_ANY_THROW(JarvisMarch_par(vec, vec_size));
    } else {
        std::vector<Point> par_jm = JarvisMarch_par(vec, vec_size);
        if (rank == 0) {
            std::vector<Point> seq_jm = JarvisMarch_seq(vec);

            ASSERT_EQ(par_jm.size(), seq_jm.size());
            for (std::vector<int>::size_type i = 0; i < par_jm.size(); i++) {
                ASSERT_EQ(par_jm[i].x, seq_jm[i].x);
                ASSERT_EQ(par_jm[i].y, seq_jm[i].y);
            }
        }
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
