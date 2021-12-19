// Copyright 2021 Boyarskov Anton

#include "../../../modules/task_2/boyarskov_a_image_smoothing/image_smoothing.h"

#include <mpi.h>

#include <ctime>
#include <random>

int* getRandomImage(int width, int height) {
  std::mt19937 gen(time(0));
  std::uniform_int_distribution<> uid(0, 255);
  int* img = new int[width * height];
  for (int i = 0; i < width * height; i++) {
    img[i] = uid(gen);
  }
  return img;
}

float calcNewPixelColor(int* img, int x, int y, int width, int height,
                        int radius) {
  int size = 2 * radius + 1;
  int len = size * size;

  float* kernel = new float[len];
  float sigma = 3.f;
  float norm = 0;
  int signed_radius = static_cast<int>(radius);

  for (int x = -signed_radius; x <= signed_radius; x++)
    for (int y = -signed_radius; y <= signed_radius; y++) {
      std::size_t idx = (x + radius) * size + (y + radius);
      kernel[idx] = std::exp(-(x * x + y * y) / (sigma * sigma));
      norm += kernel[idx];
    }
  for (int i = 0; i < len; i++) {
    kernel[i] /= norm;
  }

  float returnC = 0;

  for (int i = -radius; i <= radius; i++) {
    for (int j = -radius; j <= radius; j++) {
      int idx = (i + radius) * size + j + radius;
      int pixel = img[x + j + (y + i) * width];
      returnC += pixel * kernel[idx];
    }
  }
  return returnC;
}

int* getSequentialGauss(int* image, int width, int height, int radius) {
  int* new_image = new int[(width - radius * 2) * (height - radius * 2)];

  for (int i = radius; i < height - radius; i++) {
    for (int j = radius; j < width - radius; j++) {
      new_image[j - radius + (i - radius) * (width - radius * 2)] =
          calcNewPixelColor(image, j, i, width, height, radius);
    }
  }
  return new_image;
}

int* getParallelGauss(int* image, int width, int height, int radius) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int delta = 0;
  int rem = 0;

  delta = (height - radius * 2) / size;
  rem = (height - radius * 2) % size;

  if (delta) {
    int row = rem + delta;

    if (rank == 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Send(image + row * width, (delta + radius * 2) * width, MPI_INT,
                 proc, 0, MPI_COMM_WORLD);
        row += delta;
      }
    }

    int* local_matrix;

    if (rank == 0) {
      local_matrix = new int[(delta + rem + radius * 2) * width];
      for (int i = 0; i < (delta + rem + radius * 2) * width; i++) {
        local_matrix[i] = image[i];
      }
      local_matrix = getSequentialGauss(local_matrix, width,
                                        delta + rem + radius * 2, radius);
    } else {
      local_matrix = new int[(delta + radius * 2) * width];
      MPI_Status status;
      MPI_Recv(local_matrix, (delta + radius * 2) * width, MPI_INT, 0, 0,
               MPI_COMM_WORLD, &status);
      local_matrix =
          getSequentialGauss(local_matrix, width, delta + radius * 2, radius);
      MPI_Send(local_matrix, delta * (width - radius * 2), MPI_INT, 0, 0,
               MPI_COMM_WORLD);
    }

    int* global_matrix = nullptr;

    if (rank == 0) {
      global_matrix = new int[(width - radius * 2) * (height - radius * 2)];
      for (int i = 0; i < delta + rem; i++) {
        for (int j = 0; j < width - radius * 2; j++) {
          global_matrix[j + i * (width - radius * 2)] =
              local_matrix[j + i * (width - radius * 2)];
        }
      }
      for (int proc = 1; proc < size; proc++) {
        int* temp = new int[delta * (width - radius * 2)];
        MPI_Status status;
        MPI_Recv(temp, delta * (width - radius * 2), MPI_INT, proc, 0,
                 MPI_COMM_WORLD, &status);
        for (int i = proc * delta + rem; i < proc * delta + rem + delta; i++) {
          for (int j = 0; j < width - radius * 2; j++) {
            global_matrix[j + i * (width - radius * 2)] =
                temp[j + (i - proc * delta - rem) * (width - radius * 2)];
          }
        }
      }
    }

    return global_matrix;
  } else {
    if (rank == 0) {
      return getSequentialGauss(image, width, height, radius);
    } else {
      return nullptr;
    }
  }
}
