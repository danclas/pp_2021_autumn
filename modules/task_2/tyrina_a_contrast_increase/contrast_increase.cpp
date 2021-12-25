// Copyright 2021 Tyrina Anastasia
#include "../../../modules/task_2/tyrina_a_contrast_increase/contrast_increase.h"

#include <mpi.h>

#include <ctime>
#include <random>

double* getRandomImage(int width, int height) {
  std::mt19937 gen(time(0));
  std::uniform_real_distribution<> urd(0.3, 0.7);
  double* img = new double[width * height];
  for (int i = 0; i < width * height; i++) {
    img[i] = urd(gen);
  }
  return img;
}

double truncate(double value) {
  if (value < 0) return 0.0;
  if (value > 255.0) return 255.0;

  return value;
}

double* getSequentialContrastIncrease(double* image, int width, int height,
                                      double contrast) {
  double* new_image = new double[width * height];

  double factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      new_image[c + r * width] =
          truncate(factor * (image[c + r * width] * 255.0 - 128.0) + 128.0) /
          255.0;
    }
  }
  return new_image;
}

double* getParallelContrastIncrease(double* image, int width, int height,
                                    double contrast) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int delta = height / size;
  const int rem = height % size;

  if (delta) {
    if (rank == 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Send(image + proc * delta * width + rem * width, delta * width,
                 MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
      }
    }

    double* local_img;
    if (rank == 0) {
      local_img = new double[(delta + rem) * width];
      for (int i = 0; i < (delta + rem) * width; i++) local_img[i] = image[i];
    } else {
      local_img = new double[delta * width];
      MPI_Status status;
      MPI_Recv(local_img, delta * width, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
               &status);
    }

    double* global_img = nullptr;
    int temp = 0;
    if (rank == 0) {
      temp = rem;
    }

    local_img =
        getSequentialContrastIncrease(local_img, width, delta + temp, contrast);

    int *counts = new int[size], *displs = new int[size];
    if (rank == 0) {
      global_img = new double[width * height];
      for (int i = 0; i < size; i++) {
        if (i == 0) {
          counts[i] = (delta + rem) * width;
          displs[i] = 0;
        } else {
          counts[i] = delta * width;
          displs[i] = (delta * i + rem) * width;
        }
      }
    }
    MPI_Gatherv(local_img, (delta + temp) * width, MPI_DOUBLE, global_img,
                counts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    return global_img;
  } else {
    if (rank == 0) {
      return getSequentialContrastIncrease(image, width, height, contrast);
    } else {
      return nullptr;
    }
  }
}
