// Copyright 2021 Ivanov Arkadiy
#ifndef MODULES_TASK_2_IVANOV_A_MATRIX_MULTIPLICATION_MATRIX_MULTIPLICATION_H_
#define MODULES_TASK_2_IVANOV_A_MATRIX_MULTIPLICATION_MATRIX_MULTIPLICATION_H_


#include <mpi.h>
#include <iostream>
#include <cstdint>

#define UINT64_T uint64_t
#define MIE "mem init error"

template<class T>
class matrix {
 private:
    T* m;
    int numRows, numColums;

 public:
    matrix();
    matrix(const matrix<T>& c);
    matrix(const int _numRows, const int _numColums);
    ~matrix();

    T* operator[](const int i);
    const T* operator[](const int i) const;
    matrix<T> operator*(const matrix<T>& c);
    matrix<T> operator*(const matrix<T>& c) const;
    matrix<T>& operator=(const matrix<T>& c);
    bool operator==(const matrix<T>& c);
    template<class T2> friend std::ostream& operator<< (std::ostream& o, const matrix<T2>& c);

    void fillMatrix(int (*generateFunciton)(int, int, int*),
        int value1, int value2, int baseFeedbackValue);
    int getNumRows() const { return numRows; }
    int getNumColums() const { return numColums; }
    T* data() { return m; }
    /*const*/ T* data() const { return /*reinterpret_cast<const T*>(*/m/*)*/; }
    void prepareSpace(int _numRows, int _numColums);
};

template<class T>
matrix<T>::matrix() {
    m = nullptr;
    numRows = numColums = 0;
}

template<class T>
matrix<T>::matrix(const matrix<T>& c) {
    numRows = c.numRows;
    numColums = c.numColums;
    if (numRows * numColums == 0) {
        m = nullptr;
    } else {
        m = new T[static_cast<UINT64_T>(numRows) * numColums];  // C26451
        if (m == nullptr)
            throw MIE;
        for (int i = 0; i < numRows * numColums; i++)
            m[i] = c.m[i];
    }
}

template<class T>
matrix<T>::matrix(const int _numRows, const int _numColumns) {
    if (_numColumns == 0 || _numRows == 0)
        throw "Can't create matrix with 0 column numbers or rows";
    numRows = _numRows;
    numColums = _numColumns;
    m = new T[static_cast<UINT64_T>(numRows) * numColums];  // C26451
    if (m == nullptr)
        throw MIE;
    for (int i = 0; i < numRows * numColums; m[i++] = 0) {}
}

template<class T>
matrix<T>::~matrix() {
    numRows = numColums = 0;
    if (m != nullptr) {
        delete[] m;
        m = nullptr;
    }
}

template<class T>
T* matrix<T>::operator[](const int i) {
    if (i >= numRows)
        throw "Unacceptable row number";
    return m + static_cast<UINT64_T>(i) * numColums;  // C26451
}

template<class T>
const T* matrix<T>::operator[](const int i) const {
    if (i >= numRows)
        throw "Unacceptable row number";
    return reinterpret_cast<const T*>(m + static_cast<UINT64_T>(i) * numColums);  // C26451
}

template<class T>
matrix<T> matrix<T>::operator*(const matrix<T>& c) {
    if (numColums != c.numRows)
        throw "Impossible to multiply";
    matrix<T> res(numRows, c.numColums);
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < c.numColums; j++) {
            res[i][j] = 0;
            for (int k = 0; k < numColums; k++) {
                res[i][j] += (*this)[i][k] * c[k][j];
            }
        }
    }
    return res;
}

template<class T>
matrix<T> matrix<T>::operator*(const matrix<T>& c) const {
    if (numColums != c.numRows)
        throw "Impossible to multiply";
    matrix<T> res(numRows, c.numColums);
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < c.numColums; j++) {
            res[i][j] = 0;
            for (int k = 0; k < numColums; k++) {
                res[i][j] += (*this)[i][k] * c[k][j];
            }
        }
    }
    return res;
}


template<class T>
matrix<T>& matrix<T>::operator=(const matrix<T>& c) {
    if (this == &c)
        return *this;
    if (m != nullptr)
        delete[] m;
    numRows = c.numRows;
    numColums = c.numColums;
    m = new T[static_cast<UINT64_T>(numRows) * numColums];  // C26451
    if (m == nullptr)
        throw MIE;
    for (int i = 0; i < numRows * numColums; i++)
        m[i] = c.m[i];
    return *this;
}

template<class T>
bool matrix<T>::operator==(const matrix<T>& c) {
    if (numRows != c.numRows || numColums != c.numColums)
        return false;
    for (int i = 0; i < numRows * numColums; i++)
        if (m[i] != c.m[i])
            return false;
    return true;
}

template<class T>
std::ostream& operator<< (std::ostream& o, const matrix<T>& c) {
    for (int i = 0; i < c.getNumRows(); i++) {
        for (int j = 0; j < c.getNumColums(); j++) {
            o << c[i][j] << '\t';
        }
        o << std::endl;
    }
    return o;
}

template<class T>
void matrix<T>::fillMatrix(int (*generateFunciton)(int, int, int*), int value1, int value2, int baseFeedbackValue) {
    int feedback = baseFeedbackValue;
    for (int i = 0; i < numRows * numColums; i++)
        m[i] = static_cast<T>(generateFunciton(value1, value2, &feedback));
}

template<class T>
void matrix<T>::prepareSpace(int _numRows, int _numColums) {
    if (m != nullptr)
        delete[] m;
    m = nullptr;
    numRows = _numRows;
    numColums = _numColums;
    m = new T[static_cast<UINT64_T>(numRows) * numColums];  // C26451
    if (m == nullptr)
        throw MIE;
}

// can generate in range [-maxValue, -minValue] && [minValue, maxValue] IN: minValue, maxValue > 0
int generateRngValue(int minValue, int maxValue, int* feedback);

// generates: {minValue + feedback, minValue + feedback + 1, minValue + feedback + 2, ...}
int generateRisingValue(int minValue, int empty, int* feedback);

// generates: {maxValue - feedback, maxValue - feedback - 1, maxValue - feedback - 2, ...}
int generateFallingValue(int empty, int maxValue, int* feedback);

// generates: v1, v2, v1, v2, v1, v2, v1, v2, ... with rule feedback % 2 == 0 -> v1 else v2
int generateJumpingValue(int v1, int v2, int* feedback);

// on root proc A,B != nullptr |||| on !root A,B can be nullptr
template<class T>
matrix<T> parallelMultiplication(const matrix<T>* A, const matrix<T>* B, MPI_Datatype matrixDatatype) {
    // get MPI info
    int procRank = 0, procCount = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    matrix<T> ans;

    if (procCount == 1)
        return (*A) * (*B);

    // Proc with rank 0 sends info about matrixes sizes
    int matrixSizes[4] = { 0, 0, 0, 0 };
    if (procRank == 0) {
        if (A->getNumColums() != B->getNumRows())
            throw "Wrong matrixes sizes";  // all processes should shut down
        matrixSizes[0] = A->getNumRows();
        matrixSizes[1] = A->getNumColums();
        matrixSizes[2] = B->getNumRows();
        matrixSizes[3] = B->getNumColums();
    }
    MPI_Bcast(reinterpret_cast<void*>(matrixSizes), 4, MPI_INT, 0, MPI_COMM_WORLD);

    if (matrixSizes[0] == 1) {
        if (procRank == 0)
            return (*A) * (*B);
        else
            return ans;
    }

    // creating new communicator
    int color;
    if (matrixSizes[0] / procCount >= 1) {
        color = 0;
    } else {
        if (procRank >= matrixSizes[0])
            color = 1;
        else
            color = 0;
    }
    MPI_Comm NEW_COMM_WORLD;
    MPI_Comm_split(MPI_COMM_WORLD, color, procRank, &NEW_COMM_WORLD);
    if (color == 1) {
        MPI_Comm_free(&NEW_COMM_WORLD);
        return ans;
    }
    MPI_Comm_size(NEW_COMM_WORLD, &procCount);

    matrix<T> a, b;
    if (procRank) {
        b.prepareSpace(matrixSizes[2], matrixSizes[3]);
    }
    int linesToProcess = matrixSizes[0] / procCount;  // [0,0,0,0] | [1,1,1,1]
    if (procRank < matrixSizes[0] % procCount)
        linesToProcess++;  // [0,0,0,0] -> [1,1,1,0] | [1,1,1,1] -> [2,2,1,1]
    // now all processes now how many lines they have to process
    if (linesToProcess) {  // if there is lines to process
        a.prepareSpace(linesToProcess, matrixSizes[1]);  // preparing space
    }

    // now root process have to compute, whom to send data. SCATTERING
    int* sendcounts = nullptr, * displs = nullptr;
    if (procRank == 0) {
        sendcounts = new int[procCount];
        if (sendcounts == nullptr)
            throw MIE;
        displs = new int[procCount];
        if (displs == nullptr) {
            delete[] sendcounts;
            throw MIE;
        }
        // scnt[?,?,?,?] -> [0,0,0,0] | [?,?,?,?] -> [1,1,1,1]
        std::fill_n(sendcounts, procCount, matrixSizes[0] / procCount);
        // [0,0,0,0] -> [1,1,1,0] | [1,1,1,1] -> [2,2,1,1]
        for (int i = 0; i < matrixSizes[0] % procCount; ++sendcounts[i++]) {}
        // converting line counts to data counts
        // [1,1,1,0] -> [5,5,5,0] | [2,2,1,1] -> [10,10,5,5]
        for (int i = 0; i < procCount; i++)
            sendcounts[i] *= matrixSizes[1];
        // counting displacement
        std::fill_n(displs, procCount, 0);  // displs[0] = 0;
        // [0,0,0,0] -> [0,5,10,0] | [0,0,0,0] -> [0,10,20,25]
        for (int i = 1; i < procCount; i++)
            if (sendcounts[i])
                displs[i] = displs[i - 1] + sendcounts[i - 1];
        // 3l 5c 4p scnt[5,5,5,0] dsplc[0,5,10,0] | 6l 5c 4p s[10,10,5,5] d[0,10,20,25]
    }

    // sending scattered matrix A on prepared matrix a
    if (procRank == 0) {
        MPI_Scatterv(reinterpret_cast<const void*>(A->data()), sendcounts, displs,
            matrixDatatype, a.data(), linesToProcess * matrixSizes[1], matrixDatatype, 0, NEW_COMM_WORLD);
    } else {
        MPI_Scatterv(nullptr, nullptr, nullptr, matrixDatatype, a.data(), linesToProcess * matrixSizes[1],
            matrixDatatype, 0, NEW_COMM_WORLD);
    }

    // sending matrix B
    if (procRank == 0)   // for root process
        MPI_Bcast(reinterpret_cast<void*>(B->data()),
            matrixSizes[2] * matrixSizes[3], matrixDatatype, 0, NEW_COMM_WORLD);
    else  // for !root process
        MPI_Bcast(reinterpret_cast<void*>(b.data()),
            matrixSizes[2] * matrixSizes[3], matrixDatatype, 0, NEW_COMM_WORLD);

    // Now root process have part of A matrix in matrix a and matrix B to operate (b empty)
    // !root processes have their own parts of A (but not everyone) in matrix a and matrix B in b
    // this matrix will be result of a*B (for root) and a*b (for !root) and will be empty for linesToProcess == 0
    matrix<T> c;
    if (procRank == 0) {
        c = a * (*B);
    } else {
        if (linesToProcess) {
            c = a * b;
        }
    }

    // now the c matrixes needs to be merged
    if (procRank == 0) {
        ans.prepareSpace(matrixSizes[0], matrixSizes[3]);  // preparing memory to receive final matrix
        for (int i = 0; i < procCount; i++) {
            sendcounts[i] /= matrixSizes[1];  // [5,5,5,0] -> [1,1,1,0] | [10,10,5,5] -> [2,2,1,1]
            // 5 lines 2 colums | 5 lines 2 colums <= matrix B (for example)
            sendcounts[i] *= matrixSizes[3];  // [1,1,1,0] -> [2,2,2,0] | [2,2,1,1] -> [4,4,2,2]
        }
        std::fill_n(displs, procCount, 0);
        for (int i = 1; i < procCount; i++)  // (POTENTIAL ERROR FOR 3l 5c 4p dspls[3]=6???) fixed
            if (sendcounts[i])  // [0,5,10,15] -> [0,2,4,6] | [0,10,20,25] -> [0,4,8,10]
                displs[i] = displs[i - 1] + sendcounts[i - 1];
        MPI_Gatherv(reinterpret_cast<const void*>(c.data()), linesToProcess * matrixSizes[3], matrixDatatype,
            reinterpret_cast<void*>(ans.data()), sendcounts, displs, matrixDatatype, 0, NEW_COMM_WORLD);
    } else {
        MPI_Gatherv(reinterpret_cast<const void*>(c.data()), linesToProcess * matrixSizes[3], matrixDatatype,
            nullptr, nullptr, nullptr, matrixDatatype, 0, NEW_COMM_WORLD);
    }
    // now on root we have final matrix in ans
    if (procRank == 0) {
        delete[] sendcounts;
        delete[] displs;
    }
    MPI_Comm_free(&NEW_COMM_WORLD);
    return ans;
}

#endif  // MODULES_TASK_2_IVANOV_A_MATRIX_MULTIPLICATION_MATRIX_MULTIPLICATION_H_
