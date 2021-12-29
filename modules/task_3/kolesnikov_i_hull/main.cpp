// Copyright 2021 Kolesnikov Ilya
#include <gtest/gtest.h>
#include <mpi.h>
#include <string>
#include "./kolesnikov_hull.h"
#include <gtest-mpi-listener.hpp>

TEST(hull, last_hull_is_equal_with_first_point) {
    int rank = 0, size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    vec.push_back(Point(1, 2));
    vec.push_back(Point(2, 4));
    vec.push_back(Point(2, 6));
    vec.push_back(Point(4, 5));
    vec.push_back(Point(7, 8));
    vec.push_back(Point(8, 2));
    vec.push_back(Point(10, 12));

    std::vector<Point> res = convexHull_jarvis_parallel(vec);

    if (rank == 0) {
        ASSERT_EQ(res[res.size() - 1].returnX(), vec[0].returnX());
        ASSERT_EQ(res[res.size() - 1].returnY(), vec[0].returnY());
    }
}

TEST(hull, single_parallel_2_nd_item) {
    int rank = 0, size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    vec.push_back(Point(1, 2));
    vec.push_back(Point(2, 4));
    vec.push_back(Point(2, 6));
    vec.push_back(Point(4, 5));
    vec.push_back(Point(7, 8));
    vec.push_back(Point(8, 2));
    vec.push_back(Point(10, 12));

    std::vector<Point> res = convexHull_jarvis_parallel(vec);
    std::vector<Point> res_single = convexHull_jarvis_parallel(vec);

    if (rank == 0 && res.size() > 1) {
        ASSERT_EQ(res[1].returnX(), res_single[1].returnX());
        ASSERT_EQ(res[1].returnY(), res_single[1].returnY());
    }
}

TEST(hull, single_parallel_3_d_item) {
    int rank = 0, size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    vec.push_back(Point(1, 2));
    vec.push_back(Point(2, 4));
    vec.push_back(Point(2, 6));
    vec.push_back(Point(4, 5));
    vec.push_back(Point(7, 8));
    vec.push_back(Point(8, 2));
    vec.push_back(Point(10, 12));

    std::vector<Point> res = convexHull_jarvis_parallel(vec);
    std::vector<Point> res_single = convexHull_jarvis_parallel(vec);

    if (rank == 0 && res.size() > 2) {
        ASSERT_EQ(res[2].returnX(), res_single[2].returnX());
        ASSERT_EQ(res[2].returnY(), res_single[2].returnY());
    }
}

TEST(hull, last_is_equal_with_first) {
    int rank = 0, size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    vec.push_back(Point(1, 2));
    vec.push_back(Point(2, 4));
    vec.push_back(Point(2, 6));
    vec.push_back(Point(4, 5));
    vec.push_back(Point(7, 8));
    vec.push_back(Point(8, 2));
    vec.push_back(Point(10, 12));

    std::vector<Point> res = convexHull_jarvis_parallel(vec);

    if (rank == 0 && res.size() > 2) {
        ASSERT_EQ(res[0].returnX(), res[res.size() - 1].returnX());
        ASSERT_EQ(res[0].returnY(), res[res.size() - 1].returnY());
    }
}

TEST(hull, last_is_equal_with_first2) {
    int rank = 0, size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<Point> vec;
    vec.push_back(Point(1, 2));
    vec.push_back(Point(2, 4));
    vec.push_back(Point(2, 6));
    vec.push_back(Point(4, 5));
    vec.push_back(Point(7, 8));
    vec.push_back(Point(8, 2));
    vec.push_back(Point(10, 12));

    std::vector<Point> res = convexHull_jarvis_parallel(vec);

    if (rank == 0 && res.size() > 2) {
        ASSERT_EQ(res[0].returnY(), res[res.size() - 1].returnY());
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
