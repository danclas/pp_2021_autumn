// Copyright 2021 Kim Nikita
#include <mpi.h>
#include <vector>
#include <random>
#include "../../../modules/task_3/kim_n_sobel/sobel.h"


std::vector<std::vector<int>> getRandomImage(int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<std::vector<int>> res(size);
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      res[i].push_back(gen() % 256);
  return res;
}

int clamp(int value, int min, int max) {
  if (value < min)
    return min;
  if (value > max)
    return max;
  return value;
}

int calcNewPixelColor(std::vector<std::vector<int>> image, int x, int y, int size, int kernel[3][3]) {
  int new_pixel = 0;
  for (int l = -1; l <= 1; l++)
    for (int k = -1; k <= 1; k++) {
      int id_x = clamp(x + k, 0, size - 1);
      int id_y = clamp(y + l, 0, size - 1);
      int cur_pixel = image[id_x][id_y];
      new_pixel += cur_pixel * kernel[k + 1][l + 1];
    }
  return clamp(new_pixel, 0, size - 1);
}

int calcNewPixelColor(std::vector<int> image, int x, int y, int size, int kernel[3][3]) {
  int new_pixel = 0;
  for (int l = -1; l <= 1; l++)
    for (int k = -1; k <= 1; k++) {
      int id_x = clamp(x + k, 0, size - 1);
      int id_y = clamp(y + l, 0, size - 1);
      int cur_pixel = image[id_x * size + id_y];
      new_pixel += cur_pixel * kernel[k + 1][l + 1];
    }
  return clamp(new_pixel, 0, size - 1);
}

std::vector<std::vector<int>> getSobelImage(const std::vector<std::vector<int>>& image, int size, char type) {
  std::vector<std::vector<int>> sobel_image(size);
  int kernel[3][3];
  if (type == 'x') {
    // { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } }
    kernel[0][0] = -1;
    kernel[0][1] = 0;
    kernel[0][2] = 1;
    kernel[1][0] = -2;
    kernel[1][1] = 0;
    kernel[1][2] = 2;
    kernel[2][0] = -1;
    kernel[2][1] = 0;
    kernel[2][2] = 1;
  } else if (type == 'y') {
    // { { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } }
    kernel[0][0] = -1;
    kernel[0][1] = -2;
    kernel[0][2] = -1;
    kernel[1][0] = 0;
    kernel[1][1] = 0;
    kernel[1][2] = 0;
    kernel[2][0] = 1;
    kernel[2][1] = 2;
    kernel[2][2] = 1;
  }
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      sobel_image[i].push_back(calcNewPixelColor(image, i, j, size, kernel));
  return sobel_image;
}

std::vector<std::vector<int>> getSobelImageParall(const std::vector<std::vector<int>>& image, int size, char type) {
  int proc_num, proc_rank;
  int kernel[3][3];
  if (type == 'x') {
    // { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } }
    kernel[0][0] = -1;
    kernel[0][1] = 0;
    kernel[0][2] = 1;
    kernel[1][0] = -2;
    kernel[1][1] = 0;
    kernel[1][2] = 2;
    kernel[2][0] = -1;
    kernel[2][1] = 0;
    kernel[2][2] = 1;
  } else if (type == 'y') {
    // { { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } }
    kernel[0][0] = -1;
    kernel[0][1] = -2;
    kernel[0][2] = -1;
    kernel[1][0] = 0;
    kernel[1][1] = 0;
    kernel[1][2] = 0;
    kernel[2][0] = 1;
    kernel[2][1] = 2;
    kernel[2][2] = 1;
  }
  std::vector<int> local_image(size * size);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  int range = size / proc_num;
  int start = proc_rank * range;
  int end;
  if (proc_rank == proc_num - 1)
    end = size;
  else
    end = start + range;
  std::vector<int> recvcounts(proc_num);
  std::vector<int> displs(proc_num);
  if (proc_rank == 0) {
    for (int i = 0; i < proc_num; i++) {
      recvcounts[i] = range * size;
      displs[i] = i * range * size;
    }
    recvcounts[proc_num - 1] = (size - (proc_num - 1) * range) * size;
  }
  std::vector<int> local_sobel((end - start) * size);
  std::vector<int> global_sobel(size * size);
  std::vector<std::vector<int>> vectomat(size, std::vector<int>(size));
  if (proc_rank == 0) {
    std::vector<int> mattovec(size * size);
    for (int i = 0; i < size; i++)
      for (int j = 0; j < size; j++)
        mattovec[i * size + j] = image[i][j];
    for (int i = 1; i < proc_num; i++)
      MPI_Send(mattovec.data(), size * size, MPI_INT, i, 0, MPI_COMM_WORLD);
    local_image = mattovec;
  } else {
    MPI_Status status;
    MPI_Recv(local_image.data(), size * size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
  for (int i = start; i < end; i++)
    for (int j = 0; j < size; j++)
      local_sobel[(i-start) * size + j] = calcNewPixelColor(local_image, i, j, size, kernel);
  MPI_Gatherv(local_sobel.data(), (end - start) * size, MPI_INT,
    global_sobel.data(), recvcounts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
  if (proc_rank == 0) {
    for (int i = 0; i < size; i++)
      for (int j = 0; j < size; j++)
        vectomat[i][j] = global_sobel[i * size + j];
  }
  return vectomat;
}
