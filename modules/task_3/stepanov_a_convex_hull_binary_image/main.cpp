// Copyright 2021 Stepanov Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./convex_hull_binary_image.h"
#include <gtest-mpi-listener.hpp>


TEST(GENERATE_IMAGE, can_generate_random_binary_image) {
    std::vector<int> image(10 * 10);
    ASSERT_NO_THROW(generateBinaryImage(&image, 10));
}

TEST(MARKING_COMPONENTS, can_marking_components_image) {
    std::vector<int> image(10 * 10);
    generateBinaryImage(&image, 10);
    ASSERT_NO_THROW(markingComponents(&image, 10));
}

TEST(MARKING_COMPONENTS, marking_components_correct_marking) {
    std::vector<int> image = { 1, 0, 1, 1,
                               0, 0, 1, 1,
                               1, 0, 0, 0,
                               1, 1, 1, 1 };
    std::vector<int> expected_marking_image = { 2, 0, 3, 3,
                                               0, 0, 3, 3,
                                               4, 0, 0, 0,
                                               4, 4, 4, 4 };
    markingComponents(&image, 4);
    EXPECT_EQ(image, expected_marking_image);
}

TEST(MARKING_COMPONENTS, marking_components_correct_insert_pixel_in_components) {
    std::vector<int> image = { 1, 0, 1, 1,
                               0, 0, 1, 1,
                               1, 0, 0, 0,
                               1, 1, 1, 1 };
    std::vector<std::vector<int>> expected_components(3, std::vector<int>{});
    expected_components[0] = { 0 };
    expected_components[1] = { 2, 6, 7, 3 };
    expected_components[2] = { 8, 12, 13, 14, 15 };

    auto components = markingComponents(&image, 4);

    for (std::size_t i = 0; i < 3; i++) {
        EXPECT_EQ(expected_components[i], components[i]);
    }
}

TEST(CREATE_HULL_SEQUENTIAL, can_create_hull) {
    std::vector<int> image(10 * 10);
    generateBinaryImage(&image, 10);
    ASSERT_NO_THROW(createHullImageSequential(image, 10));
}

TEST(CREATE_HULL_SEQUENTIAL, hull_created_correctly) {
    std::vector<int> image = { 1, 0, 1, 1,
                               0, 0, 1, 1,
                               1, 0, 0, 0,
                               1, 1, 1, 1 };
    std::vector<std::vector<int>> expected_hull{ {0, 0, 0, 0},
                                                 {0, 3, 1, 2},
                                                 {2, 3, 3, 0} };
    std::vector<std::vector<int>> hull = createHullImageSequential(image, 4);


    for (std::size_t i = 0; i < 3; i++) {
        EXPECT_EQ(expected_hull[i], hull[i]);
    }
}

TEST(CREATE_HULL_PARALLEL, can_create_hull) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::size_t size_image = 15;
    std::vector<int> image(size_image * size_image);

    if (rank == 0) {
        generateBinaryImage(&image, size_image);
    }

    ASSERT_NO_THROW(createHullImageParallel(image, size_image));
}

TEST(CREATE_HULL_PARALLEL, hull_created_correctly_with_image_even_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::size_t size_image = 8;
    std::vector<int> image(size_image * size_image);

    if (rank == 0) {
        generateBinaryImage(&image, size_image);
    }

    auto parallel_res = createHullImageParallel(image, size_image);

    if (rank == 0) {
        auto sequential_res = createHullImageSequential(image, size_image);
        EXPECT_EQ(parallel_res, sequential_res);
    }
}

TEST(CREATE_HULL_PARALLEL, hull_created_correctly_with_image_odd_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::size_t size_image = 15;
    std::vector<int> image(size_image * size_image);

    if (rank == 0) {
        generateBinaryImage(&image, size_image);
    }

    auto parallel_res = createHullImageParallel(image, size_image);

    if (rank == 0) {
        auto sequential_res = createHullImageSequential(image, size_image);
        EXPECT_EQ(parallel_res, sequential_res);
    }
}

/*TEST(CREATE_HULL_PARALLEL, boost_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::size_t size_image = 1024;
    std::vector<int> image(size_image * size_image);

    if (rank == 0) {
        generateBinaryImage(&image, size_image);
    }

    auto launch_parallel = MPI_Wtime();
    auto parallel_res = createHullImageParallel(image, size_image);
    auto finish_parallel = MPI_Wtime();

    if (rank == 0) {
        auto launch_sequential = MPI_Wtime();
        auto sequential_res = createHullImageSequential(image, size_image);
        auto finish_sequential = MPI_Wtime();

        auto parallel_time = finish_parallel - launch_parallel;
        auto sequential_time = finish_sequential - launch_sequential;
        auto boost = sequential_time / parallel_time;

        printf("\nsequential_time=%f\nparallel_time=%f\nboost=%f\n", sequential_time, parallel_time, boost);
    }
}*/

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
