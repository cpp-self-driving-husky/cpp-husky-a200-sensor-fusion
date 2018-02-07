#ifndef MATRIX_H_
#define MATRIX_H_
#include <cmath>
#include "state_vector.h"

// test for printing
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



            /*

            void getCofactor(T* A, T* temp, int p, int q, int n) {
                int i = 0, j = 0;
                for (int row = 0; row < n; row++) {
                    for (int col = 0; col < n; col++) {
                        if (row != p && col != q) {
                            temp[i*this->cols_+j] = A[row*this->cols_+col];
                            ++j;
                            if (j == n-1) {
                                j = 0;
                                i++;
                            }
                        }
                    }
                }
            }

            // TODO This needs to be made more efficient!
            //      allocating memory like this is slow!
            int determinant(T* A, int n) {
                int D = 0;
                if (n == 1)
                    return A[0];
                T* temp = new T[n*n];
                int sign = 1;
                for (int f = 0; f < n; f++) {
                    this->getCofactor(A,temp, 0, f, n);
                    D += sign * A[f] * determinant(temp, n-1);
                    sign = -sign;
                }
                delete[] temp;
                return D;
            }

            void adjoint(T* A, T* adj, int N) {
                if (N == 0) {
                    adj[0] = 1;
                    return;
                }
                int sign = 1;
                T* temp = new T[N*N];
                for (int i = 0; i < N; ++i) {
                    for (int j = 0; j < N; ++j) {
                        this->getCofactor(A,temp,i,j,N);
                        sign = ((i+j) % 2 == 0) ? 1 : -1;
                        adj[j*N+i] = sign*this->determinant(temp,N-1);
                    }
                }
            }

            bool invert(Matrix<T>& inverse) {
                int len = this->rows_*this->cols_;
                T* A = new T[len];
                for (int i = 0; i < len; ++i)
                    A[i] = this->matrix_[i];
                T det = this->determinant(A,this->rows_);
                if (det == 0)
                    return false;
                T* adj = new T[len];
                this->adjoint(A,adj,this->rows_);
                for (int i = 0; i < len; ++i)
                    inverse[i] = adj[i] / det;
                delete[] A;
                delete[] adj;
                return true;
            }

            */


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

}


#endif

