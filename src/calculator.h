#ifndef CALCULATOR_H_
#define CALCULATOR_H_
#include "matrix.h"
#include <cmath>


// TODO remove inclusion
//      test for printing
#include<iostream>



namespace {
    const int DEFAULT_STORAGE = 100;
};


namespace calc {

    template<class T>
    class Calculator {

        public:
            Calculator() :
                square_(mtx::Matrix<T>(DEFAULT_STORAGE,DEFAULT_STORAGE)),
                state_x_(nullptr),
                state_y_(nullptr),
                indeces_(nullptr),
                vars_(0),
                storage_(nullptr)
            {
                this->init(DEFAULT_STORAGE);
            }

            Calculator(int elems) :
                square_(mtx::Matrix<T>(elems,elems)),
                state_x_(nullptr),
                state_y_(nullptr),
                indeces_(nullptr),
                vars_(0),
                storage_(nullptr)
            {
                this->init(elems);
            }

            ~Calculator() {
                this->destroy();
            }

            void init(int elems) {
                this->destroy();
                this->vars_ = elems;
                this->square_.init(elems,elems);
                this->state_x_ = new T[this->vars_];
                this->state_y_ = new T[this->vars_];
                this->indeces_ = new int[this->vars_];
                this->storage_ = new T[this->storageSize(this->vars_)];
            }

            int storageSize(int elems) {
                int storage_size = static_cast<int>(std::pow(elems,3));
                return
                    storage_size > DEFAULT_STORAGE ?
                    storage_size : DEFAULT_STORAGE;
            }

            void destroy() {
                this->square_.destroy();
                this->deallocate(this->state_x_);
                this->deallocate(this->state_y_);
                this->deallocate(this->indeces_);
                this->deallocate(this->storage_);
                this->vars_ = 0;
            }

            template<typename S>
            void deallocate(S* elements) {
                if (elements != nullptr) {
                    delete[] elements;
                    elements = nullptr;
                }
            }

            void store(mtx::Matrix<T>& matrix) {
                int elems = matrix.getSize();
                for (int i = 0; i < elems; ++i)
                    this->square_[i] = matrix[i];
            }

            template<typename S>
            void zero(S* elements, int len) {
                for (int i = 0; i < len; ++i)
                    elements[i] = 0.0;
            }

            template<typename S>
            void swaps(S& a, S& b) {
                S s = a;
                a = b;
                b = s;
            }

            void mean(state::StateVector<T>& state, mtx::Matrix<T>& data) {
                int vars = state.getVars();
                for (int v = 0; v < vars; ++v) {
                    state[v] = 0.0;
                    for (int m = 0; m < data.getRows(); ++m)
                        state[v] += data[vars*m+v];
                    state[v] /= data.getRows();
                }
            }

            void covariance(
                mtx::Matrix<T>& cov,
                mtx::Matrix<T>& data,
                state::StateVector<T>& mean)
            {
                int meas = data.getRows(),
                    vars = data.getCols(),
                    dim = cov.getRows();
                for (int i = 0; i < dim; ++i) {
                    for (int j = i; j < dim; ++j) {
                        T variance = 0.0;
                        for (int k = 0; k < meas; ++k)
                            variance +=
                                (data[k*vars+j]-mean[i]) *
                                (data[k*vars+i]-mean[j]);
                        variance /= (meas-1);
                        cov[i*dim+j] = variance;
                        cov[j*dim+i] = variance;
                    }
                }
            }

            void transpose(mtx::Matrix<T>& trans) {
                this->store(trans);
                this->transpose(trans,this->square_);
            }

            void transpose(mtx::Matrix<T>& trans, mtx::Matrix<T>& matrix) {
                int rows = trans.getRows(),
                    cols = trans.getCols();
                for (int i = 0; i < rows; ++i)
                    for (int j = 0; j < cols; ++j)
                        trans[j*rows+i] = matrix[i*cols+j];
                trans.swapDimensions();
            }

            void subtract(mtx::Matrix<T>& P, mtx::Matrix<T>& A, mtx::Matrix<T>& B) {
                int len = P.getSize();
                for (int i = 0; i < len; ++i)
                    P[i] = A[i] - B[i];
            }

            void multiply(mtx::Matrix<T>& P,mtx::Matrix<T>& A,mtx::Matrix<T>& B) {
                P.zero();
                int rows = A.getRows(),
                    inner = A.getCols(),
                    cols = B.getCols();
                for (int i = 0; i < rows; ++i)
                    for (int j = 0; j < cols; ++j)
                        for (int k = 0; k < inner; ++k)
                            P[i*cols+j] += A[i*inner+k] * B[k*cols+j];
            }

            void multiplyABt(mtx::Matrix<T>& P,mtx::Matrix<T>& A,mtx::Matrix<T>& B) {
                P.zero();
                int rows = A.getRows(),
                    inner = A.getCols(),
                    cols = B.getCols();
                for (int i = 0; i < rows; ++i)
                    for (int j = 0; j < cols; ++j)
                        for (int k = 0; k < inner; ++k)
                            P[i*cols+j] += A[i*inner+k] * B[j*cols+k];
            }

            void multiplyABAt(mtx::Matrix<T>& P,mtx::Matrix<T>& A,mtx::Matrix<T>& B) {
                this->multiply(this->square_,A,B);
                this->multiplyABt(P,this->square_,A);
            }

            void cholesky(mtx::Matrix<T>& L) {
                this->store(L);
                this->cholesky(L,this->square_);
            }

            void cholesky(mtx::Matrix<T>& L, mtx::Matrix<T>& A) {
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

            bool LUPdecompose(mtx::Matrix<T>& L){
                int len = L.getRows(), kd = 0;
                for (int i = 0; i < len; ++i)
                    this->indeces_[i] = i;
                for (int k = 0; k < len-1; ++k) {
                    T p = 0.0;
                    for (int i = k; i < len; ++i) {
                        T t = L[i*len+k];
                        if (t < 0)
                            t *= -1;
                        if (t > p) {
                            p = t;
                            kd = i;
                        }
                    }
                    if (p == 0.0)
                        return false;
                    this->swaps(this->indeces_[kd],this->indeces_[k]);
                    for (int i = 0; i < len; ++i)
                        this->swaps(L[kd*len+i],L[k*len+i]);
                    for (int i = k+1; i < len; ++i) {
                        L[i*len+k] = L[i*len+k] / L[k*len+k];
                        for (int j = k+1; j < len; ++j)
                            L[i*len+j] -= L[i*len+k]*L[k*len+j];
                    }
                }
                return true;
            }

            bool LUPinverse(mtx::Matrix<T>& LU) {
                this->zero(this->state_x_,this->vars_);
                this->zero(this->state_y_,this->vars_);
                int len = LU.getRows();
                for (int i = 0; i < len; ++i) {
                    for (int j = 0; j < len; ++j)
                        this->square_[i*len+j] = 0.0;
                    this->square_[i*len+i] = 1.0;
                    for (int n = 0; n < len; ++n) {
                        T t = 0.0;
                        for (int m = 0; m <= n-1; ++m )
                            t += LU[n*len+m]*this->state_y_[m];
                        this->state_y_[n] = this->square_[i*len+this->indeces_[n]]-t;
                    }
                    for (int n = len-1; n>=0; --n) {
                        T t = 0.0;
                        for (int m = n+1; m < len; ++m)
                            t += LU[n*len+m] * this->state_x_[m];
                        this->state_x_[n] = (this->state_y_[n]-t) / LU[n*len+n];
                    }
                    for (int j = 0; j < len; ++j)
                        this->square_[i*len+j] = this->state_x_[j];
                }
                for (int i = 0; i < len; ++i)
                    for (int j = 0; j < len; ++j)
                        LU[i*len+j] = this->square_[j*len+i];
                return true;
            }

            // TODO actually test if invertible
            void inverse(mtx::Matrix<T>& I) {
                this-LUPdecompose(I);
                this->LUPinverse(I);
            }

            // TODO actually test if invertible
            void inverse(mtx::Matrix<T>& I, mtx::Matrix<T>& M) {
                I = M;
                this->LUPdecompose(I);
                this->LUPinverse(I);
            }

            int getStorageSize() {
                return this->square_.getSize();
            }


        private:
            mtx::Matrix<T> square_;
            int* indeces_;
            T* state_x_;
            T* state_y_;
            int vars_;
            T* storage_;

    };

}

    #endif

