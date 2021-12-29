// Copyright 2021 Stepanov Alexander
#include <mpi.h>
#include <vector>
#include <random>
#include <stack>
#include <algorithm>
#include "../../../modules/task_3/stepanov_a_convex_hull_binary_image/convex_hull_binary_image.h"


void generateBinaryImage(std::vector<int>* image,
    std::size_t size_image) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<> urd(0, 1);

    for (std::size_t i = 0; i < size_image * size_image; i++) {
        auto random_value = urd(gen);
        (*image)[i] = random_value > 0.5 ? 1 : 0;
    }
}

std::vector<std::vector<int>> markingComponents(std::vector<int>* image,
    std::size_t size_image) {
    std::size_t last_marker = 0;
    std::vector<std::vector<int>> container_components;

    for (std::size_t index = 0; index < size_image * size_image; index++) {
        if ((*image)[index] == 1) {
            container_components.push_back(std::vector<int>{});

            markingNeighbors(image, size_image, &container_components[last_marker],
                static_cast<int>(last_marker + 2), index);

            last_marker++;
        }
    }

    return container_components;
}

void markingNeighbors(std::vector<int>* image, std::size_t size_image,
    std::vector<int>* current_component, int marker, std::size_t start_index) {
    std::stack<std::size_t> pixel_stack;
    pixel_stack.push(start_index);
    (*image)[start_index] = marker;

    while (pixel_stack.size() > 0) {
        std::size_t pos = pixel_stack.top();
        (*current_component).push_back(static_cast<int>(pos));
        pixel_stack.pop();

        std::size_t right_pos = pos + 1,
        down_pos = pos + size_image,
        left_pos = pos - 1,
        upper_pos = pos - size_image;

        if (right_pos < size_image * size_image && right_pos % size_image > 0 &&
            (*image)[right_pos] == 1) {
            (*image)[right_pos] = marker;
            pixel_stack.push(right_pos);
        }

        if (down_pos < size_image * size_image && (*image)[down_pos] == 1) {
            (*image)[down_pos] = marker;
            pixel_stack.push(down_pos);
        }


        if (pos > 0 && pos % size_image > 0 && (*image)[left_pos] == 1) {
            (*image)[left_pos] = marker;
            pixel_stack.push(left_pos);
        }

        if (pos >= size_image && (*image)[upper_pos] == 1) {
            (*image)[upper_pos] = marker;
            pixel_stack.push(upper_pos);
        }
    }
}

std::vector<std::vector<int>> createHullImageSequential(const std::vector<int>& image,
    std::size_t size_image) {
    std::vector<int> marking_image = image;
    auto container_components = markingComponents(&marking_image, size_image);

    std::vector<std::vector<int>> container_hulls(container_components.size(), std::vector<int>{});

    for (std::size_t i = 0; i < container_components.size(); i++) {
        container_hulls[i] = createHullComponent(container_components[i], static_cast<int>(size_image));
    }

    return container_hulls;
}

std::vector<int> createHullComponent(const std::vector<int>& current_component,
    std::size_t size_image) {
    std::size_t left_border = size_image, right_border = 0,
        upper_border = size_image, down_border = 0;

    for (const auto& pixel : current_component) {
        std::size_t column_position = pixel % size_image;
        std::size_t row_position = pixel / size_image;

        left_border = std::min(column_position, left_border);
        right_border = std::max(column_position, right_border);
        upper_border = std::min(row_position, upper_border);
        down_border = std::max(row_position, down_border);
    }

    return { static_cast<int>(upper_border), static_cast<int>(right_border),
        static_cast<int>(down_border), static_cast<int>(left_border) };
}

std::vector<std::vector<int>> createHullImageParallel(const std::vector<int>& image,
    std::size_t size_image) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::size_t local_size = size_image / size;
    std::size_t remains = size_image % size;

    std::vector<int> local_image(size_image * size_image);

    if (rank == 0) {
        local_image = image;
        local_size += remains;
    }

    MPI_Bcast(local_image.data(), static_cast<int>(size_image * size_image), MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int>local_res(local_size * size_image * 4 + (rank == 0 ? 0 : remains * size_image * 4), -1);
    std::size_t start_index = rank == 0 ? 0 : local_size * size_image * rank + remains * size_image, last_index = 0;;

    for (std::size_t i = 0; i < local_size * size_image; i++) {
        if (local_image[start_index + i] == 1) {
            std::vector<int> component;
            markingNeighbors(&local_image, size_image,
                &component, static_cast<int>(i + 2), start_index + i);

            auto hull = createHullComponent(component, size_image);

            int step_row = static_cast<int>(rank == 0 ? 0 : rank * local_size + remains);
            int upper_border = hull[0];
            if (upper_border >= step_row && upper_border < static_cast<int>(step_row + local_size)) {
                for (std::size_t j = 0; j < 4; j++) {
                    local_res[last_index++] = hull[j];
                }
            }
        }
    }

    std::vector<int> global_res(local_size * size_image * 4 * size);
    MPI_Gather(local_res.data(), static_cast<int>(local_res.size()), MPI_INT,
        global_res.data(), static_cast<int>(local_res.size()), MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<std::vector<int>> all_hull;
    if (rank == 0) {
        std::size_t next_proc_data = local_size * size_image * 4;
        std::vector<int> hull(4);
        for (std::size_t i = 0; i < global_res.size(); i+=4) {
            if (global_res[i] == -1) {
                i = next_proc_data - 4;
                next_proc_data += local_size * size_image * 4;
            } else {
                for (std::size_t j = 0; j < 4; j++) {
                    hull[j] = global_res[i + j];
                }
                all_hull.push_back(hull);
            }
        }
    }

    return all_hull;
}
