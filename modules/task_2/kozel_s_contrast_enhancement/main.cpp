// Copyright 2021 Kozel Svetlana
#include <gtest/gtest.h>
#include "./contrast_enhancement.h"
#include <gtest-mpi-listener.hpp>

TEST(Change_contrast_MPI, test1) {
    int w = 3, h = 3, contrast = 20;
    std::vector<int> a(w * h), ans_par(w * h);
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        a[0] = 178;
        a[1] = 151;
        a[2] = 116;
        a[3] = 238;
        a[4] = 208;
        a[5] = 118;
        a[6] = 58;
        a[7] = 252;
        a[8] = 238;
        ans_par[0] = 179;
        ans_par[1] = 146;
        ans_par[2] = 104;
        ans_par[3] = 251;
        ans_par[4] = 215;
        ans_par[5] = 107;
        ans_par[6] = 35;
        ans_par[7] = 255;
        ans_par[8] = 251;
    }
    std::vector<int> ans(w * h);
    ans = contrastEnhancementPar(a, w, h, contrast);
    if (procRank == 0) {
        ASSERT_EQ(ans_par, ans);
        printIMG(a, w, h);
        printIMG(ans, w, h);
    }
}

TEST(Change_contrast_MPI, test2) {
    int w = 3, h = 3, contrast = 20;
    std::vector<int> a(w * h), ans_seq(w * h);
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        a[0] = 178;
        a[1] = 151;
        a[2] = 116;
        a[3] = 238;
        a[4] = 208;
        a[5] = 118;
        a[6] = 58;
        a[7] = 252;
        a[8] = 238;
        ans_seq[0] = 179;
        ans_seq[1] = 146;
        ans_seq[2] = 104;
        ans_seq[3] = 251;
        ans_seq[4] = 215;
        ans_seq[5] = 107;
        ans_seq[6] = 35;
        ans_seq[7] = 255;
        ans_seq[8] = 251;

        std::vector<int> ans(w * h);
        ans = contrastEnhancementSeq(a, w, h, contrast);

        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Change_contrast_MPI, test3) {
    int w = 200, h = 150, contrast = 20;
    std::vector<int> a(w * h);
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        a = randomIMG(w, h);
    }
    std::vector<int> ans(w * h);
    ans = contrastEnhancementPar(a, w, h, contrast);
    std::vector<int> ans_seq(w * h);
    if (procRank == 0) {
        ans_seq = contrastEnhancementSeq(a, w, h, contrast);
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Change_contrast_MPI, test4) {
    int w = 1000, h = 800, contrast = 80;
    std::vector<int> a(w * h);
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        a = randomIMG(w, h);
    }
    std::vector<int> ans(w * h);
    ans = contrastEnhancementPar(a, w, h, contrast);
    std::vector<int> ans_seq(w * h);
    if (procRank == 0) {
        ans_seq = contrastEnhancementSeq(a, w, h, contrast);
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(Change_contrast_MPI, test5) {
    int w = 16, h = 10, contrast = 20;
    std::vector<int> a(w * h);
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0) {
        a = randomIMG(w, h);
    }
    std::vector<int> ans(w * h);
    ans = contrastEnhancementPar(a, w, h, contrast);
    std::vector<int> ans_seq(w * h);
    if (procRank == 0) {
        ans_seq = contrastEnhancementSeq(a, w, h, contrast);
        ASSERT_EQ(ans_seq, ans);
        printIMG(a, w, h);
        printIMG(ans, w, h);
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
