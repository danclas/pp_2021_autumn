// Copyright 2021 Ershov Alexey
#include "./reduce_mpi.h"

#include <mpi.h>

#include <random>

Matrix getRandomMatrix(const size_t m, const size_t n) {
  double* tmp = new double[m * n];
  std::random_device dev;
  std::mt19937 gen(dev());
  for (size_t i = 0; i < m * n; ++i) {
    tmp[i] = gen() % 100;
  }
  Matrix matrix(tmp, m, n);
  delete[] tmp;
  tmp = nullptr;
  return matrix;
}

Matrix::Matrix() {
  this->data = nullptr;
  this->weight = 0;
  this->height = 0;
}

Matrix::Matrix(double* data, size_t weight, size_t height) {
  this->weight = weight;
  this->height = height;
  this->data = new double[this->height * this->weight];
  for (size_t i = 0; i < (this->height * this->weight); ++i) {
    this->data[i] = data[i];
  }
}

Matrix::Matrix(const Matrix& a) {
  this->height = a.height;
  this->weight = a.weight;
  this->data = new double[this->height * this->weight];
  for (size_t i = 0; i < (this->weight * this->height); ++i) {
    this->data[i] = a.data[i];
  }
}

Matrix::~Matrix() {
  delete[] this->data;
  this->data = nullptr;
}

Matrix& Matrix::operator=(const Matrix& right) {
  if (this == &right) {
    return *this;
  }
  this->height = right.height;
  this->weight = right.weight;
  this->data = new double[this->height * this->weight];
  for (size_t i = 0; i < (this->weight * this->height); ++i) {
    this->data[i] = right.data[i];
  }
  return *this;
}

void Matrix::setMatrix(double* data, size_t weight, size_t height) {
  this->weight = weight;
  this->height = height;
  this->data = new double[this->height * this->weight];
  for (size_t i = 0; i < (this->height * this->weight); ++i) {
    this->data[i] = data[i];
  }
}

template <typename T>
T* getRandomVector(size_t size) {
  T* vec = new T[size];
  std::random_device dev;
  std::mt19937 gen(dev());
  for (size_t i = 0; i < size; ++i) {
    vec[i] = gen() % 100;
  }
  return vec;
}

int reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype type,
           MPI_Op op, int root, MPI_Comm comm) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Barrier(comm);

  if (rank == root) {
    int typeSize = 0;
    if (type == MPI_INT) {
      typeSize = sizeof(int);
    } else if (type == MPI_DOUBLE) {
      typeSize = sizeof(double);
    } else if (type == MPI_FLOAT) {
      typeSize = sizeof(float);
    }

    size_t memSize = (typeSize * count);

    void* buf = new char[memSize];
    void* tmp = new char[memSize];

    int counter = 0;
    if (type == MPI_INT) {
      for (int i = 0; i < size; ++i) {
        if (i != root) {
          MPI_Recv(buf, count, type, MPI_ANY_SOURCE, 1, comm,
                   MPI_STATUS_IGNORE);
        } else {
          for (int j = 0; j < count; ++j) {
            (reinterpret_cast<int*>(buf))[j] =
                (reinterpret_cast<int*>(sendbuf))[j];
          }
        }
        if (counter == 0) {
          for (int j = 0; j < count; ++j) {
            (reinterpret_cast<int*>(tmp))[j] = (reinterpret_cast<int*>(buf))[j];
          }
          counter++;
        } else {
          if (op == MPI_PROD) {
            prodInt(buf, tmp, count);
          } else if (op == MPI_SUM) {
            sumInt(buf, tmp, count);
          } else if (op == MPI_MIN) {
            minInt(buf, tmp, count);
          } else if (op == MPI_MAX) {
            maxInt(buf, tmp, count);
          }
        }
      }
      for (int i = 0; i < count; ++i) {
        (reinterpret_cast<int*>(recvbuf))[i] = (reinterpret_cast<int*>(tmp))[i];
      }
      delete[] reinterpret_cast<int*>(buf);
      delete[] reinterpret_cast<int*>(tmp);
      buf = nullptr;
      tmp = nullptr;
    } else if (type == MPI_FLOAT) {
      for (int i = 0; i < size; ++i) {
        if (i != root) {
          MPI_Recv(buf, count, type, MPI_ANY_SOURCE, 1, comm,
                   MPI_STATUS_IGNORE);
        } else {
          for (int j = 0; j < count; ++j) {
            (reinterpret_cast<float*>(buf))[j] =
                (reinterpret_cast<float*>(sendbuf))[j];
          }
        }
        if (counter == 0) {
          for (int j = 0; j < count; ++j) {
            (reinterpret_cast<float*>(tmp))[j] =
                (reinterpret_cast<float*>(buf))[j];
          }
          counter++;
        } else {
          if (op == MPI_PROD) {
            prodFloat(buf, tmp, count);
          } else if (op == MPI_SUM) {
            sumFloat(buf, tmp, count);
          } else if (op == MPI_MIN) {
            minFloat(buf, tmp, count);
          } else if (op == MPI_MAX) {
            maxFloat(buf, tmp, count);
          }
        }
      }
      for (int i = 0; i < count; ++i) {
        (reinterpret_cast<float*>(recvbuf))[i] =
            (reinterpret_cast<float*>(tmp))[i];
      }
      delete[] reinterpret_cast<float*>(buf);
      delete[] reinterpret_cast<float*>(tmp);
      buf = nullptr;
      tmp = nullptr;
    } else {
      for (int i = 0; i < size; ++i) {
        if (i != root) {
          MPI_Recv(buf, count, type, MPI_ANY_SOURCE, 1, comm,
                   MPI_STATUS_IGNORE);
        } else {
          for (int j = 0; j < count; ++j) {
            (reinterpret_cast<double*>(buf))[j] =
                (reinterpret_cast<double*>(sendbuf))[j];
          }
        }
        if (counter == 0) {
          for (int j = 0; j < count; ++j) {
            (reinterpret_cast<double*>(tmp))[j] =
                (reinterpret_cast<double*>(buf))[j];
          }
          counter++;
        } else {
          if (op == MPI_PROD) {
            prodDouble(buf, tmp, count);
          } else if (op == MPI_SUM) {
            sumDouble(buf, tmp, count);
          } else if (op == MPI_MIN) {
            minDouble(buf, tmp, count);
          } else if (op == MPI_MAX) {
            maxDouble(buf, tmp, count);
          }
        }
      }
      for (int i = 0; i < count; ++i) {
        (reinterpret_cast<double*>(recvbuf))[i] =
            (reinterpret_cast<double*>(tmp))[i];
      }
      delete[] reinterpret_cast<double*>(buf);
      delete[] reinterpret_cast<double*>(tmp);
      buf = nullptr;
      tmp = nullptr;
    }
  } else {
    MPI_Send(sendbuf, count, type, root, 1, comm);
  }
  return 0;
}

void sumInt(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    (reinterpret_cast<float*>(tmp))[j] += (reinterpret_cast<float*>(buf))[j];
  }
}

void maxInt(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    if ((reinterpret_cast<int*>(buf))[j] >= (reinterpret_cast<int*>(tmp))[j]) {
      (reinterpret_cast<int*>(tmp))[j] = (reinterpret_cast<int*>(buf))[j];
    }
  }
}

void minInt(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    if ((reinterpret_cast<double*>(buf))[j] <=
        (reinterpret_cast<double*>(tmp))[j]) {
      (reinterpret_cast<double*>(tmp))[j] = (reinterpret_cast<double*>(buf))[j];
    }
  }
}

void prodInt(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    (reinterpret_cast<int*>(tmp))[j] *= (reinterpret_cast<int*>(buf))[j];
  }
}

void sumFloat(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    (reinterpret_cast<float*>(tmp))[j] += (reinterpret_cast<float*>(buf))[j];
  }
}

void maxFloat(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    if ((reinterpret_cast<float*>(buf))[j] >=
        (reinterpret_cast<float*>(tmp))[j]) {
      (reinterpret_cast<float*>(tmp))[j] = (reinterpret_cast<float*>(buf))[j];
    }
  }
}

void minFloat(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    if ((reinterpret_cast<float*>(buf))[j] <=
        (reinterpret_cast<float*>(tmp))[j]) {
      (reinterpret_cast<float*>(tmp))[j] = (reinterpret_cast<float*>(buf))[j];
    }
  }
}

void prodFloat(void* buf, void* tmp, const int count) {
  for (int i = 0; i < count; ++i) {
    (reinterpret_cast<float*>(tmp))[i] *= (reinterpret_cast<float*>(buf))[i];
  }
}

void sumDouble(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    (reinterpret_cast<double*>(tmp))[j] += (reinterpret_cast<double*>(buf))[j];
  }
}

void maxDouble(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    if ((reinterpret_cast<double*>(buf))[j] >=
        (reinterpret_cast<double*>(tmp))[j]) {
      (reinterpret_cast<double*>(tmp))[j] = (reinterpret_cast<double*>(buf))[j];
    }
  }
}

void minDouble(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    if ((reinterpret_cast<double*>(buf))[j] <=
        (reinterpret_cast<double*>(tmp))[j]) {
      (reinterpret_cast<double*>(tmp))[j] = (reinterpret_cast<double*>(buf))[j];
    }
  }
}

void prodDouble(void* buf, void* tmp, const int count) {
  for (int j = 0; j < count; ++j) {
    (reinterpret_cast<double*>(tmp))[j] *= (reinterpret_cast<double*>(buf))[j];
  }
}

template int* getRandomVector(size_t size);

template double* getRandomVector(size_t size);

template float* getRandomVector(size_t size);
