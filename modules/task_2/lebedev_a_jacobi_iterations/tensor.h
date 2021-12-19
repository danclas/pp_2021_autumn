// Copyright 2021 Lebedev Alexey
#ifndef MODULES_TASK_2_LEBEDEV_A_JACOBI_ITERATIONS_TENSOR_H_
#define MODULES_TASK_2_LEBEDEV_A_JACOBI_ITERATIONS_TENSOR_H_

#include <memory>
#include <vector>
#include <numeric>
#include <functional>
#include <stdexcept>

template <typename T>
class Tensor {
 private:
     std::shared_ptr<std::vector<T>> data;
     std::vector<int> shape;
     std::vector<int> strides;
     int size;
     const size_t prod(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end) {
         return std::accumulate(begin, end, 1, std::multiplies<size_t>{});
     }
     void init_strides() {
         strides.clear();
         for (size_t i = 1; i < shape.size(); i++) {
             strides.push_back(prod(shape.begin() + i, shape.end()));
         }
         strides.push_back(1);
     }

 public:
     Tensor(): size(0) {}
     explicit Tensor(const std::vector<int>& _shape): shape(_shape) {
         size = prod(shape.begin(), shape.end());
         data = std::make_shared<std::vector<T>>(size);
         init_strides();
     }
     Tensor(const Tensor<T>& t) = default;
     ~Tensor() = default;

     void resize(const std::vector<int>& _shape) {
         size = prod(_shape.begin(), _shape.end());
         if (data == nullptr) {
             data = std::make_shared<std::vector<T>>(size);
         } else {
             data->resize(size);
         }
         shape = _shape;
         init_strides();
     }

     bool is_allocated() const {
         return data != nullptr;
     }

     T* get_data() const {
         if (is_allocated()) {
             return data.get()->data();
         } else {
             return nullptr;
         }
     }
     int get_size() const {
         return size;
     }
     std::vector<int> get_shape() const {
         return shape;
     }
     std::vector<int> get_strides() const {
         return strides;
     }

     T& operator[] (const int offset) {
         if (is_allocated()) {
             return (*data.get())[offset];
         } else {
             throw std::length_error("Offset is out of range!");
         }
     }
     T operator[] (const int offset) const {
         if (is_allocated()) {
             return (*data.get())[offset];
         } else {
             throw std::length_error("Offset is out of range!");
         }
     }
};


template<typename T>
Tensor<T> operator + (const Tensor<T>& t1, const Tensor<T>& t2) {
    if (t1.get_shape() != t2.get_shape()) {
        throw std::logic_error("Tensor shapes must be equal for sum operation!");
    }
    Tensor<T> sum(t1.get_shape());
    for (int i = 0; i < t1.get_size(); i++) {
        sum[i] = t1[i] + t2[i];
    }
    return sum;
}


template<typename T>
Tensor<T> matmul2D(const Tensor<T>& t1, const Tensor<T>& t2) {
    std::vector<int> t1_shape(t1.get_shape()), t2_shape(t2.get_shape());
    if (t1_shape.size() != 2 || t2_shape.size() != 2) {
        throw std::length_error("matmul2D expects two-dimensional tensors.");
    }

    if (t1_shape[1] != t2_shape[0]) {
        throw std::logic_error("Incorrect shapes");
    }

    Tensor<T> result({t1_shape[0], t2_shape[1]});
    std::vector<int> t1_strides(t1.get_strides()),
                        t2_strides(t2.get_strides()),
                        result_strides(result.get_strides());

    for (int i = 0; i < t1_shape[0]; i++) {
        for (int j = 0; j < t2_shape[1]; j ++) {
            T local_sum = 0;
            for (int k = 0; k < t1_shape[1]; k++) {
                local_sum += t1[i * t1_strides[0] + k * t1_strides[1]] * t2[j * t2_strides[1] + k * t2_strides[0]];
            }
            result[i * result_strides[0] + j * result_strides[1]] = local_sum;
        }
    }

    return result;
}


#endif  // MODULES_TASK_2_LEBEDEV_A_JACOBI_ITERATIONS_TENSOR_H_
