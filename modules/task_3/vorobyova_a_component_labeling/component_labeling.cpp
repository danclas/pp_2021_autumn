// Copyright 2021 Vorobyova Anna
#include "../../modules/task_3/vorobyova_a_component_labeling/component_labeling.h"

#include <mpi.h>

std::vector<int> getRandomImage(int n, int m) {
  std::mt19937 gen(time(0));
  std::uniform_int_distribution<> uniform(0, 1);
  vector<int> image(n * m);
  for (int i = 0; i < n * m; i++) {
    image[i] = uniform(gen);
  }
  return image;
}

pair<vector<int>, pair<vector<int>, int> > partOne(vector<int> image, int n,
                                                   int m, int start) {
  int c = 0;
  int matrix_size = n * m;
  vector<int> srm(matrix_size);
  vector<int> map(image.begin(), image.begin() + matrix_size);
  for (int x = 0; x < matrix_size; x++) srm[x] = x + start;

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      int index = i * n + j;
      if (map[index] != 0) {
        int left, upper;
        if (index < n) {
          left = 0;
        } else {
          left = map[index - n];
        }
        if (((index < 1) || ((index - 1) / n != i))) {
          upper = 0;
        } else {
          upper = map[index - 1];
        }

        if ((left == 0) && (upper == 0)) {
          map[index] = index + start + 1;
          c++;
        }
        if ((left == 0) && (upper != 0)) map[index] = upper;
        if ((left != 0) && (upper == 0)) map[index] = left;
        if ((left != 0) && (upper != 0)) {
          if (left == upper) {
            map[index] = left;
          } else {
            int max_pixel = max(left, upper);
            while (srm[max_pixel - start] != max_pixel)
              max_pixel = srm[max_pixel - start];

            int min_pixel = min(left, upper);
            while (srm[min_pixel - start] != min_pixel)
              min_pixel = srm[min_pixel - start];

            if (max_pixel != min_pixel) {
              srm[max_pixel - start] = min_pixel;
              c--;
            }

            map[index] = min_pixel;
          }
        }
      }
    }
  }

  return make_pair(map, make_pair(srm, c));
}

vector<int> partTwo(vector<int> map, vector<int> srm, int n, int m) {
  int matrix_size = n * m;
  vector<int> final_matrix(matrix_size);
  int total = matrix_size;
  for (int pos = 0; pos < total - 1; pos++) {
    int pixel = map[pos];
    if (pixel != 0) {
      if (srm[pixel] == pixel) {
        final_matrix[pos] = pixel;
      } else {
        while (srm[pixel] != pixel) pixel = srm[pixel];
        final_matrix[pos] = pixel;
      }
    }
  }
  return final_matrix;
}

pair<vector<int>, int> ComponentLabeling(vector<int> image, int n, int m) {
  pair<vector<int>, pair<vector<int>, int> > part_one = partOne(image, n, m);

  vector<int> map = part_one.first;
  vector<int> srm = part_one.second.first;
  int c = part_one.second.second;

  vector<int> final_matrix = partTwo(map, srm, n, m);

  return make_pair(final_matrix, c);
}

pair<vector<int>, int> ComponentLabelingParallel(vector<int> image, int n,
                                                 int m) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (size == 1) {
    return ComponentLabeling(image, n, m);
  }

  int matrix_size = n * m;
  const int delta = m / size * n;
  const int remainder = (m % size) * n;

  if (delta == 0) {
    if (rank == 0) {
      return ComponentLabeling(image, n, m);
    } else {
      return make_pair(vector<int>{}, 0);
    }
  }

  if (rank == 0) {
    for (int proc = 1; proc < size; proc++)
      MPI_Send(image.data() + remainder + proc * delta, delta, MPI_INT, proc, 0,
               MPI_COMM_WORLD);
  }

  vector<int> local_matrix(delta + remainder);

  if (rank == 0) {
    local_matrix =
        vector<int>(image.cbegin(), image.cbegin() + delta + remainder);
  } else {
    MPI_Recv(local_matrix.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUSES_IGNORE);
  }

  int local_height = 0, local_label = delta * rank;
  if (rank == 0) {
    local_height = (remainder + delta) / n;
  } else {
    local_height = delta / n;
    local_label += remainder;
  }
  pair<vector<int>, pair<vector<int>, int> > part_one =
      partOne(local_matrix, n, local_height, local_label);

  vector<int> map = part_one.first, srm = part_one.second.first;
  int local_labels_counts = part_one.second.second;

  vector<int> starts(size), deltas(size);
  deltas[0] = remainder + delta;
  deltas[1] = delta;
  starts[1] = remainder + delta;
  for (int proc = 2; proc < size; proc++) {
    starts[proc] = starts[proc - 1] + delta;
    deltas[proc] = delta;
  }

  vector<int> global_srm(matrix_size);
  int temp = delta;
  if (rank == 0) temp += remainder;

  MPI_Gatherv(srm.data(), temp, MPI_INT, global_srm.data(), deltas.data(),
              starts.data(), MPI_INT, 0, MPI_COMM_WORLD);

  vector<int> global_map(matrix_size);
  MPI_Gatherv(map.data(), temp, MPI_INT, global_map.data(), deltas.data(),
              starts.data(), MPI_INT, 0, MPI_COMM_WORLD);

  int global_c = 0;
  MPI_Reduce(&local_labels_counts, &global_c, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  vector<int> global_matrix(matrix_size);
  if (rank == 0) {
    for (int j = 1; j < size; j++) {
      int row_two = delta * j;
      if (j != 0) {
        row_two += remainder;
      }
      int row_one = row_two - n;

      for (int x = 0; x < n; x++) {
        int left = global_map[row_one + x];
        int upper = global_map[row_two + x];
        if ((left != 0) && (upper != 0)) {
          int left_set = global_srm[left];
          int upper_set = global_srm[upper];
          if (left_set != upper_set) {
            int max_pixel = max(left, upper);

            while (global_srm[max_pixel] != max_pixel)
              max_pixel = global_srm[max_pixel];

            int min_pixel = min(left, upper);

            while (global_srm[min_pixel] != min_pixel)
              min_pixel = global_srm[min_pixel];

            if (max_pixel != min_pixel) {
              global_srm[max_pixel] = min_pixel;
              global_c--;
            }
          }
        }
      }
    }
    global_matrix = partTwo(global_map, global_srm, n, m);
  }

  return make_pair(global_matrix, global_c);
}
