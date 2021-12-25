// Copyright 2021 Lbov Alexander
#include <mpi.h>
#include <random>
#include "../../../modules/task_1/lbov_a_sum_elements_matrix/sum_element_matrix.h"

int* generate_matrix(int size) {  
    std::random_device dev;
    std::mt19937 gen(dev());
    int* matrix = new int[size];
    for (int i = 0; i < size; i++) {
        matrix[i] = gen() % 100;
    }
    return matrix;
}

int not_parallel_sum(int* matrix, int size) {  
    int s = 0;
    for (int i = 0; i < size; i++)
        s += matrix[i];
    return s;
}

int parallel_sum(int* matrix, int size) {
    int lsize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &lsize); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     const int okr = size / lsize;  
    const int ost = size % lsize;

if (rank == 0){
       
for (int process = 1; process < lsize; process++) {
MPI_Send(matrix + process * okr + ost, okr, MPI_INT, process, 0, MPI_COMM_WORLD);
}
}
     
   
    

int* l_matrix = nullptr; 
if (rank == 0) {
l_matrix = new int[ost + okr];
}
else{
l_matrix = new int[okr];
}
    
if (rank == 0) {
for (int i = 0; i < ost + okr; i++) {
l_matrix[i] = matrix[i];
}
}
else{
MPI_Status status;
MPI_Recv(l_matrix, okr, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
}

int sum = 0;
int l_sum = 0;

if (rank == 0){
l_sum = not_parallel_sum(l_matrix, ost + okr);
}
else{
l_sum = not_parallel_sum(l_matrix, okr);
}

MPI_Reduce(&l_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
return sum;
}