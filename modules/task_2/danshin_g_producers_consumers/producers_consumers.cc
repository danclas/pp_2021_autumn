// Copyright 2021 Gleb "belgad" Danshin
#include "../../../modules/task_2/danshin_g_producers_consumers/producers_consumers.h"

void producers_consumers(int product_number) {
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size < 3) {
        return;
    }

    int proc_num_prod = (world_size - 1) / 2;

    if (world_rank == 0) {
        hub(product_number);
    } else if (world_rank <= proc_num_prod) {
        producer();
    } else {
        consumer();
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

void hub(int product_number) {
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int producer_number = (world_size - 1) / 2;
    int consumer_number = world_size - 1 - producer_number;

    int prod_num_prod_div = product_number / producer_number,
    prod_num_prod_mod = product_number % producer_number;
    int prod_num_cons_div = product_number / consumer_number,
    prod_num_cons_mod = product_number % consumer_number;

    int prod_num_atomic;
    int* prod_num_atomic_array = new int[1 + producer_number + consumer_number];
    prod_num_atomic_array[0] = 0;
    for (int i = 1; i <= producer_number; ++i) {
        prod_num_atomic_array[i] = prod_num_prod_div + (--prod_num_prod_mod >= 0);
    }
    for (int i = 1; i <= consumer_number; ++i) {
        prod_num_atomic_array[i + producer_number] = prod_num_cons_div + (--prod_num_cons_mod >= 0);
    }
    MPI_Scatter(prod_num_atomic_array, 1, MPI_INT, &prod_num_atomic, 1, MPI_INT, 0, MPI_COMM_WORLD);

    const int N = 10;
    int buffer[N];
    int left = 0, right = 0;
    bool empty = true, full = false;

    MPI_Request* requests = new MPI_Request[world_size - 1];
    MPI_Request* producer_requests = requests;
    MPI_Request* consumer_requests = requests + producer_number;
    int *recieved_data = new int[producer_number];
    for (int i = 0; i < producer_number; ++i) {
        producer_requests[i] = MPI_REQUEST_NULL;
        MPI_Irecv(recieved_data + i, 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD, producer_requests + i);
    }
    for (int i = 0; i < consumer_number; ++i) {
        consumer_requests[i] = MPI_REQUEST_NULL;
        MPI_Irecv(nullptr, 0, MPI_INT, 1 + producer_number + i, 3, MPI_COMM_WORLD, consumer_requests + i);
    }

    for (int i = 1; i <= 2 * product_number; ++i) {
        int index;
        MPI_Waitany(world_size - 1, requests, &index, MPI_STATUS_IGNORE);
        if (index < producer_number) {
            if (full) {
                int cons_index;
                MPI_Waitany(consumer_number, consumer_requests, &cons_index, MPI_STATUS_IGNORE);
                MPI_Ssend(buffer + left, 1, MPI_INT, 1 + producer_number + cons_index, 2, MPI_COMM_WORLD);
                MPI_Irecv(nullptr, 0, MPI_INT, 1 + producer_number + cons_index, 3, MPI_COMM_WORLD, consumer_requests + cons_index);
                left = (left + 1) % N;
                buffer[right] = recieved_data[index];
                right = (right + 1) % N;
                ++i;
            } else {
                empty = false;
                buffer[right] = recieved_data[index];
                right = (right + 1) % N;
                if (left == right) {
                    full = true;
                }
            }
            MPI_Irecv(recieved_data + index, 1, MPI_INT, 1 + index, 1, MPI_COMM_WORLD, requests + index);
        } else {
            if (empty) {
                int prod_index;
                MPI_Waitany(producer_number, producer_requests, &prod_index, MPI_STATUS_IGNORE);
                MPI_Ssend(recieved_data + prod_index, 1, MPI_INT, 1 + index, 2, MPI_COMM_WORLD);
                MPI_Irecv(recieved_data + prod_index, 1, MPI_INT, 1 + prod_index, 1, MPI_COMM_WORLD, producer_requests + prod_index);
                ++i;
            } else {
                full = false;
                MPI_Ssend(&buffer[left], 1, MPI_INT, 1 + index, 2, MPI_COMM_WORLD);
                left = (left + 1) % N;
                if (left == right) {
                    empty = true;
                }
            }
            MPI_Irecv(nullptr, 0, MPI_INT, 1 + index, 3, MPI_COMM_WORLD, requests + index);
        }
    }

    for (int i = 0; i < world_size - 1; ++i) {
        MPI_Cancel(requests + i);
        MPI_Request_free(requests + i);
    }

    delete[] prod_num_atomic_array;
    producer_requests = nullptr;
    consumer_requests = nullptr;
    delete[] requests;
    delete[] recieved_data;
}

void producer() {
    int world_size, world_rank; MPI_Comm_size(MPI_COMM_WORLD, &world_size); MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::random_device random_device;
    std::mt19937 random_engine(random_device());

    int product_number_atomic;
    MPI_Scatter(nullptr, 1, MPI_INT, &product_number_atomic, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int product;
    for (int i = 0; i < product_number_atomic; ++i) {
        std::this_thread::sleep_for(std::chrono::microseconds((random_engine() & 0x1F)));
        product = random_engine() & 0xFF;
        MPI_Ssend(&product, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
}

void consumer() {
    int world_size, world_rank; MPI_Comm_size(MPI_COMM_WORLD, &world_size); MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::random_device random_device;
    std::mt19937 random_engine(random_device());

    int product_number_atomic;
    MPI_Scatter(nullptr, 1, MPI_INT, &product_number_atomic, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int product;
    for (int i = 0; i < product_number_atomic; ++i) {
        MPI_Ssend(nullptr, 0, MPI_INT, 0, 3, MPI_COMM_WORLD);
        MPI_Recv(&product, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::this_thread::sleep_for(std::chrono::microseconds((random_engine() & 0x1F)));
    }
}
