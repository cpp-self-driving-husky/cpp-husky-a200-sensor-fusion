#ifndef MATRIX_H_
#define MATRIX_H_
#include <cmath>
#include "state_vector.h"


// TODO remove inclusion
//      test for printing
#include<iostream>


namespace mtx {

    template<class T>
    class Matrix {

        public:
            Matrix(int rows, int cols) :
                matrix_(nullptr), rows_(rows), cols_(cols)
            {
                this->init(rows,cols);
            }

            ~Matrix() {
                this->destroy();
            }

            void init(int rows, int cols) {
                this->destroy();
                this->rows_ = rows;
                this->cols_ = cols;
                int elem = this->rows_ * this->cols_;
                this->matrix_ = new T[elem];
                for (int i = 0; i < elem; ++i)
                    this->matrix_[i] = 0.0;
            }

            void replicate(Matrix& matrix) {
                int elems = this->rows_*this->cols_;
                for (int i = 0; i < elems; ++i)
                    matrix[i] = this->matrix_[i];
            }

            void zero() {
                int elems = this->rows_*this->cols_;
                for (int i = 0; i < elems; ++i)
                    this->matrix_[i] = 0.0;
            }

            void destroy() {
                if (this->matrix_ != nullptr) {
                    delete[] this->matrix_;
                    this->matrix_ = nullptr;
                }
                rows_ = cols_ = 0;
            }

            T& elem(int row, int col) {
                return this->matrix_[row*this->cols_+col];
            }

            T& operator[](int i) {
                return this->matrix_[i];
            }

            // TODO test this!
            void operator+=(mtx::Matrix<T>& matrix) {
                int elems = this->rows_*this->cols_;
                for (int i = 0; i < elems; ++i)
                    this->matrix_[i] += matrix.matrix_[i];
            }

            void stateCol(
                state::StateVector<T>& state,
                int col)
            {
                for (int i = 0; i < this->rows_; ++i)
                    state[i] = this->matrix_[i*this->rows_+col];
            }

            void stateRow(
                state::StateVector<T>& state,
                int row)
            {
                int init_elem = this->cols_*row;
                for (int i = 0; i < this->cols_; ++i)
                    state[i] = this->matrix_[i+init_elem];
            }

            void addVectorMatrixRow(
                state::StateVector<T>& state,
                int init_row)
            {
                int init_elem = init_row * this->cols_;
                for (int i = 0; i < this->cols_; ++i)
                    state[i] += this->matrix_[init_elem+i];
            }

            void subVectorMatrixRow(
                state::StateVector<T>& state,
                int init_row)
            {
                int init_elem = init_row * this->cols_;
                for (int i = 0; i < this->cols_; ++i)
                    state[i] -= this->matrix_[init_elem+i];
            }

            void addData(T* data, int meas) {
                int orig_size = this->cols_*this->rows_;
                int new_size = this->cols_*(this->rows_+meas);
                T* temp = new T[new_size];
                for (int i = 0; i < orig_size; ++i)
                    temp[i] = this->matrix_[i];
                for (int i = orig_size; i < new_size; ++i)
                    temp[i] = data[i-orig_size];
                if (this->matrix_ != nullptr)
                    delete[] this->matrix_;
                this->matrix_ = temp;
                this->rows_ += meas;
            }

            void removeData(int meas) {
                int orig_size = this->cols_*this->rows_;
                int new_size = this->cols_*(this->rows_-meas);
                int index = orig_size - new_size;
                T* temp = new T[new_size];
                for (int i = index; i < orig_size; ++i)
                    temp[i-index] = this->matrix_[i];
                if (this->matrix_ != nullptr)
                    delete[] this->matrix_;
                this->matrix_ = temp;
                this->rows_ -= meas;
            }

            void operator*=(T scalar) {
                for (int r = 0; r < this->rows_; ++r)
                    for (int c = 0; c < this->cols_; ++c)
                        this->matrix_[r*this->cols_+c] *= scalar;
            }

            void mean(state::StateVector<T>& state) {
                int vars = state.getVars();
                for (int v = 0; v < vars; ++v) {
                    state[v] = 0.0;
                    for (int m = 0; m < this->rows_; ++m)
                        state[v] += this->matrix_[vars*m+v];
                    state[v] /= this->rows_;
                }
            }

            void transpose(Matrix<T>& U) {
                for (int i = 0; i < this->rows_; ++i)
                    for (int j = 0; j < this->cols_; ++j)
                        U[j*this->rows_+i] = this->matrix_[i*this->cols_+j];
                U.setRows(this->cols_);
                U.setCols(this->rows_);
            }

            void swapDimensions() {
                int sub = this->rows_;
                this->rows_ = this->cols_;
                this->cols_ = sub;
            }

            bool LUPdecompose(state::StateVector<int>& P) {
                int len = this->rows_, kd = 0, kswap;
                T p, t;
                for (int i = 0; i < len; ++i)
                    P[i] = i;
                for (int k = 0; k < len-1; ++k) {
                    p = 0.0;
                    for (int i = k; i < len; ++i) {
                        t = this->matrix_[i*len+k];
                        if (t < 0)
                            t *= -1;
                        if (t > p) {
                            p = t;
                            kd = i;
                        }
                    }
                    if (p == 0.0)
                        return false;
                    kswap = P[kd];
                    P[kd] = P[k];
                    P[k] = kswap;
                    for (int i = 0; i < len; ++i) {
                        t = this->matrix_[kd*len+i];
                        this->matrix_[kd*len+i] = this->matrix_[k*len+i];
                        this->matrix_[k*len+i] = t;
                    }
                    for (int i = k+1; i < len; ++i) {
                        this->matrix_[i*len+k] =
                            this->matrix_[i*len+k] / this->matrix_[k*len+k];
                        for (int j = k+1; j < len; ++j)
                            this->matrix_[i*len+j] -=
                                this->matrix_[i*len+k]*this->matrix_[k*len+j];
                    }
                }
                return true;
            }

            bool LUPinverse(
                mtx::Matrix<T>& B,
                state::StateVector<int>& P,
                state::StateVector<T>& X,
                state::StateVector<T>& Y)
            {
                int len = this->rows_;
                for (int i = 0; i < len; ++i) {
                    for (int j = 0; j < len; ++j)
                        B[i*len+j] = 0.0;
                    B[i*len+i] = 1.0;
                    for (int n = 0; n < len; ++n) {
                        T t = 0.0;
                        for (int m = 0; m <= n-1; ++m )
                            t += this->matrix_[n*len+m]*Y[m];
                        Y[n] = B[i*len+P[n]]-t;
                    }
                    for (int n = len-1; n>=0; --n) {
                        T t = 0.0;
                        for (int m = n+1; m < len; ++m)
                            t += this->matrix_[n*len+m]*X[m];
                        X[n] = (Y[n]-t) / this->matrix_[n*len+n];
                    }
                    for (int j = 0; j < len; ++j)
                        B[i*len+j] = X[j];

                }
                for (int i = 0; i < len; ++i)
                    for (int j = 0; j < len; ++j)
                        this->matrix_[i*len+j] = B[j*len+i];
                return true;
            }

            void covariance(
                Matrix<T>& covariance_matrix,
                state::StateVector<T>& mean_vector)
            {
                int meas = this->rows_;
                int vars = this->cols_;
                int dimension = covariance_matrix.getRows();
                for (int i = 0; i < dimension; ++i) {
                    for (int j = i; j < dimension; ++j) {
                        T y_mean = mean_vector[i];
                        T x_mean = mean_vector[j];
                        T variance = 0.0;
                        for (int k = 0; k < meas; ++k)
                            variance +=
                                (this->matrix_[k*vars+j]-x_mean) *
                                (this->matrix_[k*vars+i]-y_mean);
                        variance /= (meas-1);
                        covariance_matrix[i*dimension+j] = variance;
                        covariance_matrix[j*dimension+i] = variance;
                    }
                }
            }

            // TODO remove this function
            //      is in Calculator class now
            void cholesky(Matrix<T>& L) {
                int n = this->rows_;
                int elems = L.getRows()*L.getCols();
                for (int i = 0; i < elems; ++i)
                    L[i] = 0.0;
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < (i+1); ++j) {
                        T s = 0.0;
                        for (int k = 0; k < j; k++)
                            s += L[i*n+k] * L[j*n+k];
                        L[i*n+j] = (i == j) ?
                            std::sqrt(this->matrix_[i*n+i] - s) :
                            (1.0 / L[j*n+j] * (this->matrix_[i*n+j] - s));
                    }
                }
            }

            void setRows(int rows) {
                this->rows_ = rows;
            }

            void setCols(int cols) {
                this->cols_ = cols;
            }

            int getRows() {
                return this->rows_;
            }

            int getCols() {
                return this->cols_;
            }

            int getSize() {
                return this->rows_ * this->cols_;
            }

            // test function
            void print() {
                for (int r = 0; r < this->rows_; ++r) {
                    for (int c = 0; c < this->cols_; ++c) {
                        std::cout << this->matrix_[r*this->cols_+c] << " ";
                    }
                    std::cout << "\n";
                }
                std::cout <<
                    "rows: " << this->rows_ << ", " <<
                    "cols: " << this->cols_ << "\n";
                std::cout << std::endl;
            }


        private:
            T* matrix_;
            int rows_;
            int cols_;

    };

    // a data matrix places observations as rows,
    // and each variable is in columns
    template<class T>
    using DataMatrix = Matrix<T>;

    template<class T>
    using CovarianceMatrix = Matrix<T>;


    namespace {
        const int DEFAULT_STORAGE = 100;
    };


    // matrix calculator class
    template<class T>
    class Calculator {

        public:
            Calculator() :
                storage_(mtx::Matrix<T>(1,DEFAULT_STORAGE))
            {}

            Calculator(int elems) :
                storage_(mtx::Matrix<T>(1,this->storageSize(elems)))
            {}

            ~Calculator() {
                this->destroy();
            }

            void init(int elems) {
                this->destroy();
                this->storage_ = Matrix<T>(1,this->storageSize(elems));
            }

            int storageSize(int elems) {
                int storage_size = static_cast<int>(std::pow(elems,3));
                return
                    storage_size > DEFAULT_STORAGE ?
                    storage_size : DEFAULT_STORAGE;
            }

            void destroy() {
                this->storage_.destroy();
            }

            void duplicate(Matrix<T>& matrix) {
                int elems = matrix.getSize();
                for (int i = 0; i < elems; ++i)
                    this->storage_[i] = matrix[i];
            }

            void zero(int len) {
                for (int i = 0; i < len; ++i)
                    this->storage_[i] = 0.0;
            }

            void transpose(Matrix<T>& trans) {
                this->duplicate(trans);
                this->transpose(trans,this->storage_);
            }

            void transpose(mtx::Matrix<T>& trans, mtx::Matrix<T>& matrix) {
                int rows = trans.getRows(),
                    cols = trans.getCols();
                for (int i = 0; i < rows; ++i)
                    for (int j = 0; j < cols; ++j)
                        trans[j*rows+i] = matrix[i*cols+j];
                trans.swapDimensions();
            }

            void multiply(Matrix<T>& P, Matrix<T>& A, Matrix<T>& B) {
                P.zero();
                int rows = A.getRows(),
                    inner = A.getCols(),
                    cols = B.getCols();
                for (int i = 0; i < rows; ++i)
                    for (int j = 0; j < cols; ++j)
                        for (int k = 0; k < inner; ++k)
                            P[i*cols+j] += A[i*inner+k] * B[k*cols+j];
            }

            void cholesky(Matrix<T>& L) {
                this->duplicate(L);
                this->cholesky(L,this->storage_);
            }

            void cholesky(Matrix<T>& L, Matrix<T>& A) {
                L.zero();
                int elems = L.getSize(),
                    n = L.getRows();
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < (i+1); ++j) {
                        T s = 0.0;
                        for (int k = 0; k < j; ++k)
                            s += L[i*n+k] * L[j*n+k];
                        L[i*n+j] = (i == j) ?
                            std::sqrt(A[i*n+i] - s) :
                            (1.0 / L[j*n+j] * (A[i*n+j] - s));
                    }
                }
            }





            int getStorageSize() {
                return this->storage_.getSize();
            }


        private:
            Matrix<T> storage_;

    };

}

#endif

