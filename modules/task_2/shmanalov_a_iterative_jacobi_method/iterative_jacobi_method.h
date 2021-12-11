// Copyright 2021 Shmanalov Alexander
#ifndef MODULES_TASK_2_SHMANALOV_A_ITERATIVE_JACOBI_METHOD_ITERATIVE_JACOBI_METHOD_H_
#define MODULES_TASK_2_SHMANALOV_A_ITERATIVE_JACOBI_METHOD_ITERATIVE_JACOBI_METHOD_H_

void generationRandomData(int* matrixA, int* matrixB, int size);
void parallelIteration(int* tempA, int* tempB, double* xPrev, double* xRes,
                       int rank, int rowsOnProcess, int size);
double calcDifference(double* xPrev, double* xRes, int size);
void sequentialJacobi(int* matrixA, int* matrixB, double* resultX,
                      const double epsilon, int size);
void parallelJacobi(int* matrixA, int* matrixB, double* resultX,
                    const double epsilon, int size);


#endif  // MODULES_TASK_2_SHMANALOV_A_ITERATIVE_JACOBI_METHOD_ITERATIVE_JACOBI_METHOD_H_
