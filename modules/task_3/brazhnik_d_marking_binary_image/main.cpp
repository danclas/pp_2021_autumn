// Copyright 2021 Brazhnik Dmitry
#include "../../../modules/task_3/brazhnik_d_marking_binary_image/marking_binary_image.h"

TEST(Parallel_Operations_MPI, TEST_GENERATE_BINARY_IMAGE_SIZE_3x5_VALUE_ONLY_0_1) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    int sizeX = 3;
    int sizeY = 5;
    std::vector<int> data = generate_binary_image(sizeX, sizeY);

    for (int i = 0; i < sizeX * sizeY; i++) {
        ASSERT_TRUE(data[i] == 0 || data[i] == 1);
    }
}

TEST(Parallel_Operations_MPI, TEST_GENERATE_BINARY_IMAGE_NEGATIVE_SIZE_EXCEPTION) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    int sizeX = -3;
    int sizeY = -5;
    std::vector<int> data;

    ASSERT_ANY_THROW(data = generate_binary_image(sizeX, sizeY));
}

TEST(Parallel_Operations_MPI, TEST_BASIC_MARKING_IMAGE_SIZE_5x5_TRUE) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
    int sizeX = 5;
    int sizeY = 5;
    int size = sizeX * sizeY;
    std::vector<int> data(size);

    data = {
        0, 1, 0, 0, 1,
        0, 1, 1, 1, 1,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 1,
        0, 1, 0, 0, 1
    };

    std::pair<std::vector<int>, int> resultSecond = basic_marking_binary_image(data, sizeX, sizeY);


    if (commRank == 0) {
        ASSERT_EQ(resultSecond.second, 3);
    }
}

TEST(Parallel_Operations_MPI, TEST_TIME_BASIC_MARKING_IMAGE_SIZE_10x10) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);
    int sizeX = 10;
    int sizeY = 10;
    int size = sizeX * sizeY;

    double timeBasicStart, timeBasicFinish, timeBasicResult;

    std::vector<int> data(size);
    if (commRank == 0) {
        data = {
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1
        };
    }
    if (commRank == 0) {
        timeBasicStart = MPI_Wtime();
    }
    std::pair<std::vector<int>, int> resultParallel = basic_marking_binary_image(data, sizeX, sizeY);

    if (commRank == 0) {
        timeBasicFinish = MPI_Wtime();
        timeBasicResult = timeBasicFinish - timeBasicStart;
    }

    if (commRank == 0) {
        printf("Time basic: %f\n", timeBasicResult);
        ASSERT_EQ(resultParallel.second, 1);
    }
}


TEST(Parallel_Operations_MPI, TEST_TIME_PARALLEL_10x10_COMP_1) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeParallelStart, timeParallelFinish;

    int sizeX = 10;
    int sizeY = 10;
    int size = sizeX * sizeY;

    std::vector<int> data(size);

    if (commRank == 0) {
        data = {
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 1
        };
    }

    if (commRank == 0) {
        timeParallelStart = MPI_Wtime();
    }
    std::pair<std::vector<int>, int> result = parallel_marking_binary_image(data, sizeX, sizeY);
    if (commRank == 0) {
        timeParallelFinish = MPI_Wtime();
    }

    if (commRank == 0) {
        printf("Time parallel: %f\n", timeParallelFinish - timeParallelStart);
        ASSERT_EQ(result.second, 1);
    }
}

TEST(Parallel_Operations_MPI, TEST_PARALLEL_LARGE_IMAGE_50x50_TIME) {
    int commRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeParallelStart, timeParallelFinish;

    int sizeX = 50;
    int sizeY = 50;
    int size = sizeX * sizeY;

    std::vector<int> data(size);

    if (commRank == 0) {
        data = generate_binary_image(sizeX, sizeY);
    }

    if (commRank == 0) {
        timeParallelStart = MPI_Wtime();
    }
    std::pair<std::vector<int>, int> result = parallel_marking_binary_image(data, sizeX, sizeY);
    if (commRank == 0) {
        timeParallelFinish = MPI_Wtime();
    }

    if (commRank == 0) {
        printf("Time parallel: %f\n", timeParallelFinish - timeParallelStart);
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
