// Copyright 2021 Mironova Ekaterina
#include <gtest/gtest.h>
#include "./jarvis_pass.h"
#include <gtest-mpi-listener.hpp>


TEST(MPI_TASK_3, work_correct_with_100_points) {
    std::vector<point> points;
    int size = 100;
    std::vector<point> result1, result2;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        points = fillPoints(size);
        result1 = jarvisPass(points);
    }
    result2 = parallelJarvisPass(points, size);
    if (procRank == 0) {
        for (std::vector<point>::size_type i = 0; i < result1.size(); i++) {
            ASSERT_EQ(result1[i].x, result2[i].x);
            ASSERT_EQ(result1[i].y, result2[i].y);
        }
    }
}

TEST(MPI_TASK_3, work_correct_with_101_points) {
    std::vector<point> points;
    int size = 101;
    std::vector<point> result1, result2;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        points = fillPoints(size);
        result1 = jarvisPass(points);
    }
    result2 = parallelJarvisPass(points, size);
    if (procRank == 0) {
        for (std::vector<point>::size_type i = 0; i < result1.size(); i++) {
            ASSERT_EQ(result1[i].x, result2[i].x);
            ASSERT_EQ(result1[i].y, result2[i].y);
        }
    }
}

TEST(MPI_TASK_3, work_correct_with_105_points) {
    std::vector<point> points;
    int size = 105;
    std::vector<point> result1, result2;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        points = fillPoints(size);
        result1 = jarvisPass(points);
    }
    result2 = parallelJarvisPass(points, size);
    if (procRank == 0) {
        for (std::vector<point>::size_type i = 0; i < result1.size(); i++) {
            ASSERT_EQ(result1[i].x, result2[i].x);
            ASSERT_EQ(result1[i].y, result2[i].y);
        }
    }
}

TEST(MPI_TASK_3, work_correct_with_109_points) {
    std::vector<point> points;
    int size = 109;
    std::vector<point> result1, result2;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        points = fillPoints(size);
        result1 = jarvisPass(points);
    }
    result2 = parallelJarvisPass(points, size);
    if (procRank == 0) {
        for (std::vector<point>::size_type i = 0; i < result1.size(); i++) {
            ASSERT_EQ(result1[i].x, result2[i].x);
            ASSERT_EQ(result1[i].y, result2[i].y);
        }
    }
}

TEST(MPI_TASK_3, work_correct_with_150_points) {
    std::vector<point> points;
    int size = 150;
    std::vector<point> result1, result2;
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        points = fillPoints(size);
        result1 = jarvisPass(points);
    }
    result2 = parallelJarvisPass(points, size);
    if (procRank == 0) {
        for (std::vector<point>::size_type i = 0; i < result1.size(); i++) {
            ASSERT_EQ(result1[i].x, result2[i].x);
            ASSERT_EQ(result1[i].y, result2[i].y);
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    std::srand(std::time(nullptr));

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
