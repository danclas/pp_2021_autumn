// Copyright 2021 Kazhaeva Anastasia
#include <mpi.h>
#include <ctime>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/kazhaeva_a_horizontal_gaussian_filter/horizontal_filter.h"

void Kernell(vector<float>* m, float sigma) {
  std::mt19937 gen(time(0));
  std::uniform_real_distribution<> urd(0, 1);
  float norm = 0;
  m->resize(9);
  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      int idx = (x + 1) * 3 + (y + 1);
      (*m)[idx] = std::exp(-(x * x + y * y) / (sigma * sigma));
      norm += (*m)[idx];
    }
  }
  for (int i = 0; i < 9; i++) {
    if ((i != 1) && (i != 4) && (i != 7))(*m)[i] = 0.f;
    (*m)[i] /= norm;
  }
}


float calculated(const vector<float>& matrix,
  const vector<float>& img, int height, int weight, const int x, const int y) {
  float c = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int idx = (i + 1) * 3 + j + 1;
      float imgColor = img[clamp(y + i, height - 1, 0)
        * weight + clamp(x + j, weight - 1, 0)];
      c += (imgColor)*matrix[idx];
    }
  }
  return c;
}
std::vector<float> Sequential(const std::vector<float>& matrix,
  const std::vector<float>& image, int weight, int height) {
  vector<float> resultMatrix(weight * height);
  for (int x = 0; x < weight; ++x) {
    for (int y = 0; y < height; y++) {
      float color = 0;
      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          int idx = (i + 1) * 3 + j + 1;
          float imgColor = image[clamp(y + i, height - 1, 0)*
            weight+clamp(x + j, weight - 1, 0)];
          color += (imgColor)*matrix[idx];
        }
      }
      resultMatrix[y * weight + x] = color;
    }
  }
  return resultMatrix;
}


std::vector<float> Parallel(const std::vector<float>& matrix,
  const std::vector<float>& img, int weight, int height) {
  int num, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  int* sendcounts = nullptr;
  int* displs = nullptr;
  vector<float>res(weight * height);

  sendcounts = new int[num];
  displs = new int[num + 1];
  int ost = height % num;
  int sum = 0;

  for (int i = 0; i < num; i++) {
    sendcounts[i] = (height / num) * weight;
    if (ost > 0) {
      sendcounts[i] += weight;
      ost--;
    }
    displs[i] = sum;
    sum += sendcounts[i] / weight;
  }
  displs[num] = height;

  vector<float>temp(sendcounts[rank]);
  int t = 0;
  for (int i = displs[rank]; i < displs[rank + 1]; i++) {
    for (int j = 0; j < weight; j++) {
      float value = calculated(matrix, img, height, weight, j, i);
      temp[t * weight + j] = value;
    }
    t++;
  }

  if (rank == 0) {
    for (int i = 0; i < displs[rank + 1]; i++)
      for (int j = 0; j < weight; j++)
        res[i * weight + j] = temp[i * weight + j];
    for (int proc = 1; proc < num; proc++) {
      vector<float> resultMatrixTemp(sendcounts[proc]);

      MPI_Recv(resultMatrixTemp.data(), resultMatrixTemp.size(),
        MPI_FLOAT, proc, 0, MPI_COMM_WORLD, &status);
      int t = 0;
      for (int i = displs[proc]; i < displs[proc + 1]; i++) {
        for (int j = 0; j < weight; j++) {
          res[i * weight + j] = resultMatrixTemp[t * weight + j];
        }
        t++;
      }
    }
  } else {
    MPI_Send(temp.data(), temp.size(), MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
  }

  return res;
}


int randNumber(std::random_device* dev, std::mt19937* rng, int max) {
  std::uniform_int_distribution<std::mt19937::result_type> dist(0, max - 1);
  return dist(*rng);
}

std::vector<float> RandomImg(int weight, int height) {
  std::random_device dev;
  std::vector<float> img(weight * height);
  std::mt19937 rng(dev());
  rng.seed(5);
  for (int i = 0; i < weight; i++) {
    for (int j = 0; j < height; j++) {
      std::mt19937 gen(time(0));
      img[i * height + j] = static_cast<float>(
        randNumber(&dev, &rng, 255) % 256);
    }
  }
  return img;
}

