// Copyright 2021 Kostin Maksim
#include "../../../modules/task_3/kostin_m_contrast_enhancement/contrast_enhancement.h"
#include <mpi.h>
#include <random>

std::vector<int> getRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = gen() % 100;
  }
  return vec;
}

int* getRandomImg(int width, int height) {
  std::random_device dev;
  std::mt19937 gen(dev());
  int* img = new int[width * height];
  for (int i = 0; i < width * height; i++) {
    img[i] = gen() % 255;
  }
  return img;
}
int GetLocalMin(int* local, int width, int height) {
  int min = local[0];
  for (int i = 0; i < width * height; i++) {
    if (local[i] < min) {
      min = local[i];
    }
  }
  return min;
}
int GetLocalMax(int* local, int width, int height) {
  int max = local[0];
  for (int i = 0; i < width * height; i++) {
    if (local[i] > max) {
      max = local[i];
    }
  }
  return max;
}
int crop(int val) {
  if (val < 0) return 0;
  if (val > 255) return 255;
  return val;
}
int* SequentialLinHist(int* img, int width, int height, int min, int max) {
  int* imp_img = new int[width * height];

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      imp_img[j + i * width] =
        crop((img[j + i * width] - min) * (255 / (max - min)));
    }
  }
  return imp_img;
}
int* ParallelLinHist(int* img, int width, int height) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int count = height / size;
  int rem = height % size;
  int local_min;
  int local_max;
  if (count) {
    if (rank == 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Send(img + proc * count * width + rem * width, count * width,
          MPI_INT, proc, 0, MPI_COMM_WORLD);
      }
    }
    int* local_image;
    local_image = nullptr;

    if (rank == 0) {
      local_image = new int[(count + rem) * width];

      for (int i = 0; i < (count + rem) * width; i++) {
        local_image[i] = img[i];
      }

      local_min = GetLocalMin(local_image, width, rem + count);
      local_max = GetLocalMax(local_image, width, rem + count);

    } else {
      local_image = new int[count * width];
      MPI_Status status;
      MPI_Recv(local_image, count * width, MPI_INT, 0, 0, MPI_COMM_WORLD,
        &status);

      local_min = GetLocalMin(local_image, width, count);
      local_max = GetLocalMax(local_image, width, count);
    }
    int global_min;
    int global_max;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    MPI_Bcast(&global_min, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&global_max, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int* global_image;
    global_image = nullptr;
    int tmp = 0;
    if (rank == 0) {
      tmp = rem;
    }
    local_image = SequentialLinHist(local_image, width, tmp + count,
      global_min, global_max);
    int *counts = new int[size], *displs = new int[size];
    if (rank == 0) {
      global_image = new int[width * height];
      for (int i = 0; i < size; i++) {
        if (i == 0) {
          counts[i] = (rem + count) * width;
          displs[i] = 0;
        } else {
          counts[i] = count * width;
          displs[i] = (rem + count * i) * width;
        }
      }
    }
    MPI_Gatherv(local_image, (tmp + count) * width, MPI_INT, global_image,
      counts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    return global_image;
  } else {
    if (rank == 0) {
      int loc_min = GetLocalMin(img, width, height);
      int loc_max = GetLocalMax(img, width, height);
      return SequentialLinHist(img, width, height, loc_min, loc_max);
    } else {
      return nullptr;
    }
  }
}
