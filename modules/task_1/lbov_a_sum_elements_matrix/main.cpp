// Copyright 2021 Lbov Alexander
#include <gtest/gtest.h>
#include "./sum_element_matrix.h"
#include <gtest-mpi-listener.hpp>

TEST(Sum_Element_Matrix_MPI, Size_5_2) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int* matr = nullptr;
	const int a = 5, b = 2;

	if (rank == 0) matr = generate_matrix(a * b);
	
	int parr_sum_elem = parallel_sum(matr, a * b);

	if (rank == 0) {
		int no_parr_sum = not_parallel_sum(matr, a * b);
		ASSERT_EQ(parr_sum_elem, no_parr_sum);
	}
}

TEST(Sum_Element_Matrix_MPI, Size_10_5) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int* matr = nullptr;
	const int a = 10, b = 5;

	if (rank == 0) matr = generate_matrix(a * b);

	int parr_sum_elem = parallel_sum(matr, a * b);

	if (rank == 0) {
		int no_parr_sum = not_parallel_sum(matr, a * b);
		ASSERT_EQ(parr_sum_elem, no_parr_sum);
	}
}


TEST(Sum_Element_Matrix_MPI, Size_50_2) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int* matr = nullptr;
	const int a = 50, b = 2;

	if (rank == 0) matr = generate_matrix(a * b);

	int parr_sum_elem = parallel_sum(matr, a * b);

	if (rank == 0) {
		int no_parr_sum = not_parallel_sum(matr, a * b);
		ASSERT_EQ(parr_sum_elem, no_parr_sum);
	}
}



TEST(Sum_Element_Matrix_MPI, Size_2_1) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int* matr = nullptr;
	const int a = 2, b = 1;

	if (rank == 0) matr = generate_matrix(a * b);

	int parr_sum_elem = parallel_sum(matr, a * b);

	if (rank == 0) {
		int no_parr_sum = not_parallel_sum(matr, a * b);
		ASSERT_EQ(parr_sum_elem, no_parr_sum);
	}
}


TEST(Sum_Element_Matrix_MPI, Size_200_4) {
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int* matr = nullptr;
	const int a = 200, b = 4;

	if (rank == 0) matr = generate_matrix(a * b);

	int parr_sum_elem = parallel_sum(matr, a * b);

	if (rank == 0) {
		int no_parr_sum = not_parallel_sum(matr, a * b);
		ASSERT_EQ(parr_sum_elem, no_parr_sum);
	}
}



int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	MPI_Init(&argc, &argv);

	::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
	::testing::TestEventListeners& listener =
		::testing::UnitTest::GetInstance()->listeners();

	listener.Release(listener.default_result_printer());
	listener.Release(listener.default_xml_generator());

	listener.Append(new GTestMPIListener::MPIMinimalistPrinter);
	return RUN_ALL_TESTS();
}