// Copyright 2021 Lazarev Alexey
#include "../../../modules/task_2/lazarev_a_horiz_schem/horiz_schem.h"

std::vector<int> random_vector(int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<> urd(-100, 100);
  std::vector<int> vec(size * size);
  for (int i = 0; i < size * size; i++) {
    vec[i] = urd(gen);
  }
  return vec;
}

std::vector<int> transposition(std::vector<int> b, int size) {
  std::vector<int> temp = std::vector<int>(b.begin(), b.end());
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      b[i * size + j] = temp[j * size + i];
      {}
    }
  }
  return b;
}

std::vector<int> multiplication(std::vector<int> a, std::vector<int> b,
                                int size) {
  std::vector<int> c(size * size, 0);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      for (int k = 0; k < size; k++) {
        c[i * size + j] += a[i * size + k] * b[k * size + j];
      }
    }
  }
  return c;
}

std::vector<int> multiplication_parallel(std::vector<int> A,
                                         const std::vector<int>& B, int data) {
  std::vector<int> b(data * data);
  std::vector<int> C(data * data);
  int ProcRank, ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int* local_matrix = new int[ProcNum];
  if (ProcRank == 0) {
    b = transposition(B, data);
  }
  MPI_Bcast(b.data(), b.size(), MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < ProcNum; i++) {
    if (i < data % ProcNum) {
      local_matrix[i] = data * (data / ProcNum + 1);
    } else {
      local_matrix[i] = data * (data / ProcNum);
    }
  }
  std::vector<int> a(local_matrix[ProcRank]);

  if (ProcRank == 0) {
    int step = local_matrix[ProcRank];
    for (int i = 1; i < ProcNum; i++) {
      if (step == data * data) break;
      MPI_Send(&A[step], local_matrix[i], MPI_INT, i, 0, MPI_COMM_WORLD);
      step += local_matrix[i];
    }
    a = std::vector<int>(A.begin(), A.begin() + a.size());
  } else if (local_matrix[ProcRank] > 0) {
    MPI_Status status;
    MPI_Recv(a.data(), local_matrix[ProcRank], MPI_INT, 0, MPI_ANY_TAG,
             MPI_COMM_WORLD, &status);
  }

  std::vector<int> c(local_matrix[ProcRank], 0);
  int rows = local_matrix[ProcRank] / data;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < data; j++) {
      for (int k = 0; k < data; k++) {
        c[i * data + j] += a[i * data + k] * b[j * data + k];
      }
    }
  }

  std::vector<int> displs(ProcNum, 0);
  for (int i = 1; i < ProcNum; i++) {
    displs[i] = displs[i - 1] + local_matrix[i - 1];
  }
  MPI_Gatherv(c.data(), local_matrix[ProcRank], MPI_INT, C.data(), local_matrix,
              displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
  return C;
}
