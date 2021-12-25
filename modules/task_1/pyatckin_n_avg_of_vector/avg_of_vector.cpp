// Copyright 2021 Pyatckin Nickolay
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/pyatckin_n_avg_of_vector/avg_of_vector.h"


std::vector<double> getRandomVector(int sz) {
	std::mt19937 gen(121);
	std::vector<double> vec(sz);
	for (int i = 0; i < sz; i++) { vec[i] = gen() % 1000 / 100.0; }
	return vec;
}

double getSequentialOperations(std::vector<double>& vec, int size_vec) {
	double local_average = 0;
	for (std::vector<double>::size_type i = 0; i < size_vec; i++) {
		local_average += vec[i];
	}
	return local_average / static_cast<double>(size_vec);
}

double getParallelOperations(std::vector<double>& vec, int size_vec) {
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	const int delta = size_vec / size;
	double global_average = 0;
	double tail_average = 0;
	std::vector<double> subset_of_vec(delta);

	if (rank == 0) {
		for (int proc = 1; proc < size; proc++) {
			MPI_Send(vec.data() + proc * delta, delta,
				MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
		}
	}

	std::vector<double> local_vec(delta);
	double local_average = 0;
	if (rank == 0) {
		local_vec = std::vector<double>(vec.begin(),
			vec.begin() + delta);
	}
	else {
		MPI_Recv(local_vec.data(), delta, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}
	local_average = getSequentialOperations(local_vec, delta);

	MPI_Reduce(&local_average, &global_average, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		int tail = size_vec - (delta * size);
		if (tail) {
			subset_of_vec = std::vector<double>(vec.begin() + size * delta, vec.end());
			tail_average = getSequentialOperations(subset_of_vec, tail);
			return (tail_average + global_average) / static_cast<double>(size);
		}
	}
	return global_average / static_cast<double>(size);
}
