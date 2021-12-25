// Copyright 2021 Brazhnik Dmitry
#include <gtest/gtest.h>
#include "../../../modules/task_2/brazhnik_d_from_all_to_one/from_all_to_one.h"
#include <gtest-mpi-listener.hpp>
#define SIZE 1000

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_OP_MPI_SUM_INT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int data = getRandVal();
    int customResult = 0;
    int MPIResult = 0;

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(&data, &MPIResult, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(&data, &customResult, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        ASSERT_EQ(customResult, MPIResult);
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_OP_MPI_PROD_INT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int data = getRandVal();
    int customResult = 0;
    int MPIResult = 0;

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(&data, &MPIResult, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(&data, &customResult, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        ASSERT_EQ(customResult, MPIResult);
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_OP_MPI_MIN_INT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int data = getRandVal();
    int customResult = 0;
    int MPIResult = 0;

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(&data, &MPIResult, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(&data, &customResult, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        ASSERT_EQ(customResult, MPIResult);
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_OP_MPI_MAX_INT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int data = getRandVal();
    int customResult = 0;
    int MPIResult = 0;

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(&data, &MPIResult, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(&data, &customResult, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        ASSERT_EQ(customResult, MPIResult);
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_FOR_VECTOR_OP_MPI_SUM_SHOW_TIME_WORKED_INT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int sizeVector = SIZE;
    std::vector<int> data = fillRandomValToVectorInt(sizeVector);
    std::vector<int> customResult(sizeVector);
    std::vector<int> MPIResult(sizeVector);

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(static_cast<void*>(data.data()), static_cast<void*>(MPIResult.data()), sizeVector,
        MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }
    timeCustomStart = MPI_Wtime();
    customReduce(static_cast<void*>(data.data()), static_cast<void*>(customResult.data()), sizeVector,
        MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        ASSERT_EQ(customResult, MPIResult);
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_FOR_VECTOR_OP_MPI_PROD_SHOW_TIME_WORKED_INT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int sizeVector = SIZE;
    std::vector<int> data = fillRandomValToVectorInt(sizeVector);
    std::vector<int> customResult(sizeVector);
    std::vector<int> MPIResult(sizeVector);

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(static_cast<void*>(data.data()), static_cast<void*>(MPIResult.data()), sizeVector,
        MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(static_cast<void*>(data.data()), static_cast<void*>(customResult.data()), sizeVector,
        MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        ASSERT_EQ(customResult, MPIResult);
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_FOR_VECTOR_OP_MPI_MIN_SHOW_TIME_WORKED_INT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int sizeVector = SIZE;
    std::vector<int> data = fillRandomValToVectorInt(sizeVector);
    std::vector<int> customResult(sizeVector);
    std::vector<int> MPIResult(sizeVector);

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(static_cast<void*>(data.data()), static_cast<void*>(MPIResult.data()), sizeVector,
        MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(static_cast<void*>(data.data()), static_cast<void*>(customResult.data()), sizeVector,
        MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        ASSERT_EQ(customResult, MPIResult);
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_FOR_VECTOR_OP_MPI_MAX_SHOW_TIME_WORKED_INT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int sizeVector = SIZE;
    std::vector<int> data = fillRandomValToVectorInt(sizeVector);
    std::vector<int> customResult(sizeVector);
    std::vector<int> MPIResult(sizeVector);

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(static_cast<void*>(data.data()), static_cast<void*>(MPIResult.data()), sizeVector,
        MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(static_cast<void*>(data.data()), static_cast<void*>(customResult.data()), sizeVector,
        MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        ASSERT_EQ(customResult, MPIResult);
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_FOR_VECTOR_OP_MPI_SUM_SHOW_TIME_WORKED_DOUBLE) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int sizeVector = SIZE;
    std::vector<double> data = fillRandomValToVectorDouble(sizeVector);
    std::vector<double> customResult(sizeVector);
    std::vector<double> MPIResult(sizeVector);

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(static_cast<void*>(data.data()), static_cast<void*>(MPIResult.data()), sizeVector,
        MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(static_cast<void*>(data.data()), static_cast<void*>(customResult.data()), sizeVector,
        MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        for (int i = 0; i < SIZE; i++) {
            ASSERT_TRUE(customResult[i] - MPIResult[i] < 0.01);
        }
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_FOR_VECTOR_OP_MPI_MAX_SHOW_TIME_WORKED_DOUBLE) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int sizeVector = SIZE;
    std::vector<double> data = fillRandomValToVectorDouble(sizeVector);
    std::vector<double> customResult(sizeVector);
    std::vector<double> MPIResult(sizeVector);

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(static_cast<void*>(data.data()), static_cast<void*>(MPIResult.data()), sizeVector,
        MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(static_cast<void*>(data.data()), static_cast<void*>(customResult.data()), sizeVector,
        MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        for (int i = 0; i < SIZE; i++) {
            ASSERT_TRUE(customResult[i] - MPIResult[i] < 0.01);
        }
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_FOR_VECTOR_OP_MPI_MIN_SHOW_TIME_WORKED_DOUBLE) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int sizeVector = SIZE;
    std::vector<double> data = fillRandomValToVectorDouble(sizeVector);
    std::vector<double> customResult(sizeVector);
    std::vector<double> MPIResult(sizeVector);

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(static_cast<void*>(data.data()), static_cast<void*>(MPIResult.data()), sizeVector,
        MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(static_cast<void*>(data.data()), static_cast<void*>(customResult.data()), sizeVector,
        MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        for (int i = 0; i < SIZE; i++) {
            ASSERT_TRUE(customResult[i] - MPIResult[i] < 0.01);
        }
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_FOR_VECTOR_OP_MPI_SUM_SHOW_TIME_WORKED_FLOAT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int sizeVector = SIZE;
    std::vector<float> data = fillRandomValToVectorFloat(sizeVector);
    std::vector<float> customResult(sizeVector);
    std::vector<float> MPIResult(sizeVector);

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(static_cast<void*>(data.data()), static_cast<void*>(MPIResult.data()), sizeVector,
        MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(static_cast<void*>(data.data()), static_cast<void*>(customResult.data()), sizeVector,
        MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        for (int i = 0; i < SIZE; i++) {
            ASSERT_TRUE(customResult[i] - MPIResult[i] < 0.01);
        }
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_FOR_VECTOR_OP_MPI_MIN_SHOW_TIME_WORKED_FLOAT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int sizeVector = SIZE;
    std::vector<float> data = fillRandomValToVectorFloat(sizeVector);
    std::vector<float> customResult(sizeVector);
    std::vector<float> MPIResult(sizeVector);

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(static_cast<void*>(data.data()), static_cast<void*>(MPIResult.data()), sizeVector,
        MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(static_cast<void*>(data.data()), static_cast<void*>(customResult.data()), sizeVector,
        MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        for (int i = 0; i < SIZE; i++) {
            ASSERT_TRUE(customResult[i] - MPIResult[i] < 0.01);
        }
    }
}

TEST(Parallel_Operations_MPI, TEST_SAME_RESULT_FOR_VECTOR_OP_MPI_MAX_SHOW_TIME_WORKED_FLOAT) {
    int countProc, commRank;
    MPI_Comm_size(MPI_COMM_WORLD, &countProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    double timeMPIStart, timeMPIFinish, timeMPIResult;
    double timeCustomStart, timeCustomFinish, timeCustomResult;

    int sizeVector = SIZE;
    std::vector<float> data = fillRandomValToVectorFloat(sizeVector);
    std::vector<float> customResult(sizeVector);
    std::vector<float> MPIResult(sizeVector);

    timeMPIStart = MPI_Wtime();
    MPI_Reduce(static_cast<void*>(data.data()), static_cast<void*>(MPIResult.data()), sizeVector,
        MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeMPIFinish = MPI_Wtime();
        timeMPIResult = timeMPIFinish - timeMPIStart;
    }

    timeCustomStart = MPI_Wtime();
    customReduce(static_cast<void*>(data.data()), static_cast<void*>(customResult.data()), sizeVector,
        MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (commRank == 0) {
        timeCustomFinish = MPI_Wtime();
        timeCustomResult = timeCustomFinish - timeCustomStart;
        printf("MPI:\t%lf \nCustom:\t%lf\n", timeMPIResult, timeCustomResult);
        for (int i = 0; i < SIZE; i++) {
            ASSERT_TRUE(customResult[i] - MPIResult[i] < 0.01);
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
